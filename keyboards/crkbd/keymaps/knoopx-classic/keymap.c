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

#include QMK_KEYBOARD_H

enum layers {
  LY_QWERTY,
  LY_NUMBER,
  LY_SYMBOL,
  LY_EMPTY1,
  LY_EMPTY2,
  LY_EMPTY3,
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
  [LY_ADJUST] = LAYOUT_split_3x6_3(
      RGB_TOG, RGB_SPD, KC_VOLU, RGB_SPI, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, RGB_VAI, RGB_VAD, XXXXXXX,
      XXXXXXX, RGB_HUD, KC_VOLD, RGB_HUI, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                          KC_MPRV, KC_MNXT, KC_MPLY,    _______, _______, _______
  )
};
