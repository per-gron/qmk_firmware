/* Copyright 2019
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include "config_common.h"

/* USB Device descriptor parameter */
// #define VENDOR_ID       0x456B
// #define PRODUCT_ID      0x0001
// #define DEVICE_VER      0x0001
// #define MANUFACTURER    ErgoKB
// #define PRODUCT         Phoenix
#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x6465
#define DEVICE_VER      0x0001
#define MANUFACTURER    none
#define PRODUCT         onekey
#define DESCRIPTION     test board for qmk

/* key matrix size */
#define MATRIX_ROWS 1
#define MATRIX_COLS 1

#define MOUSEKEY_INTERVAL    10
#define MOUSEKEY_DELAY       0
#define MOUSEKEY_TIME_TO_MAX 60
#define MOUSEKEY_MAX_SPEED   7
#define MOUSEKEY_WHEEL_DELAY 0

// #define DIODE_DIRECTION ROW2COL

// #define MATRIX_ROW_PINS { B1, B0, A7, A6, A5, B7 }
// #define MATRIX_COL_PINS { B10, B12, B13, B14, B15, A8, A10 }
#define MATRIX_ROW_PINS {}
#define MATRIX_COL_PINS {}

// #define HAL_USE_SERIAL TRUE
// #define SPLIT_HAND_PIN B9
// #define SOFT_SERIAL_PIN A9
// #define SERIAL_USART_DRIVER SD1
// #define SERIAL_USART_TX_PAL_MODE 7
