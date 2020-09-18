#include <stdint.h>
#include <string.h>

#include "i2c_master.h"
#include "ldc1614_registers.h"
#include "moving_average.h"
#include "matrix.h"
#include "quantum/quantum.h"

// The larger this value is, the smoother (and slower) the analog response will be.
#define ANALOG_AVERAGE_SAMPLE_COUNT 40
// A larger value makes slower keypresses and releases count, makes the algorithm more sensitive to background value shifts.
#define KEYPRESS_AVERAGE_SAMPLE_COUNT 80
#define KEY_PRESS_THRESHOLD 4000
#define KEY_RELEASE_THRESHOLD 3000

static matrix_row_t matrix[MATRIX_ROWS];
static const uint16_t DEFAULT_I2C_TIMEOUT_MS = 5;

static const I2CConfig i2c_config = {
  OPMODE_I2C,
  400000,
  FAST_DUTY_CYCLE_2,
};

#define UNTRANSLATED_DEVICE_ADDRESS(addr_pin) ((addr_pin) ? 0x2b : 0x2a)
#define DEVICE_ADDRESS(addr_pin, translated) UNTRANSLATED_DEVICE_ADDRESS(addr_pin) - ((translated) ? 2 : 0)

static const uint16_t ldc1614_addresses[] = {
    DEVICE_ADDRESS(/*addr_pin=*/1, /*translated=*/1),
    DEVICE_ADDRESS(/*addr_pin=*/0, /*translated=*/1),
    DEVICE_ADDRESS(/*addr_pin=*/1, /*translated=*/0),
    DEVICE_ADDRESS(/*addr_pin=*/0, /*translated=*/0),
    DEVICE_ADDRESS(/*addr_pin=*/1, /*translated=*/1),
    DEVICE_ADDRESS(/*addr_pin=*/0, /*translated=*/1),
    DEVICE_ADDRESS(/*addr_pin=*/1, /*translated=*/0),
    DEVICE_ADDRESS(/*addr_pin=*/0, /*translated=*/0),
    DEVICE_ADDRESS(/*addr_pin=*/1, /*translated=*/0),
    DEVICE_ADDRESS(/*addr_pin=*/0, /*translated=*/0),
    DEVICE_ADDRESS(/*addr_pin=*/1, /*translated=*/1),
    DEVICE_ADDRESS(/*addr_pin=*/0, /*translated=*/1),
};

#define CHANNELS_PER_LDC1614 4
#define LDC1614_COUNT (sizeof(ldc1614_addresses)/sizeof(*ldc1614_addresses))

static MovingAverage analog_moving_averages[LDC1614_COUNT][CHANNELS_PER_LDC1614];
// Stores recent analog readouts when the key was not pressed.
static MovingAverage key_released_moving_averages[LDC1614_COUNT][CHANNELS_PER_LDC1614];
// Stores the release threshold while the key is pressed.
static MovingAverage key_pressed_moving_averages[LDC1614_COUNT][CHANNELS_PER_LDC1614];
static int32_t max_key_pressed_moving_averages[LDC1614_COUNT][CHANNELS_PER_LDC1614];

static I2CDriver* const ldc1614_i2c_drivers[] = {
    &I2CD1,
    &I2CD1,
    &I2CD1,
    &I2CD1,
    &I2CD2,
    &I2CD2,
    &I2CD2,
    &I2CD2,
    &I2CD3,
    &I2CD3,
    &I2CD3,
    &I2CD3,
};

static msg_t i2c_read_reg(uint8_t ldc1614_id, uint8_t regaddr, uint8_t* data, uint16_t length) {
    I2CDriver* driver = ldc1614_i2c_drivers[ldc1614_id];
    uint8_t devaddr = ldc1614_addresses[ldc1614_id];
    i2cStart(driver, &i2c_config);
    return i2cMasterTransmitTimeout(driver, devaddr, &regaddr, 1, data, length, TIME_MS2I(DEFAULT_I2C_TIMEOUT_MS));
}

