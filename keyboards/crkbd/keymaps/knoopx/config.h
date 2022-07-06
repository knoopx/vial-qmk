/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#define VIAL_KEYBOARD_UID                                                      \
  { 0x3B, 0x6B, 0xA0, 0x29, 0x80, 0x56, 0xED, 0xD1 }

#define DYNAMIC_KEYMAP_LAYER_COUNT 10
#define DYNAMIC_KEYMAP_MACRO_COUNT 10

#define VIAL_COMBO_ENTRIES 10
#define VIAL_KEY_OVERRIDE_ENTRIES 10
#define VIAL_TAP_DANCE_ENTRIES 10

#define RGB_MATRIX_RAINBOW_INDICATORS
#define RGB_MATRIX_IDLE_TIMEOUT 1000 * 60
#define RGB_DISABLE_TIMEOUT 1000 * 60 * 30
#define RGB_DISABLE_WHEN_USB_SUSPENDED

#define EE_HANDS
#define SPLIT_USB_DETECT
#define SPLIT_TRANSPORT_MIRROR
#define SPLIT_LAYER_STATE_ENABLE
#define SPLIT_LED_STATE_ENABLE
#define SPLIT_MODS_ENABLE
#define SPLIT_TRANSACTION_IDS_USER LAST_INPUT_TIME_SYNC

#undef NO_ACTION_ONESHOT
#undef NO_ACTION_TAPPING
#undef NO_ACTION_LAYER
#undef NO_ACTION_MACRO
#undef NO_ACTION_FUNCTION

#define RGB_MATRIX_MAXIMUM_BRIGHTNES 200
#define RGB_MATRIX_STARTUP_HUE 90
#define RGB_MATRIX_STARTUP_MODE RGB_MATRIX_SOLID_COLOR
#define RGB_MATRIX_FRAMEBUFFER_EFFECTS

#define ENABLE_RGB_MATRIX_TYPING_HEATMAP
#define ENABLE_RGB_MATRIX_ALPHAS_MODS
#define ENABLE_RGB_MATRIX_BAND_PINWHEEL_VAL
#define ENABLE_RGB_MATRIX_BAND_SPIRAL_VAL
#define ENABLE_RGB_MATRIX_BAND_VAL
#define ENABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT
#define ENABLE_RGB_MATRIX_GRADIENT_UP_DOWN