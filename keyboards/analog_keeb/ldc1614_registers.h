#pragma once

#include <stdint.h>

static const uint8_t kLdc1614StatusRegister = 0x18;
typedef union {
    struct {
        // (Read-only) Channel 3 Unread Conversion. Default: 0.
        uint16_t unreadconv3 : 1;
        // (Read-only) Channel 2 Unread Conversion. Default: 0.
        uint16_t unreadconv2 : 1;
        // (Read-only) Channel 1 Unread Conversion. Default: 0.
        uint16_t unreadconv1 : 1;
        // (Read-only) Channel 0 Unread Conversion. Default: 0.
        uint16_t unreadconv0 : 1;
        // (Read-only) Reserved. Reads 00b.
        uint16_t reserved_0 : 2;
        // (Read-only) Data Ready Flag. Default: 0.
        uint16_t drdy : 1;
        // (Read-only) Reserved. Reads 0.
        uint16_t reserved_1 : 1;
        // (Read-only) Zero Count Error. Default: 0.
        uint16_t err_zc : 1;
        // (Read-only) Sensor Amplitude Low Error. Default: 0.
        uint16_t err_ale : 1;
        // (Read-only) Sensor Amplitude High Error. Default: 0.
        uint16_t err_ahe : 1;
        // (Read-only) Watchdog Timeout Error. Default: 0.
        uint16_t err_wd : 1;
        // (Read-only) Conversion Over-range Error. Default: 0.
        uint16_t err_or : 1;
        // (Read-only) Conversion Under-range Error. Default: 0.
        uint16_t err_ur : 1;
        // (Read-only) Error Channel. Default: b00.
        uint16_t err_chan : 2;
    };
    uint16_t val;
} Ldc1614Status;

void prettyprint_status_register(const Ldc1614Status* status);

// The reference count registers are just one 16 bit number, so there is no need for a
// struct like some other registers have.
//
// `channel_id` may be 0, 1, 2 or 3.
#define REFERENCE_COUNT_REGISTER(channel_id) (0x08 + (channel_id))

// The settle count registers are just one 16 bit number, so there is no need for a
// struct like some other registers have.
//
// `channel_id` may be 0, 1, 2 or 3.
#define SETTLE_COUNT_REGISTER(channel_id) (0x10 + (channel_id))

// `channel_id` may be 0, 1, 2 or 3.
#define CLOCK_DIVIDER_REGISTER(channel_id) (0x14 + (channel_id))
// The following union is used for all four clock dividers registers, as they
// share the same format.
typedef union {
    struct {
        // (Read-write) Reference Divider. Default value 0.
        uint16_t fref_divider : 10;
        // (Read-write) Reserved. Set to b00.
        uint16_t reserved : 2;
        // (Read-write) Input Divider. Default value 0.
        uint16_t fin_divider : 4;
    };
    uint16_t val;
} Ldc1614ClockDividers;

void prettyprint_clock_dividers_register(const Ldc1614ClockDividers* clock_dividers);

// `channel_id` may be 0, 1, 2 or 3.
#define DRIVE_CURRENT_REGISTER(channel_id) (0x1e + (channel_id))
// The following union is used for all four drive current registers, as they
// share the same format.
typedef union {
    struct {
        // (Read-write) Reserved. Set to b00 0000.
        uint16_t reserved : 6;
        // (Read-only) Sensor Current Drive.
        //
        // When writing this register, set this field to 0.
        uint16_t init_drive : 5;
        // (Read-only) L-C Sensor Drive Current. Default: 0.
        uint16_t idrive : 5;
    };
    uint16_t val;
} Ldc1614DriveCurrent;

#define DATA_MSB_REGISTER(channel_id) ((channel_id) * 2)
#define DATA_LSB_REGISTER(channel_id) ((channel_id) * 2 + 1)

void prettyprint_drive_current_register(const Ldc1614DriveCurrent* drive_current);

static const uint8_t kLdc1614ConfigRegister = 0x1a;
typedef union {
    struct {
        // (Read-write) Reserved. Set to b00'0001
        uint16_t reserved_1 : 6;
        // High Current Sensor Drive. Default: 0.
        uint16_t high_current_drv : 1;
        // (Read-write) INTB Disable. Default: 0.
        uint16_t intb_dis : 1;
        // (Read-write) Reserved. Set to b0.
        uint16_t reserved_0 : 1;
        // (Read-write) Select Reference Frequency Source. Default: 0.
        uint8_t ref_clk_src : 1;
        // (Read-write) Automatic Sensor Amplitude Correction Disable. Default: 0.
        uint8_t auto_amp_dis : 1;
        // (Read-write) Sensor Activation Mode Selection. Default: 0.
        uint8_t sensor_activate_sel : 1;
        // (Read-write) Sensor RP Override Enable. Default: 0.
        uint8_t rp_override_en : 1;
        // (Read-write) Sleep Mode Enable. Default: 1.
        uint8_t sleep_mode_en : 1;
        // (Read-write) Active channel selection. Default: 0.
        uint16_t active_chan : 2;
    };
    uint16_t val;
} Ldc1614Config;

void prettyprint_config_register(const Ldc1614Config* config);

static const uint8_t kLdc1614MuxConfigRegister = 0x1b;
typedef union {
    struct {
        // (Read-write) Input Deglitch Filter Bandwidth. Default: b111.
        uint16_t deglitch : 3;
        // (Read-write) Reserved. Set to 00 0100 0001.
        uint16_t reserved : 10;
        // (Read-write) Auto-Scan Sequence Configuration. Default: 0.
        uint16_t rr_sequence : 2;
        // (Read-write) Auto-Scan Mode Enable. Default: 0.
        uint16_t autoscan_en : 1;
    };
    uint16_t val;
} Ldc1614MuxConfig;

void prettyprint_mux_config_register(const Ldc1614MuxConfig* mux_config);
