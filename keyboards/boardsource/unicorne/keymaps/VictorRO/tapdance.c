static void sentence_end(tap_dance_state_t *state, void *user_data) {
  switch (state->count) {
    // Double tapping TD_SPC produces
    // ". <one-shot-shift>" i.e. dot, space and capitalize next letter.
    // This helps to quickly end a sentence and begin another one
    // without having to hit shift.
    case 2:
      /* Check that Shift is inactive */
      if (!(get_mods() & MOD_MASK_SHIFT)) {
        // remove the added space of the single tap case
        tap_code(KC_BSPC);
        // replace the space with a single dot
        tap_code(KC_DOT);
        // tap a space
        tap_code(KC_SPC);
        /* Internal code of OSM(MOD_LSFT) */
        add_oneshot_mods(MOD_BIT(KC_LEFT_SHIFT));

      } else {
        // send ">" (KC_DOT + shift → ">")
        tap_code(KC_SPC);
      }
      break;

    // send KC_SPC on every normal tap of TD_SPC
    default:
      tap_code(KC_SPC);
  }
};

void sentence_end_finished (tap_dance_state_t *state, void *user_data) {
  last_keycode = KC_SPC;
}

tap_dance_action_t tap_dance_actions[] = {
  [SPC_TD] = ACTION_TAP_DANCE_FN_ADVANCED(sentence_end, sentence_end_finished, NULL),
};

