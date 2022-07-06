/* Copyright 2017 Jason Williams (Wilba)
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

#include <stdint.h>
#include <stdbool.h>

#ifdef VIAL_ENABLE
#include "vial.h"
#endif

uint8_t  dynamic_keymap_get_layer_count(void);
void *   dynamic_keymap_key_to_eeprom_address(uint8_t layer, uint8_t row, uint8_t column);
uint16_t dynamic_keymap_get_keycode(uint8_t layer, uint8_t row, uint8_t column);
void     dynamic_keymap_set_keycode(uint8_t layer, uint8_t row, uint8_t column, uint16_t keycode);
#if defined(ENCODER_MAP_ENABLE) || defined(VIAL_ENCODERS_ENABLE)
uint16_t dynamic_keymap_get_encoder(uint8_t layer, uint8_t encoder_id, bool clockwise);
void     dynamic_keymap_set_encoder(uint8_t layer, uint8_t encoder_id, bool clockwise, uint16_t keycode);
#endif // ENCODER_MAP_ENABLE
#ifdef QMK_SETTINGS
uint8_t dynamic_keymap_get_qmk_settings(uint16_t offset);
void dynamic_keymap_set_qmk_settings(uint16_t offset, uint8_t value);
#endif
#ifdef VIAL_TAP_DANCE_ENABLE
int dynamic_keymap_get_tap_dance(uint8_t index, vial_tap_dance_entry_t *entry);
int dynamic_keymap_set_tap_dance(uint8_t index, const vial_tap_dance_entry_t *entry);
#endif
#ifdef VIAL_COMBO_ENABLE
int dynamic_keymap_get_combo(uint8_t index, vial_combo_entry_t *entry);
int dynamic_keymap_set_combo(uint8_t index, const vial_combo_entry_t *entry);
#endif
#ifdef VIAL_KEY_OVERRIDE_ENABLE
int dynamic_keymap_get_key_override(uint8_t index, vial_key_override_entry_t *entry);
int dynamic_keymap_set_key_override(uint8_t index, const vial_key_override_entry_t *entry);
#endif
void dynamic_keymap_reset(void);
// These get/set the keycodes as stored in the EEPROM buffer
// Data is big-endian 16-bit values (the keycodes)
// Order is by layer/row/column
// Thus offset 0 = 0,0,0, offset MATRIX_COLS*2 = 0,1,0, offset MATRIX_ROWS*MATRIX_COLS*2 = 1,0,0
// Note the *2, because offset is in bytes and keycodes are two bytes
// This is only really useful for host applications that want to get a whole keymap fast,
// by reading 14 keycodes (28 bytes) at a time, reducing the number of raw HID transfers by
// a factor of 14.
void dynamic_keymap_get_buffer(uint16_t offset, uint16_t size, uint8_t *data);
void dynamic_keymap_set_buffer(uint16_t offset, uint16_t size, uint8_t *data);

// This overrides the one in quantum/keymap_common.c
// uint16_t keymap_key_to_keycode(uint8_t layer, keypos_t key);

// Note regarding dynamic_keymap_macro_set_buffer():
// The last byte of the buffer is used as a valid flag,
// so macro sending is disabled during writing a new buffer,
// should it happen during, or after an interrupted transfer.
//
// Users writing to the buffer must first set the last byte of the buffer
// to non-zero (i.e. 0xFF). After (or during) the final write, set the
// last byte of the buffer to zero.
//
// Since the contents of the buffer must be a list of null terminated
// strings, the last byte must be a null when at maximum capacity,
// and it not being null means the buffer can be considered in an
// invalid state.

uint8_t  dynamic_keymap_macro_get_count(void);
uint16_t dynamic_keymap_macro_get_buffer_size(void);
void     dynamic_keymap_macro_get_buffer(uint16_t offset, uint16_t size, uint8_t *data);
void     dynamic_keymap_macro_set_buffer(uint16_t offset, uint16_t size, uint8_t *data);
void     dynamic_keymap_macro_reset(void);

void dynamic_keymap_macro_send(uint8_t id);
