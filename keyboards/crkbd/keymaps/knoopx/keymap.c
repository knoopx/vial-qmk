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

#include <lib/lib8tion/lib8tion.h>
#include <math.h>

#include QMK_KEYBOARD_H

#include "transactions.h"

#define RGB_MATRIX_LAYER_HUE_DELTA 42.5f

//    0  1  2  3  4  5    8  9  10 11 12 13
// 0  25 24 19 18 11 10   10 11 18 19 24 25
// 1  26 23 20 17 12 09   09 12 17 20 23 26
// 2  27 22 21 16 13 08   08 13 16 21 22 27
// 3           15 14 07   07 14 15

const uint8_t rgb_matrix_capslock_indicator_pos[2] = {2, 0};

// HELPER METHODS

HSV hsv_set_hue(HSV hsv, int hue) {
  HSV shifted = {.h = hue, .s = hsv.s, .v = hsv.v};
  return shifted;
}

HSV hsv_hue_shift(HSV hsv, int delta) {
  return hsv_set_hue(hsv, (hsv.h + delta) % 255);
}

void rgb_matrix_set_hsv_color(int index, HSV hsv) {
  RGB rgb = hsv_to_rgb(hsv);
  rgb_matrix_set_color(index, rgb.r, rgb.g, rgb.b);
}

void rgb_matrix_set_indicator(int index) {
#ifdef RGB_MATRIX_RAINBOW_INDICATORS
  uint8_t time = scale16by8(g_rgb_timer, 16);
  HSV     hsv = rgb_matrix_config.hsv;
  hsv.h = time;
  rgb_matrix_set_hsv_color(index, hsv);
#else
  rgb_matrix_set_hsv_color(index, hsv_hue_shift(rgb_matrix_config.hsv, 127));
#endif
}

void rgb_matrix_set_layer_color(int index, int layer) {
  rgb_matrix_set_hsv_color(
      index,
      hsv_hue_shift(rgb_matrix_config.hsv, layer * RGB_MATRIX_LAYER_HUE_DELTA));
}

int resolve_effective_kc_layer(keypos_t keypos, int layer) {
  if (layer == 0)
    return 0;

  int kc = keymap_key_to_keycode(layer, keypos);
  if (kc == KC_TRNS) {
    return resolve_effective_kc_layer(keypos, layer - 1);
  }
  return layer;
}

// API HOOKS

static uint32_t last_input_time = 0;
bool            last_input_time_needs_sync = false;

void last_input_time_sync_handler(uint8_t in_buflen, const void *in_data,
                                  uint8_t out_buflen, void *out_data) {
  last_input_time = sync_timer_read32();
}

void led_set_keymap(uint8_t usb_led) {
  if (!(usb_led & (1 << USB_LED_NUM_LOCK))) {
    // turn on numlock at startup
    tap_code(KC_NLCK);
  }
}

void housekeeping_task_user(void) {
  if (!is_keyboard_master())
    return;

  if (last_input_time_needs_sync) {
    if (transaction_rpc_exec(LAST_INPUT_TIME_SYNC, 0, NULL, 0, NULL)) {
      last_input_time_needs_sync = false;
    }
  }
}

void keyboard_post_init_user(void) {
  transaction_register_rpc(LAST_INPUT_TIME_SYNC, last_input_time_sync_handler);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  last_input_time = timer_read32();
  last_input_time_needs_sync = true;
  return true;
}

