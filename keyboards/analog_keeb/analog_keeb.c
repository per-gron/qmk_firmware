/* Copyright 2018 Jack Humbert <jack.humb@gmail.com>
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
#include "analog_keeb.h"

#include <stdint.h>

#include "print.h"
#include "quantum/quantum.h"

void board_init(void) {
    // At this point ChibiOS is initialized but QMK is just starting.

    // Set the crystal oscillator EN pin to high to enable it. (It's floating.)
    setPinOutput(C15);
    writePinHigh(C15);
}

void matrix_scan_kb(void) {
    // TODO
	matrix_scan_user();
}

void matrix_init_kb(void) {
    // TODO
	matrix_init_user();
    print("PER GRON TEST");

    // setPinOutput(B3);
    // for (;;) {
    //     togglePin(B3);
    //     for (int i = 0; i < 10000; i++);
    // }
}