static msg_t i2c_read_16bit_reg(uint8_t ldc1614_id, uint8_t regaddr, uint16_t* data) {
    uint8_t buf[2];
    msg_t result = i2c_read_reg(ldc1614_id, regaddr, buf, sizeof(buf));
    // Endian conversion.
    *data = buf[1] | (buf[0] << 8);
    return result;
}

static msg_t i2c_write_reg(uint8_t ldc1614_id, uint8_t regaddr, const uint8_t* data, uint16_t length) {
    I2CDriver* driver = ldc1614_i2c_drivers[ldc1614_id];
    uint8_t devaddr = ldc1614_addresses[ldc1614_id];
    i2cStart(driver, &i2c_config);

    uint8_t complete_packet[length + 1];
    for (uint8_t i = 0; i < length; i++) {
        complete_packet[i + 1] = data[i];
    }
    complete_packet[0] = regaddr;

    return i2cMasterTransmitTimeout(driver, devaddr, complete_packet, length + 1, 0, 0, TIME_MS2I(DEFAULT_I2C_TIMEOUT_MS));
}

static msg_t i2c_write_16bit_reg(uint8_t ldc1614_id, uint8_t regaddr, uint16_t data) {
    uint8_t buf[2];
    // Endian conversion.
    buf[0] = data >> 8;
    buf[1] = data & 0xff;
    return i2c_write_reg(ldc1614_id, regaddr, buf, sizeof(buf));
}

static void initialize_ldc1614_channel(uint8_t ldc1614_id, uint8_t channel_id) {
    Ldc1614ClockDividers dividers;
    memset(&dividers, 0, sizeof(dividers));
    // The calculated sensor frequency is just below 5MHz. This is less than 8.75MHz, so
    // fin_divider can be 1.
    dividers.fin_divider = 1;
    // 5MHz is also < 40MHz / 4, so fref_divider can also be 1
    dividers.fref_divider = 1;
    msg_t result = i2c_write_16bit_reg(ldc1614_id, CLOCK_DIVIDER_REGISTER(channel_id), dividers.val);
    if (result != 0) {
        xprintf("Failed to write clock divider register: %d\n", result);
    }

    // SETTLECOUNT ≥ Q × fREF / (16 × fSENSOR)
    //
    // The calculated Q factor is 60. fREF0 is 40MHz, calculated fSENSOR is 5MHz, so
    // SETTLECOUNT ≥ (60 * 40 / (16 * 5) = 30). Choosing 40 here to have some margin.
    // 
    // The settling time is SETTLECOUNT * 16 / fREF, so in this case 40*16/40e6s = 16us
    uint16_t settle_count = 40;
    result = i2c_write_16bit_reg(ldc1614_id, SETTLE_COUNT_REGISTER(channel_id), settle_count);
    if (result != 0) {
        xprintf("Failed to write settle count register: %d\n", result);
    }

    Ldc1614DriveCurrent drive_current;
    memset(&drive_current, 0, sizeof(drive_current));
    // The number 21 was derived by trying different values. It resulted in a sensor amplitude of 1.7 V
    // which was the closest to the middle of the 1.5V - 1.75V range suggested by the datasheet.
    drive_current.idrive = 21;
    result = i2c_write_16bit_reg(ldc1614_id, DRIVE_CURRENT_REGISTER(channel_id), drive_current.val);
    if (result != 0) {
        xprintf("Failed to write drive current register: %d\n", result);
    }

    // RCOUNT, aka reference count.
    //
    // * The conversion time for a given channel is (RCOUNT * 16 + 4) / fREF.
    // * The channel switch delay is 692ns + 5/fREF = 692e-9 + 5 / 40e6 = 0.82us
    // * The settling time is defined above
    //
    // The sampling period is [number of channels] * ([conversion time] + [switch delay] + [settling time]).
    //
    // For a 333.3Hz refresh rate, aka 3ms sampling period, we can afford a conversion
    // time of (3000-(0.82+16))/4 = 745.8us, which gives an RCOUNT of 
    // (745.8e-6 * 40e6 - 4)/16 = 1864
    uint16_t reference_count = 1864;
    reference_count = 0xffff / 15; // / 10; // TODO
    result = i2c_write_16bit_reg(ldc1614_id, REFERENCE_COUNT_REGISTER(channel_id), reference_count);
    if (result != 0) {
        xprintf("Failed to write reference count register: %d\n", result);
    }
}

