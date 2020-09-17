#include <stdint.h>

#include "i2c_master.h"
#include "matrix.h"
#include "quantum/quantum.h"

static matrix_row_t matrix[MATRIX_ROWS];

static const I2CConfig i2ccfg_1 = {
  OPMODE_I2C,
  400000,
  FAST_DUTY_CYCLE_2,
};

// The I2C address of LDC1614 devices when the ADDR pin is set low and high, respectively.
static const uint8_t untranslated_device_addresses[] = { 0x2a, 0x2b };

msg_t i2c_read_reg(I2CDriver* driver, const I2CConfig* i2c_config, uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length, uint16_t timeout_ms) {
    i2cStart(driver, i2c_config);
    return i2cMasterTransmitTimeout(driver, devaddr, &regaddr, 1, data, length, TIME_MS2I(timeout_ms));
}

msg_t i2c_write_reg(I2CDriver* driver, const I2CConfig* i2c_config, uint8_t devaddr, uint8_t regaddr, const uint8_t* data, uint16_t length, uint16_t timeout_ms) {
    i2cStart(driver, i2c_config);

    uint8_t complete_packet[length + 1];
    for (uint8_t i = 0; i < length; i++) {
        complete_packet[i + 1] = data[i];
    }
    complete_packet[0] = regaddr;

    return i2cMasterTransmitTimeout(driver, devaddr, complete_packet, length + 1, 0, 0, TIME_MS2I(timeout_ms));
}


matrix_row_t matrix_get_row(uint8_t row) {
    return matrix[row];
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

    // This *must* be called for correct keyboard behavior
    matrix_init_quantum();
}

uint8_t matrix_scan(void) {
    bool matrix_has_changed = false;

    print("Reading manufacturer ID...\n");
    uint8_t manufacturer_id_regaddr = 0x7E;
    uint16_t manufacturer_id = 0;
    msg_t result =  i2c_read_reg(&I2CD1, &i2ccfg_1, untranslated_device_addresses[1], manufacturer_id_regaddr, (uint8_t*)&manufacturer_id, sizeof(manufacturer_id), 100);
    // msg_t result = i2c_write_reg(&I2CD1, &i2ccfg_1, untranslated_device_addresses[1], manufacturer_id_regaddr, (uint8_t*)&manufacturer_id, sizeof(manufacturer_id), 500);
    xprintf("  status: %d\n", result);
    if (result == MSG_RESET) {
        // Error could be for example I2C_ACK_FAILURE, defined in hal_i2c.h
        printf("  error: %d\n", i2cGetErrors(&I2CD1));
    }
    xprintf("  data: %d\n", manufacturer_id);
    wait_ms(1000);

    // TODO: add matrix scanning routine here

    // This *must* be called for correct keyboard behavior
    matrix_scan_quantum();

    return matrix_has_changed;
}

__attribute__((weak)) void matrix_init_kb(void) { matrix_init_user(); }

__attribute__((weak)) void matrix_scan_kb(void) { matrix_scan_user(); }

__attribute__((weak)) void matrix_init_user(void) {}

__attribute__((weak)) void matrix_scan_user(void) {}
