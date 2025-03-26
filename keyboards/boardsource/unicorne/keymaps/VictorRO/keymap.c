/*
copyright 2024 jack (@waffle87)
spdx-license-identifier: gpl-2.0-or-later
*/

#include QMK_KEYBOARD_H

#include "keymap_spanish.h"

#include "keymap.h"
#include "oneshot.h"
#include "swapper.h"

uint16_t last_keycode;

#define LA_SYM MO(SYM)
#define LA_NAV MO(NAV)

#define MY_TILD RALT(ES_NTIL)

#ifdef TAP_DANCE_ENABLE
enum {
  SPC_TD,
};

#define TD_SPC TD(SPC_TD)
#else
#define TD_SPC KC_SPC
#endif

enum layers {
  BAS,
  SYM,
  NAV,
  NUM,
};

enum keycodes {
  // Custom oneshot mod implementation with no timers.
  OS_SHFT = SAFE_RANGE,
  OS_CTRL,
  OS_ALT,
  OS_CMD,

  SW_WIN,  // Switch to next window         (cmd-tab)
  SW_LANG, // Switch to next input language (ctl-spc)
};

enum combos {
  JK_CMD
};

const uint16_t PROGMEM jk_combo[] = {KC_J, KC_K, COMBO_END};

combo_t key_combos[] = {
  [JK_CMD] = COMBO(jk_combo, OS_CMD),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [BAS] = LAYOUT_split_3x6_3(
    KC_ESC,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,         KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_BSPC,
    KC_TAB,   KC_A,     KC_S,     KC_D,     KC_F,     KC_G,         KC_H,     KC_J,     KC_K,     KC_L,     ES_QUOT,  KC_ENT,
    ES_CCED,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,         KC_N,     KC_M,     KC_COMM,  KC_DOT,   ES_SLSH,  ES_ACUT,
                                  KC_LCTL,  LA_NAV,   KC_LSFT,      TD_SPC,   LA_SYM,   KC_DEL
  ),

  [NAV] = LAYOUT_split_3x6_3(
    XXXXXXX,  XXXXXXX,  SW_WIN,   KC_VOLD,  KC_VOLU,  KC_MUTE,      KC_HOME,  KC_PGDN,  KC_PGUP,  KC_END,   XXXXXXX,  XXXXXXX,
    XXXXXXX,  OS_CTRL,  OS_ALT,   OS_CMD,   OS_SHFT,  XXXXXXX,      KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,  XXXXXXX,  XXXXXXX,
    XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_MPRV,  KC_MPLY,  KC_MNXT,      QK_REP,   XXXXXXX,  XXXXXXX,  XXXXXXX,  SW_LANG,  XXXXXXX,
                                  _______,  _______,  _______,      _______,  _______,  _______
  ),

  [SYM] = LAYOUT_split_3x6_3(
    XXXXXXX,  KC_EXLM,  ES_AT,    ES_HASH,  KC_DLR,   KC_PERC,      ES_DQUO,  ES_AMPR,  ES_ASTR,  ES_UNDS,  ES_SCLN,  ES_NOT,
    XXXXXXX,  ES_GRV,   MY_TILD,  ES_LCBR,  ES_LPRN,  ES_LBRC,      ES_COLN,  OS_SHFT,  OS_CMD,   OS_ALT,   OS_CTRL,  ES_BULT,
    XXXXXXX,  ES_LABK,  ES_RABK,  ES_RCBR,  ES_RPRN,  ES_RBRC,      ES_PIPE,  ES_MINS,  ES_EQL,   ES_PLUS,  ES_BSLS,  ES_NTIL,
                                  _______,  _______,  _______,      _______,  _______,  _______
  ),

  [NUM] = LAYOUT_split_3x6_3(
    QK_BOOT,  RM_VALU,  RM_HUEU,  RM_SATU,  RM_NEXT,  RM_TOGG,      XXXXXXX,  KC_7,     KC_8,     KC_9,    XXXXXXX,  XXXXXXX,
    XXXXXXX,  OS_CTRL,  OS_ALT,   OS_CMD,   OS_SHFT,  XXXXXXX,      XXXXXXX,  KC_4,     KC_5,     KC_6,    XXXXXXX,  XXXXXXX,
    EE_CLR,   RM_VALD,  RM_HUED,  RM_SATD,  RM_PREV,  CK_TOGG,      KC_0,     KC_1,     KC_2,     KC_3,    XXXXXXX,  XXXXXXX,
                                  _______,  _______,  _______,      _______,  _______,  _______
    )
};

bool is_oneshot_cancel_key(uint16_t keycode) {
  switch (keycode) {
    case LA_SYM:
    case LA_NAV:
      return true;
    default:
      return false;
  }
}

bool is_oneshot_ignored_key(uint16_t keycode) {
  switch (keycode) {
    case LA_SYM:
    case LA_NAV:
    case KC_LSFT:
    case OS_SHFT:
    case OS_CTRL:
    case OS_ALT:
    case OS_CMD:
      return true;
    default:
      return false;
  }
}

bool sw_win_active = false;
bool sw_lang_active = false;

oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_cmd_state = os_up_unqueued;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  update_swapper(
    &sw_win_active, KC_LGUI, KC_TAB, SW_WIN,
    keycode, record
  );
  update_swapper(
    &sw_lang_active, KC_LCTL, KC_SPC, SW_LANG,
    keycode, record
  );

  update_oneshot(
    &os_shft_state, KC_LSFT, OS_SHFT,
    keycode, record
  );
  update_oneshot(
    &os_ctrl_state, KC_LCTL, OS_CTRL,
    keycode, record
  );
  update_oneshot(
    &os_alt_state, KC_LALT, OS_ALT,
    keycode, record
  );
  update_oneshot(
    &os_cmd_state, KC_LCMD, OS_CMD,
    keycode, record
  );

  return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
  return update_tri_layer_state(state, SYM, NAV, NUM);
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case LCTL_T(KC_A):
      return TAPPING_TERM + 50;
    default:
      return TAPPING_TERM;
  }
}

#ifdef TAP_DANCE_ENABLE
  // Due to keymap introspection, the old `SRC += tapdance.c` technique in rules.mk no longer works
  #include "tapdance.c"
#endif
#include "swapper.c"
#include "oneshot.c"
