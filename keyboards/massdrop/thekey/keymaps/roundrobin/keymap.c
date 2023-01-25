/* Copyright 2021 Joe Maples <joe@maples.dev>
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

#include QMK_KEYBOARD_H

enum _layers {
    _LAYER0 = 0,
    _LAYER1,
    _LAYER2,
    _SWITCHER
};

enum _customkeycodes {
  KC_LAYER_PREV = SAFE_RANGE,
  KC_LAYER_NEXT,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_LAYER0] = LAYOUT(MO(_SWITCHER), KC_VOLD, KC_VOLU),
    [_LAYER1] = LAYOUT(MO(_SWITCHER), KC_MUTE, KC_MPLY),
    [_LAYER2] = LAYOUT(MO(_SWITCHER), KC_MPRV, KC_MNXT),
    [_SWITCHER] = LAYOUT(KC_TRNS, KC_LAYER_PREV, KC_LAYER_NEXT),
};

void keyboard_post_init_user(void) {
    layer_clear();
    layer_on(_LAYER0);

    rgblight_enable_noeeprom(); // Enables RGB, without saving settings
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    size_t max_layer = _SWITCHER;
    uint8_t current_layer = 0;

    while(!layer_state_is(current_layer) && current_layer < max_layer)
        ++current_layer;

    if (record->event.pressed) {
        switch (keycode) {
            case KC_LAYER_PREV: if (current_layer == 0) { layer_on(max_layer - 1); } else { layer_on(current_layer - 1); } layer_off(current_layer); return false;
            case KC_LAYER_NEXT: if (current_layer == max_layer - 1) { layer_on(0); } else { layer_on(current_layer + 1); } layer_off(current_layer); return false;
        }
    }

    return true; // Resume normal keycode processing
}

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
        case _LAYER0: rgblight_sethsv_noeeprom(HSV_PINK); break;
        case _LAYER1: rgblight_sethsv_noeeprom(HSV_PURPLE); break;
        case _LAYER2: rgblight_sethsv_noeeprom(HSV_CYAN); break;
        case _SWITCHER: rgblight_sethsv_noeeprom(HSV_BLACK); break;
    }

    return state;
}
