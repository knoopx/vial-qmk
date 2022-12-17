#include <lib/lib8tion/lib8tion.h>
#include <math.h>

#include QMK_KEYBOARD_H

#define RGB_MATRIX_LAYER_HUE_DELTA 42.5f

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

void led_set_keymap(uint8_t usb_led) {
  if (!(usb_led & (1 << USB_LED_NUM_LOCK))) {
    // turn on numlock at startup
    tap_code(KC_NLCK);
  }
}

void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
  uint8_t layer = get_highest_layer(layer_state);
  uint8_t default_layer = get_highest_layer(default_layer_state);

  for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
    for (uint8_t col = 0; col < MATRIX_COLS; ++col) {

      uint8_t index = g_led_config.matrix_co[row][col];

      if (last_input_activity_elapsed() > (uint32_t)RGB_MATRIX_IDLE_TIMEOUT) {
        // SLEEPING
        HSV      hsv = rgb_matrix_config.hsv;
        uint16_t time = scale16by8((g_rgb_timer - last_input_activity_time()), 16);
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

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
LAYOUT_planck_mit(
    KC_ESCAPE, KC_Q,    KC_W,    KC_E,  KC_R,  KC_T,     KC_Y, KC_U,  KC_I,     KC_O,    KC_P,      KC_BSPACE,
    KC_TAB,    KC_A,    KC_S,    KC_D,  KC_F,  KC_G,     KC_H, KC_J,  KC_K,     KC_L,    KC_SCOLON, KC_ENTER,
    KC_LSHIFT, KC_Z,    KC_X,    KC_C,  KC_V,  KC_B,     KC_N, KC_M,  KC_COMMA, KC_DOT,  KC_UP,     KC_SLASH,
    KC_LCTRL,  KC_LGUI, KC_LALT, MO(6), MO(4),    KC_SPACE,    MO(2), MO(3),    KC_LEFT, KC_DOWN,   KC_RIGHT
),
LAYOUT_planck_mit(
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, MO(5),   _______, _______, _______, _______, _______, _______, _______
),
LAYOUT_planck_mit(
    LSFT(KC_GRAVE), LSFT(KC_1),     LSFT(KC_2),   LSFT(KC_3), LSFT(KC_4), LSFT(KC_5), LSFT(KC_6), LSFT(KC_7), LSFT(KC_8),     LSFT(KC_9),        LSFT(KC_0),        _______,
    _______,        _______,        _______,      _______,    _______,    _______,    KC_QUOTE,   KC_MINUS,   LSFT(KC_EQUAL), LSFT(KC_LBRACKET), LSFT(KC_RBRACKET), LSFT(KC_BSLASH),
    _______,        LSFT(KC_COMMA), LSFT(KC_DOT), _______,    _______,    _______,    _______,    KC_EQUAL,   _______,        KC_LBRACKET,       KC_RBRACKET,       KC_BSLASH,
    _______,        _______,        _______,      _______,    _______,            _______,        _______,    _______,        _______,           _______,           _______
),
LAYOUT_planck_mit(
    KC_GRAVE, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,          KC_0,      _______,
    _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,       _______,   _______,
    _______,  _______, _______, _______, _______, _______, _______, _______, _______, RCTL(KC_PGUP), KC_PGUP,   RCTL(KC_PGDOWN),
    _______,  _______, _______, _______, _______,      _______,     _______, _______, KC_HOME,       KC_PGDOWN, KC_END
),
LAYOUT_planck_mit(
    _______, _______, _______, _______, _______, _______,   _______, _______, KC_KP_ASTERISK, KC_KP_7, KC_KP_8,     KC_KP_9,
    _______, _______, _______, _______, _______, _______,   _______, _______, KC_KP_PLUS,     KC_KP_4, KC_KP_5,     KC_KP_6,
    _______, _______, _______, _______, _______, _______,   _______, _______, KC_KP_MINUS,    KC_KP_1, KC_KP_2,     KC_KP_3,
    _______, _______, _______, _______, _______,      KC_BSPACE,     _______, KC_KP_SLASH,    KC_KP_0, KC_KP_COMMA, KC_KP_EQUAL
),
LAYOUT_planck_mit(
    _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   _______, _______, _______,       KC_PSCREEN, KC_DELETE,
    _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______, _______,       _______,    KC_INSERT,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, LCTL(KC_PGUP), KC_PGUP,    LCTL(KC_PGDOWN),
    _______, _______, _______, _______, _______,      _______,     _______, _______, KC_HOME,       KC_PGDOWN,  KC_END
),
LAYOUT_planck_mit(
    _______, KC_KP_7, KC_KP_8, KC_KP_9, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, KC_KP_1, KC_KP_2, KC_KP_3, _______, _______, _______, _______, _______, _______, _______, KC_MPLY,
    _______, KC_NO,   KC_KP_5, KC_KP_0, _______, _______, _______, _______, _______, _______, KC_VOLU, _______,
    _______, _______, _______, _______, _______,      _______,     _______, _______, KC_MPRV, KC_VOLD, KC_MNXT
),
LAYOUT_planck_mit(
    RGB_TOG, DF(1),   DF(0),   _______, _______, _______, _______, _______, _______, RGB_SPD, RGB_SPI,  RESET,
    _______, RGB_M_P, RGB_M_R, _______, _______, _______, _______, _______, _______, _______, _______,  _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, RGB_VAD, RGB_MOD,  RGB_VAI,
    _______, _______, _______, _______, _______,      _______,     _______, _______, RGB_HUD, RGB_RMOD, RGB_HUI
),
};
// clang-format on