static void initialize_ldc1614(uint8_t ldc1614_id) {
    Ldc1614Config config;
    memset(&config, 0, sizeof(config));
    // Put the sensor in sleep mode to enable configuration.
    config.sleep_mode_en = 1;
    // Set the reserved value to its default.
    config.reserved_1 = 1;
    msg_t result = i2c_write_16bit_reg(ldc1614_id, kLdc1614MuxConfigRegister, config.val);
    if (result != 0) {
        xprintf("Failed to write config register to make sensor sleep: %d\n", result);
    }

    Ldc1614MuxConfig mux_config;
    memset(&mux_config, 0, sizeof(mux_config));
    // Configure the sensor chip to scan multiple channels.
    mux_config.autoscan_en = 1;
    // Configure the sensor chip to scan all four channels.
    mux_config.rr_sequence = 2;
    // Set the reserved field to its default value.
    mux_config.reserved = 65;
    // Set the deglitch filter to 10MHz, the smallest available value greater
    // than the sensor oscillation frequency of 5MHz.
    mux_config.deglitch = 5;
    result = i2c_write_16bit_reg(ldc1614_id, kLdc1614MuxConfigRegister, mux_config.val);
    if (result != 0) {
        xprintf("Failed to write mux config register: %d\n", result);
    }

    for (int i = 0; i < 4; i++) {
        initialize_ldc1614_channel(ldc1614_id, i);
    }

    memset(&config, 0, sizeof(config));
    // Enable sensing.
    config.sleep_mode_en = 0;
    // Turn on RP override to control drive current with the drive current register.
    config.rp_override_en = 1;
    // Set full current mode for the current drive for sensor activation.
    config.sensor_activate_sel = 0;  // Recommended by table 41 in the datasheet.
    // Turn off automatic sensor amplitude correction, so the drive current register
    // is respected. The datasheet recommends this for improved precision.
    config.auto_amp_dis = 1;
    // Use internal oscillator.
    config.ref_clk_src = 0;
    // Disable the interrupt pin.
    config.intb_dis = 1;
    // Disable high current drive option (it does not work with multiple channels).
    config.high_current_drv = 0;
    // Set the reserved value to its default.
    config.reserved_1 = 1;
    result = i2c_write_16bit_reg(ldc1614_id, kLdc1614ConfigRegister, config.val);
    if (result != 0) {
        xprintf("Failed to write config register to enable sensor: %d\n", result);
    }
    
    // Set data offset registers (0x0C, 0x0D, 0x0E, 0x0F) (default = 0)
}

static uint32_t read_ldc1614_channel(uint8_t ldc1614_id, uint8_t channel_id) {
    if (false) {
        Ldc1614DriveCurrent drive_current;
        msg_t result = i2c_read_16bit_reg(ldc1614_id, DRIVE_CURRENT_REGISTER(channel_id), &drive_current.val);
        xprintf("\n  status for channel %d: %d\n", channel_id, result);
        if (result == MSG_RESET) {
            // Error could be for example I2C_ACK_FAILURE, defined in hal_i2c.h
            xprintf("    error3: %d\n", i2cGetErrors(&I2CD1));
        }
        prettyprint_drive_current_register(&drive_current);
    }

    // As per the LDC1614 datasheet section 7.5.3, the data registers must be read in the order: MSB, LSB.
    uint16_t msb;
    msg_t result = i2c_read_16bit_reg(ldc1614_id, DATA_MSB_REGISTER(channel_id), &msb);
    if (result == MSG_RESET) {
        xprintf("Failed to read MSB data register: %d\n", result);
    }
    uint16_t lsb;
    result = i2c_read_16bit_reg(ldc1614_id, DATA_LSB_REGISTER(channel_id), &lsb);
    if (result == MSG_RESET) {
        xprintf("Failed to read LSB data register: %d\n", result);
    }

    uint32_t value = ((msb & 0xfff) << 16) + lsb;
    // The following calculates the error status provided by the sensor:
    // uint16_t error = msb >> 12;
    return value;
}