void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
  uint8_t layer = get_highest_layer(layer_state);
  uint8_t default_layer = get_highest_layer(default_layer_state);

  for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
    for (uint8_t col = 0; col < MATRIX_COLS; ++col) {

      uint8_t index = g_led_config.matrix_co[row][col];

      if (sync_timer_elapsed32(last_input_time) > (uint32_t)RGB_MATRIX_IDLE_TIMEOUT) {
        // SLEEPING
        HSV      hsv = rgb_matrix_config.hsv;
        uint16_t time = scale16by8((g_rgb_timer - last_input_time), 16);
        hsv.v = scale8(abs8(sin8(time) - 128) * 2, hsv.v);
        RGB rgb = hsv_to_rgb(hsv);
        rgb_matrix_set_color(index, rgb.r, rgb.g, rgb.b);
      } else {
        //  DEFAULT LAYER
        if (layer == default_layer) {
          // capslock
          if (row == rgb_matrix_capslock_indicator_pos[0] &&
              col == rgb_matrix_capslock_indicator_pos[1]) {
            if (IS_HOST_LED_ON(USB_LED_CAPS_LOCK)) {
              rgb_matrix_set_indicator(index);
            }
          }
        } else {
          // SUBLAYER
          keypos_t keypos = {col, row};

          // off
          if (keymap_key_to_keycode(layer, keypos) == KC_NO) {
            rgb_matrix_set_color(index, RGB_OFF);
          } else
            // transparent
            if (keymap_key_to_keycode(layer, keypos) == KC_TRNS) {
              rgb_matrix_set_layer_color(
                  index, resolve_effective_kc_layer(keypos, layer));
            } else {
              rgb_matrix_set_layer_color(index, layer);
            }
        }
      }
    }
  }
}

enum layers {
  LY_QWERTY,
  LY_NUMBER,
  LY_SYMBOL,
  LY_EMPTY1,
  LY_EMPTY2,
  LY_EMPTY3,
  LY_EMPTY4,
  LY_EMPTY5,
  LY_EMPTY6,
  LY_ADJUST,
};

#define MO_NUM MO(LY_NUMBER)
#define MO_SYM MO(LY_SYMBOL)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LY_QWERTY] = LAYOUT_split_3x6_3(
      KC_ESC,     KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,   KC_BSPC,
      KC_TAB,     KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_ENTER,
      KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  KC_QUOT,
                                          KC_LCTL, KC_LALT, KC_LGUI,      KC_SPC, MO_SYM,  MO_NUM
  ),
  [LY_NUMBER] = LAYOUT_split_3x6_3(
     KC_GRAVE,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  _______,
      _______, MACRO00, MACRO01, MACRO02, MACRO03, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   KC_UP,  XXXXXXX,
      _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, KC_LEFT, KC_DOWN, KC_RIGHT,
                                          _______, _______, _______,   _______, _______, _______
  ),
  [LY_SYMBOL] = LAYOUT_split_3x6_3(
      KC_TILD, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,                      KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______,
      _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_MINS,  KC_EQL, KC_LBRC, KC_RBRC, KC_BSLS,  KC_GRV,
      _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE, KC_TILD,
                                          _______,   MO(3), _______,   _______, _______, _______
  ),
  [LY_EMPTY1] = LAYOUT_split_3x6_3(
      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
                                          _______, _______, _______,    _______, _______, _______
  ),
  [LY_EMPTY2] = LAYOUT_split_3x6_3(
      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
                                          _______, _______, _______,    _______, _______, _______
  ),
  [LY_EMPTY3] = LAYOUT_split_3x6_3(
      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
                                          _______, _______, _______,    _______, _______, _______
  ),
  [LY_EMPTY4] = LAYOUT_split_3x6_3(
      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
                                          _______, _______, _______,    _______, _______, _______
  ),
  [LY_EMPTY5] = LAYOUT_split_3x6_3(
      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
                                          _______, _______, _______,    _______, _______, _______
  ),
  [LY_EMPTY6] = LAYOUT_split_3x6_3(
      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
                                          _______, _______, _______,    _______, _______, _______
  ),
  [LY_ADJUST] = LAYOUT_split_3x6_3(
      RGB_TOG, RGB_SPD, KC_VOLU, RGB_SPI, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, RGB_VAI, RGB_VAD, XXXXXXX,
      XXXXXXX, RGB_HUD, KC_VOLD, RGB_HUI, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                          KC_MPRV, KC_MNXT, KC_MPLY,    _______, _______, _______
  )
};