static void read_ldc1614_data(uint8_t ldc1614_id) {
    int num_channels = 1;  // TODO Increase range to 4.
    for (int i = 0; i < num_channels; i++) {
        int32_t value = read_ldc1614_channel(ldc1614_id, i);

        MovingAverage* analog_moving_average = &analog_moving_averages[ldc1614_id][i];
        moving_average_push_value(analog_moving_average, value);

        MovingAverage* key_released_moving_average = &key_released_moving_averages[ldc1614_id][i];
        MovingAverage* key_pressed_moving_average = &key_pressed_moving_averages[ldc1614_id][i];
        if (!key_released_moving_average->buffer_is_full) {
            moving_average_push_value(key_released_moving_average, value);
        } else {
            // TODO Calculate the row and col and use it here.
            uint8_t row = 0;
            uint8_t col = i;
            matrix_row_t key_mask = (1 << col);
            bool key_pressed_before = !!(matrix[row] & key_mask);

            bool key_pressed;
            if (key_pressed_before) {
                int32_t avg = moving_average_get(key_pressed_moving_average);
                // In order to reduce the possibility of slowly releasing a key not
                // being registered as a released key, count the key as released if
                // the value is less than the greatest threshold value we've seen
                // during this keypress.
                if (avg > max_key_pressed_moving_averages[ldc1614_id][i]) {
                    max_key_pressed_moving_averages[ldc1614_id][i] = avg;
                }
                key_pressed = value >= max_key_pressed_moving_averages[ldc1614_id][i];
            } else {
                key_pressed = value >= moving_average_get(key_released_moving_average) + KEY_PRESS_THRESHOLD;
            }

            if (key_pressed && !key_pressed_before) {
                moving_average_set(
                    key_pressed_moving_average,
                    moving_average_get(key_released_moving_average) + KEY_PRESS_THRESHOLD - KEY_RELEASE_THRESHOLD);
            } else if (!key_pressed && key_pressed_before) {
                moving_average_set(
                    key_released_moving_average,
                    moving_average_get(key_pressed_moving_average) - KEY_PRESS_THRESHOLD + KEY_RELEASE_THRESHOLD);
            }
            if (key_pressed) {
                int32_t initial = value - KEY_RELEASE_THRESHOLD;
                moving_average_push_value(key_pressed_moving_average, initial);
                max_key_pressed_moving_averages[ldc1614_id][i] = initial;
            } else {
                moving_average_push_value(key_released_moving_average, value);
            }

            matrix[row] = (matrix[row] & ~key_mask) | (key_pressed ? key_mask : 0);

            xprintf(
                "%d\t%d\t%d\t",
                moving_average_get(key_released_moving_average),
                moving_average_get(key_pressed_moving_average),
                key_pressed);
            xprintf("%d%c", value, (i == num_channels - 1 ? '\n' : '\t'));
            // xprintf("%d%c", key_pressed, (i == num_channels - 1 ? '\n' : '\t'));
        }
    }
}

matrix_row_t matrix_get_row(uint8_t row) {
    return 0; // matrix[row]; TODO Uncomment me
}

void matrix_print(void) {
    // TODO: use print() to dump the current matrix state to console
}

void matrix_init(void) {
    // Initialize hardware and global matrix state here.

    // Initialize matrix state: all keys off.
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
    }

    for (uint8_t i = 0; i < LDC1614_COUNT; i++) {
        for (uint8_t j = 0; j < CHANNELS_PER_LDC1614; j++) {
            moving_average_init(&analog_moving_averages[i][j], ANALOG_AVERAGE_SAMPLE_COUNT);
            moving_average_init(&key_released_moving_averages[i][j], KEYPRESS_AVERAGE_SAMPLE_COUNT);
            moving_average_init(&key_pressed_moving_averages[i][j], KEYPRESS_AVERAGE_SAMPLE_COUNT);
        }
    }

    initialize_ldc1614(/*ldc1614_id=*/2);

    // This *must* be called for correct keyboard behavior
    matrix_init_quantum();
}

uint8_t matrix_scan(void) {
    bool matrix_has_changed = false;

    wait_ms(5);
    // print("\n\nReading sensor data:\n");
    read_ldc1614_data(/*ldc1614_id=*/2);
    if (false) {
        print("Reading manufacturer ID...\n");
        uint8_t manufacturer_id_regaddr = 0x7E;
        uint16_t manufacturer_id = 0;
        msg_t result = i2c_read_16bit_reg(/*ldc1614_id=*/2, manufacturer_id_regaddr, &manufacturer_id);
        xprintf("  status: %d\n", result);
        if (result == MSG_RESET) {
            // Error could be for example I2C_ACK_FAILURE, defined in hal_i2c.h
            printf("  error: %d\n", i2cGetErrors(&I2CD1));
        }
        xprintf("  data: %d\n", manufacturer_id);

        Ldc1614Config config;
        result = i2c_read_16bit_reg(/*ldc1614_id=*/2, kLdc1614ConfigRegister, &config.val);
        xprintf("\n  status2: %d\n", result);
        if (result == MSG_RESET) {
            // Error could be for example I2C_ACK_FAILURE, defined in hal_i2c.h
            printf("  error2: %d\n", i2cGetErrors(&I2CD1));
        }
        prettyprint_config_register(&config);

        Ldc1614MuxConfig mux_config;
        result = i2c_read_16bit_reg(/*ldc1614_id=*/2, kLdc1614MuxConfigRegister, &mux_config.val);
        xprintf("\n  status3: %d\n", result);
        if (result == MSG_RESET) {
            // Error could be for example I2C_ACK_FAILURE, defined in hal_i2c.h
            printf("  error3: %d\n", i2cGetErrors(&I2CD1));
        }
        prettyprint_mux_config_register(&mux_config);

        Ldc1614Status status;
        result = i2c_read_16bit_reg(/*ldc1614_id=*/2, kLdc1614StatusRegister, &status.val);
        xprintf("\n  status4: %d\n", result);
        if (result == MSG_RESET) {
            // Error could be for example I2C_ACK_FAILURE, defined in hal_i2c.h
            printf("  error4: %d\n", i2cGetErrors(&I2CD1));
        }
        prettyprint_status_register(&status);

        Ldc1614ClockDividers dividers0;
        result = i2c_read_16bit_reg(/*ldc1614_id=*/2, CLOCK_DIVIDER_REGISTER(0), &dividers0.val);
        xprintf("\n  status5: %d\n", result);
        if (result == MSG_RESET) {
            // Error could be for example I2C_ACK_FAILURE, defined in hal_i2c.h
            printf("  error5: %d\n", i2cGetErrors(&I2CD1));
        }
        prettyprint_clock_dividers_register(&dividers0);
    }

    // TODO: add matrix scanning routine here

    // This *must* be called for correct keyboard behavior
    matrix_scan_quantum();

    return matrix_has_changed;
}

__attribute__((weak)) void matrix_init_kb(void) { matrix_init_user(); }

__attribute__((weak)) void matrix_scan_kb(void) { matrix_scan_user(); }

__attribute__((weak)) void matrix_init_user(void) {}

__attribute__((weak)) void matrix_scan_user(void) {}
