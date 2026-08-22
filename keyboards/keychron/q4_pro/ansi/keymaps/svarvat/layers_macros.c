extern bool process_record_kb_bt(uint16_t keycode, keyrecord_t *record);

//bool isDeadKeyCircStarted = false;
//bool isDeadKeyTremaStarted = false;
bool editModeLThumbStrongStarted = false;
bool isCtlTabStarted = false;
bool isAltTabStarted = false;
bool isSftTabStarted = false;
bool isCapswordStarted = false;
bool isScrollLeftOn = false;
bool isScrollRightOn = false;
bool isScrollUpOn = false;
bool isScrollDownOn = false;
bool isFloodUpOn = false;
bool isFloodDownOn = false;
bool isFloodLeftOn = false;
bool isFloodRightOn = false;
bool isFloodDelOn = false;
bool isFloodEntOn = false;
bool isFloodBspcOn = false;
bool isFloodTabOn = false;
bool isMuteKeysEnabled = false;
bool isWeakLaMouseStarted = false;
bool isMouseBtn1HoldStarted = false;
bool isLThumbMoPristine = true;
bool isLThumbWeakPristine = true;
bool isSftTabPristine = true;
uint16_t inMemoryPreviousWeakLayer = 0;
uint16_t inMemoryCurrentWeakLayer = 0;
static uint16_t bt_history[2] = {BT_HST5, BT_HST6};
bool bt_wake_pending = false;
uint32_t bt_wake_timer = 0;

typedef union {
    uint32_t raw;
    struct {
        uint16_t bt_history_0;
        uint16_t bt_history_1;
    };
} user_config_t;
void eeconfig_init_user(void) {
    user_config_t user_config;
    user_config.bt_history_0 = BT_HST5;
    user_config.bt_history_1 = BT_HST6;
    eeconfig_update_user(user_config.raw);
}
void keyboard_post_init_user(void) {
    user_config_t user_config;
    user_config.raw = eeconfig_read_user();
    if (user_config.bt_history_0 >= BT_HST1 && user_config.bt_history_0 <= BT_HST6) {
        bt_history[0] = user_config.bt_history_0;
    }
    if (user_config.bt_history_1 >= BT_HST1 && user_config.bt_history_1 <= BT_HST6) {
        bt_history[1] = user_config.bt_history_1;
    }
}

void layer_on_lmouse(void) {
    layer_on(LA_MOUSE);
    tap_code16(KC_SCROLL_LOCK);
}
void layer_off_lmouse(void) {
//  set_auto_mouse_enable(false);
    layer_off(LA_MOUSE);
    tap_code16(KC_SCROLL_LOCK);
    isScrollLeftOn = false;
    isScrollUpOn = false;
    isScrollDownOn = false;
    isScrollRightOn = false;
    isFloodDownOn = false;
    isFloodUpOn = false;
    isFloodRightOn = false;
    isFloodLeftOn = false;
    if (isMouseBtn1HoldStarted) {
        unregister_code16(KC_MS_BTN1);
        isMouseBtn1HoldStarted = false;
    }
}
void layer_off_mo_layer(uint16_t layer) {
    isLThumbMoPristine = true;
    layer_off(layer);
}
void layer_on_weak_layer(uint16_t layer) {
    inMemoryCurrentWeakLayer = layer;
    layer_on(layer);
}
void layer_off_weak_layer(uint16_t layer) {
    inMemoryPreviousWeakLayer = layer;
    isLThumbWeakPristine = true;
    layer_off(layer);
}
void reverse_weak_layer(void) {
    if (isLThumbMoPristine) {
        if (inMemoryPreviousWeakLayer != 0) {
            layer_off(inMemoryCurrentWeakLayer);
            layer_on(inMemoryPreviousWeakLayer);
        }
        isLThumbMoPristine = false;
        isLThumbWeakPristine = false;
    }
}
void register_bt_host(uint16_t kc) {
    if (kc == bt_history[0]) return;
    bt_history[1] = bt_history[0];
    bt_history[0] = kc;
    user_config_t user_config;
    user_config.bt_history_0 = bt_history[0];
    user_config.bt_history_1 = bt_history[1];
    eeconfig_update_user(user_config.raw);
    bt_wake_pending = true;
    bt_wake_timer = timer_read32();
}
void switch_to_previous_bt(keyrecord_t* record) {
    if (record->event.pressed) {
        uint16_t prev_kc_bt_hst = bt_history[1];
        register_bt_host(prev_kc_bt_hst);
        keyrecord_t fake_record = {0};
        fake_record.event.pressed = true;
        process_record_kb_bt(prev_kc_bt_hst, &fake_record);
        fake_record.event.pressed = false;
        process_record_kb_bt(prev_kc_bt_hst, &fake_record);
    }
}


bool processKeycodeIfAltTab(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case KC_TAB:
        case KC_RIGHT:
        case KC_DOWN:
        case KC_UP:
        case KC_LEFT:
        case C(KC_Z):
        case MA_LTHUMBE:
        case MA_LTHUMBD:
        case KC_LSFT:
            return true;
        case KC_DEL:
            if (record->event.pressed) {
                register_code16(C(KC_Z));
            }
            return false;
        default:
            isAltTabStarted = false;
            unregister_code16(KC_LALT);
            unregister_mods(MOD_MASK_SHIFT);
            if (keycode == KC_ENT) {
                return false;
            }
            return true;
    }
}
bool processKeycodeIfSftTab(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case KC_TAB:
        case KC_RIGHT:
        case KC_DOWN:
        case MA_MS_RIGHT:
        case MA_MS_DOWN:
            if (record->event.pressed) {
                if (isSftTabPristine) isSftTabPristine = false;
                unregister_code16(KC_LSFT);
                if (IS_LAYER_OFF(LA_LTHUMBEMO) && IS_LAYER_OFF(LA_LTHUMBDMO)) tap_code16(KC_TAB);
            } else {
                register_code16(KC_LSFT);
            }
            return false;
        case KC_LEFT:
        case KC_UP:
        case MA_MS_LEFT:
        case MA_MS_UP:
            if (record->event.pressed) {
                if (isSftTabPristine) isSftTabPristine = false;
                register_code16(KC_LSFT);
                if (IS_LAYER_OFF(LA_LTHUMBEMO) && IS_LAYER_OFF(LA_LTHUMBDMO)) tap_code16(KC_TAB);
            } else {
                unregister_code16(KC_LSFT);
            }
            return false;
        case MA_LTHUMBE:
        case MA_LTHUMBD:
            return false;
        default:
            isSftTabStarted = false;
            isSftTabPristine = true;
            unregister_code16(KC_LSFT);
            return true;
    }
}
bool processKeycodeIfCtlTab(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case KC_TAB:
        case KC_DOWN:
        case KC_RIGHT:
        case MA_MS_DOWN:
        case MA_MS_RIGHT:
            if (record->event.pressed) {
                if (IS_LAYER_OFF(LA_LTHUMBEMO) && IS_LAYER_OFF(LA_LTHUMBDMO)) tap_code16(KC_TAB);
            }
            return true;
        case KC_UP:
        case KC_LEFT:
        case MA_MS_UP:
        case MA_MS_LEFT:
            if (record->event.pressed) {
                register_code16(KC_LSFT);
                if (IS_LAYER_OFF(LA_LTHUMBEMO) && IS_LAYER_OFF(LA_LTHUMBDMO)) tap_code16(KC_TAB);
            } else {
                unregister_code16(KC_LSFT);
            }
            return true;
        case KC_HOME:
        case KC_MS_BTN4:
            if (record->event.pressed) {
                register_code16(KC_LSFT);
                tap_code16(KC_PGUP);
                unregister_code16(KC_LSFT);
            }
            return false;
        case KC_END:
        case KC_MS_BTN5:
            if (record->event.pressed) {
                register_code16(KC_LSFT);
                tap_code16(KC_PGDN);
                unregister_code16(KC_LSFT);
            }
            return false;
        case C(KC_Z):
        case MA_LTHUMBD:
        case MA_LTHUMBE:
        case KC_LSFT:
            return true;
        case KC_DEL:
            if (record->event.pressed) {
                register_code16(C(KC_Z));
            }
            return false;
        default:
            isCtlTabStarted = false;
            unregister_mods(MOD_MASK_CTRL);
            unregister_mods(MOD_MASK_SHIFT);
            layer_off_mo_layer(LA_LTHUMBDMO);
            layer_off_mo_layer(LA_LTHUMBEMO);
            if (keycode == KC_ENT) {
                return false;
            }
            return true;
    }
}
bool processKeycodeIfMuteKeysEnabled(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case MA_MUTE_KEYS:
            if (record->event.pressed && IS_LAYER_ON(LA_LPINKY)) {
                isMuteKeysEnabled = false;
            }
            return false;
        case MA_LPINKY:
            return true ;
        default:
            return false;
    }
}
bool processKeycodeIfLBase(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case MA_LTHUMB:
            if (record->event.pressed) {
                layer_on(LA_LTHUMB);
            }
            return false;
        case MA_LPINKY:
            if (record->event.pressed) {
                layer_on(LA_LPINKY);
            }
            return false;
        case MA_LMOUSE:
            if (record->event.pressed) {
                if ((get_mods() & MOD_BIT(KC_RSFT)) == MOD_BIT(KC_RSFT)
                && (get_mods() & MOD_BIT(KC_RCTL)) == MOD_BIT(KC_RCTL)
                && (get_mods() & MOD_BIT(KC_RALT)) == MOD_BIT(KC_RALT)) {
                    reset_keyboard();
                } else {
//                    set_auto_mouse_enable(true);
                    layer_on_lmouse();
                }
            }
            return false;
        case MA_CAPSLOCK:
            if (record->event.pressed) {
                layer_on(LA_CAPSLOCK);
            }
            return false;
        case KC_LSFT:
            if ((get_mods() & MOD_BIT(KC_RSFT)) == MOD_BIT(KC_RSFT) && record->event.pressed) {
                isCapswordStarted = true;
                layer_on(LA_CAPSLOCK);
                return false;
            }
            return true;
        case KC_RSFT:
            if (record->event.pressed) {
                if ((get_mods() & MOD_BIT(KC_LSFT)) == MOD_BIT(KC_LSFT)) {
                    isCapswordStarted = true;
                    layer_on(LA_CAPSLOCK);
                    return false;
                }
            }
            return true;
        case KC_TAB:
            if (record->event.pressed) {
                if ((get_mods() & MOD_BIT(KC_LALT)) == MOD_BIT(KC_LALT) && !isAltTabStarted && !isSftTabStarted && !isCtlTabStarted) {
                    isAltTabStarted = true;
                    layer_on(LA_LTHUMB);
                } else if ((get_mods() & MOD_BIT(KC_LSFT)) == MOD_BIT(KC_LSFT) && !isSftTabStarted && !isAltTabStarted && !isCtlTabStarted) {
                    isSftTabStarted = true;
                    layer_on(LA_LTHUMB);
                }
            }
            return true;
        case KC_SLSH:
            if (record->event.pressed) {
                if ((get_mods() & MOD_BIT(KC_LSFT)) == MOD_BIT(KC_LSFT)) {
                    unregister_code16(KC_LSFT);
                    tap_code16(ALGR(KC_8));
                    register_code16(KC_LSFT);
                    return false;
                }
            }
            return true;
        case MA_REBOOT:
            clear_keyboard();
            soft_reset_keyboard();
            return false;
        default:
            return true;
    }
}
bool processKeycodeIfLCapslock(uint16_t keycode, keyrecord_t* record) {
    if (isCapswordStarted &&
    (keycode == KC_SPC
    || keycode == KC_UP
    || keycode == KC_DOWN
    || keycode == KC_HOME
    || keycode == KC_END
    || keycode == KC_ENT
    || keycode == KC_RSFT
    || keycode == KC_LSFT)) {
        if (record->event.pressed) {
            isCapswordStarted = false;
            layer_off(LA_CAPSLOCK);
        }
    }
    switch (keycode) {
        case MA_CAPSLOCK:
        case MA_CAPSWORD:
            if (record->event.pressed) {
                isCapswordStarted = false;
                layer_off(LA_CAPSLOCK);
            }
            return false;
//        case MA_CIRC:
//            if (record->event.pressed) {
//                if (!(isDeadKeyTremaStarted) && get_mods() && MOD_MASK_SHIFT) {isDeadKeyTremaStarted=true;}
//                else if (!isDeadKeyCircStarted) {isDeadKeyCircStarted=true;}
//            }
//            return false;
//        case MA_CAPSE:
//            if (record->event.pressed) {
//                if (isDeadKeyTremaStarted) {
//                    tap_code16(X(ETREMA));
//                    isDeadKeyTremaStarted=false;
//                } else if (isDeadKeyCircStarted) {
//                    register_code16(KC_RALT);
//                    tap_code16(S(X(ECIRC)));
//                    unregister_code16(KC_RALT);
//                    isDeadKeyCircStarted=false;
//                } else {
//                    register_code16(KC_RALT);
//                    tap_code16(X(ECIRC));
////                    tap_code16(S(KC_E));
//                    unregister_code16(KC_RALT);
//                }
//            }
//            return false;
//        case MA_CAPSA:
//            if (record->event.pressed) {
//                if (isDeadKeyTremaStarted) {
//                    tap_code16(X(ATREMA));
//                    isDeadKeyTremaStarted=false;
//                }
//                else if (isDeadKeyCircStarted) {
//                    tap_code16(X(ACIRC));
//                    isDeadKeyCircStarted=false;
//                }
//                else {tap_code16(S(KC_Q));}
//            }
//            return false;
//        case MA_CAPSI:
//            if (record->event.pressed) {
//                if (isDeadKeyTremaStarted) {
//                    tap_code16(X(ITREMA));
//                    isDeadKeyTremaStarted=false;
//                }
//                else if (isDeadKeyCircStarted) {
//                    tap_code16(X(ICIRC));
//                    isDeadKeyCircStarted=false;
//                }
//                else {tap_code16(S(KC_I));}
//            }
//            return false;
//        case MA_CAPSU:
//            if (record->event.pressed) {
//                if (isDeadKeyTremaStarted) {
//                    tap_code16(X(UTREMA));
//                    isDeadKeyTremaStarted=false;
//                }
//                else if (isDeadKeyCircStarted) {
//                    tap_code16(X(UCIRC));
//                    isDeadKeyCircStarted=false;
//                }
//                else {tap_code16(S(KC_U));}
//            }
//            return false;
//    }
//    if (isDeadKeyCircStarted) {isDeadKeyCircStarted=false;}
//    if (isDeadKeyTremaStarted) {isDeadKeyTremaStarted=false;}
        default:
            return true;
    }
}
bool processKeycodeIfLMouse(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case MA_LMOUSE:
            if (record->event.pressed) {
                layer_off_lmouse();
            }
            return false;
        case MA_LTHUMBMS:
            if (record->event.pressed) {
                layer_on(LA_LTHUMBMS);
            } else {
                layer_off(LA_LTHUMBMS);
            }
            return false;
        case MA_MS_BTN1_TAP:
            if (record->event.pressed) {
                if (isMouseBtn1HoldStarted) {
                    unregister_code16(KC_MS_BTN1);
                    isMouseBtn1HoldStarted = false;
                }
                tap_code16(KC_MS_BTN1);
            }
            return false;
        case MA_MS_BTN2_TAP:
            if (record->event.pressed) {
                tap_code16(KC_MS_BTN2);
            }
            return false;
        case MA_MS_WH_DOWN:
            if (record->event.pressed) {
                isScrollDownOn = true;
            } else {
                isScrollDownOn = false;
            }
            return false;
        case MA_MS_WH_UP:
            if (record->event.pressed) {
                isScrollUpOn = true;
            } else {
                isScrollUpOn = false;
            }
            return false;
        case MA_MS_DOWN:
            if (record->event.pressed) {
                isFloodDownOn = true;
            } else {
                isFloodDownOn = false;
            }
            return false;
        case MA_MS_UP:
            if (record->event.pressed) {
                isFloodUpOn = true;
            } else {
                isFloodUpOn = false;
            }
            return false;
        case MA_MS_LEFT:
            if (record->event.pressed) {
                isFloodLeftOn = true;
            } else {
                isFloodLeftOn = false;
            }
            return false;
        case MA_MS_RIGHT:
            if (record->event.pressed) {
                isFloodRightOn = true;
            } else {
                isFloodRightOn = false;
            }
            return false;
        case MA_MS_BTN1_HOLD:
            if (record->event.pressed) {
                if (isMouseBtn1HoldStarted) {
                    unregister_code16(KC_MS_BTN1);
                    isMouseBtn1HoldStarted = false;
                } else {
                    register_code16(KC_MS_BTN1);
                    isMouseBtn1HoldStarted = true;
                }
            }
            return false;
        default:
            return true;
    }
}
bool processKeycodeIfLPinky(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case MA_LPINKY:
            if (!(record->event.pressed)) {
                layer_off(LA_LPINKY);
            }
            return false;
        case MA_DELLINE:
            if (record->event.pressed) {
                tap_code16(KC_HOME);
                tap_code16(KC_HOME);
                register_code16(KC_LSFT);
                tap_code16(KC_END);
                tap_code16(KC_RGHT);
                unregister_code16(KC_LSFT);
                tap_code16(KC_DEL);
            }
            return false;
        case MA_JUMPTAB:
            if (!(record->event.pressed)) {
                if ((get_mods() & MOD_BIT(KC_LSFT)) == MOD_BIT(KC_LSFT)) {
                    unregister_code16(KC_LSFT);
                    tap_code16(KC_UP);
                    tap_code16(KC_HOME);
                    tap_code16(KC_HOME);
                    tap_code16(KC_DEL);
                    register_code16(KC_LSFT);
                } else {
                    tap_code16(KC_HOME);
                    tap_code16(KC_HOME);
                    tap_code16(KC_TAB);
                    tap_code16(KC_DOWN);
                }
            }
            return false;
        case MA_MUTE_KEYS:
            if (record->event.pressed) {
                isMuteKeysEnabled = true;
                layer_off(LA_LPINKY);
            }
            return false;
        case KC_UP:
            if ((get_mods() & MOD_BIT(KC_LSFT)) == MOD_BIT(KC_LSFT) && IS_LAYER_OFF(LA_LTHUMB) && record->event.pressed) {
                unregister_code16(KC_LSFT);
                tap_code16(KC_DOWN);
                register_code16(KC_LSFT);
                return false;
            }
            return true;
        case FR_EQL:
            if ((get_mods() & MOD_BIT(KC_LSFT)) == MOD_BIT(KC_LSFT) && record->event.pressed) {
                unregister_code16(KC_LSFT);
                tap_code16(FR_EQL);
                tap_code16(FR_RABK);
                register_code16(KC_LSFT);
                return false;
            }
            return true;
        case FR_MINS:
            if ((get_mods() & MOD_BIT(KC_LSFT)) == MOD_BIT(KC_LSFT) && record->event.pressed) {
                unregister_code16(KC_LSFT);
                tap_code16(FR_MINS);
                tap_code16(FR_RABK);
                register_code16(KC_LSFT);
                return false;
            }
            return true;
        default:
            return true;
    }
}
bool processKeycodeIfRThumb(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case MA_BACKTICK:
            if (record->event.pressed) {
                tap_code16(FR_GRV);
            }
            return false;
        case MA_TILD:
            if (record->event.pressed) {
                tap_code16(FR_TILD);
            }
            return false;
        default:
            return true;
    }
}
bool processKeycodeIfLThumbEStrong(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case MA_LTHUMBE:
            return false;
        case MA_SUPER:
            register_mods(MOD_MASK_CTRL);
            editModeLThumbStrongStarted = true;
            return false;
        case MA_NONE:
            if (editModeLThumbStrongStarted) {
                unregister_mods(MOD_MASK_CTRL);
                editModeLThumbStrongStarted = false;
            }
            layer_off(LA_LTHUMBESTRONG);
            return false;
        case KC_LALT:
        case KC_LSFT:
        case KC_LCTL:
        case KC_LGUI:
        case KC_RALT:
        case KC_RSFT:
        case KC_RCTL:
        case KC_RGUI:
        case MA_LPINKY:
        case MA_LTHUMB:
        case MO(LA_RTHUMB):
            if (editModeLThumbStrongStarted) {
                unregister_mods(MOD_MASK_CTRL);
                editModeLThumbStrongStarted = false;
            }
            layer_off(LA_LTHUMBESTRONG);
            return true;
        default:
            if (!(record->event.pressed)) {
                if (editModeLThumbStrongStarted) {
                    unregister_mods(MOD_MASK_CTRL);
                    editModeLThumbStrongStarted = false;
                }
                layer_off(LA_LTHUMBESTRONG);
            }
            return true;
    }
}
bool processKeycodeIfLThumbDStrong(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case MA_LTHUMBD:
            return false;
        case MA_SUPER:
            register_mods(MOD_MASK_CTRL);
            editModeLThumbStrongStarted = true;
            return false;
        case MA_NONE:
            if (editModeLThumbStrongStarted) {
                unregister_mods(MOD_MASK_CTRL);
                editModeLThumbStrongStarted = false;
            }
            layer_off(LA_LTHUMBDSTRONG);
            return false;
        case KC_LALT:
        case KC_LSFT:
        case KC_LCTL:
        case KC_LGUI:
        case KC_RALT:
        case KC_RSFT:
        case KC_RCTL:
        case KC_RGUI:
        case MA_LPINKY:
        case MA_LTHUMB:
        case MO(LA_RTHUMB):
            if (editModeLThumbStrongStarted) {
                unregister_mods(MOD_MASK_CTRL);
                editModeLThumbStrongStarted = false;
            }
            layer_off(LA_LTHUMBDSTRONG);
            return true;
        default:
            if (!(record->event.pressed)) {
                if (editModeLThumbStrongStarted) {
                    unregister_mods(MOD_MASK_CTRL);
                    editModeLThumbStrongStarted = false;
                }
                layer_off(LA_LTHUMBDSTRONG);
            }
            return true;
    }
}
bool processKeycodeIfLThumb(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case MA_LTHUMB:
            if (!(record->event.pressed)) {
                layer_off(LA_LTHUMB);
                isFloodUpOn = false;
                isFloodDownOn = false;
                isFloodLeftOn = false;
                isFloodRightOn = false;
                isFloodDelOn = false;
                isFloodEntOn = false;
                isFloodBspcOn = false;
                isFloodTabOn = false;
            }
            return false;
        case MA_LTHUMBD:
            if (record->event.pressed) {
                if (!isAltTabStarted && !isSftTabStarted && !isCtlTabStarted) {
                    layer_on_weak_layer(LA_LTHUMBDWEAK);
                }
                layer_on(LA_LTHUMBDMO);
            }
            return false;
        case MA_LTHUMBE:
            if (record->event.pressed) {
                if (!isAltTabStarted && !isSftTabStarted && !isCtlTabStarted) {
                    layer_on_weak_layer(LA_LTHUMBEWEAK);
                }
                layer_on(LA_LTHUMBEMO);
            }
            return false;
        case MA_LTHUMB1:
            if (record->event.pressed) {
                layer_on_weak_layer(LA_LTHUMB1WEAK);
            }
            return false;
        case MA_LTHUMB2:
            if (record->event.pressed) {
                layer_on_weak_layer(LA_LTHUMB2WEAK);
            }
            return false;
        case MA_LTHUMB3:
            if (record->event.pressed) {
                layer_on_weak_layer(LA_LTHUMB3WEAK);
            }
            return false;
        case MA_LMOUSE:
            if (record->event.pressed) {
                layer_off(LA_LTHUMB);
                layer_on(LA_LTHUMBMS);
                layer_on_lmouse();
                isWeakLaMouseStarted = true;
            }
            return false;
        case KC_UP:
            if (record->event.pressed) {
                isFloodUpOn = true;
            } else {
                isFloodUpOn = false;
            }
            return IS_LAYER_OFF(LA_LTHUMBEMO) && IS_LAYER_OFF(LA_LTHUMBDMO);
        case KC_DOWN:
            if (record->event.pressed) {
                isFloodDownOn = true;
            } else {
                isFloodDownOn = false;
            }
            return IS_LAYER_OFF(LA_LTHUMBEMO) && IS_LAYER_OFF(LA_LTHUMBDMO);
        case KC_LEFT:
            if (record->event.pressed) {
                if (IS_LAYER_ON(LA_LPINKY)) {
                    register_code16(KC_LCTL);
                }
                isFloodLeftOn = true;
            } else {
                isFloodLeftOn = false;
                if (IS_LAYER_ON(LA_LPINKY)) {
                    unregister_code16(KC_LCTL);
                }
            }
            return IS_LAYER_OFF(LA_LTHUMBEMO) && IS_LAYER_OFF(LA_LTHUMBDMO);
        case KC_RGHT:
            if (record->event.pressed) {
                if (IS_LAYER_ON(LA_LPINKY)) {
                    register_code16(KC_LCTL);
                }
                isFloodRightOn = true;
            } else {
                isFloodRightOn = false;
                if (IS_LAYER_ON(LA_LPINKY)) {
                    unregister_code16(KC_LCTL);
                }
            }
            return IS_LAYER_OFF(LA_LTHUMBEMO) && IS_LAYER_OFF(LA_LTHUMBDMO);
        case KC_PGUP:
            if (IS_LAYER_ON(LA_LPINKY) && record->event.pressed) {
                register_code16(KC_LCTL);
                tap_code16(KC_HOME);
                unregister_code16(KC_LCTL);
                return false;
            }
            return true;
        case KC_PGDN:
            if (IS_LAYER_ON(LA_LPINKY) && record->event.pressed) {
                register_code16(KC_LCTL);
                tap_code16(KC_END);
                unregister_code16(KC_LCTL);
                return false;
            }
            return true;
        case KC_BSPC:
            if (record->event.pressed) {
                if (IS_LAYER_ON(LA_LPINKY)) {
                    register_code16(KC_LCTL);
                    isFloodBspcOn = true;
                } else {
                    isFloodBspcOn = true;
                }
            } else {
                if (IS_LAYER_ON(LA_LPINKY)) {
                    isFloodBspcOn = false;
                    unregister_code16(KC_LCTL);
                } else {
                    isFloodBspcOn = false;
                }
            }
            return IS_LAYER_OFF(LA_LTHUMBEMO) && IS_LAYER_OFF(LA_LTHUMBDMO);
        case KC_DEL:
            if (record->event.pressed) {
                if (IS_LAYER_ON(LA_LPINKY)) {
                    register_code16(KC_LCTL);
                    isFloodDelOn = true;
                } else {
                    isFloodDelOn = true;
                }
            } else {
                if (IS_LAYER_ON(LA_LPINKY)) {
                    isFloodDelOn = false;
                    unregister_code16(KC_LCTL);
                } else {
                    isFloodDelOn = false;
                }
            }
            return IS_LAYER_OFF(LA_LTHUMBEMO) && IS_LAYER_OFF(LA_LTHUMBDMO);
        case KC_TAB:
            if (!isCtlTabStarted && !isAltTabStarted && !isSftTabStarted && record->event.pressed) {
                isCtlTabStarted = true;
                register_mods(MOD_MASK_CTRL);
                tap_code16(KC_TAB);
                return false;
            }
            return true;
        default:
            return true;
    }
}
bool processKeycodeIfLThumbMs(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case MA_LTHUMBMS:
            if (!(record->event.pressed)) {
                isWeakLaMouseStarted = false;
                layer_off(LA_LTHUMBMS);
                isScrollLeftOn = false;
                isScrollUpOn = false;
                isScrollDownOn = false;
                isScrollRightOn = false;
                isFloodUpOn = false;
                isFloodDownOn = false;
                isFloodLeftOn = false;
                isFloodRightOn = false;
                isFloodDelOn = false;
                isFloodEntOn = false;
                isFloodBspcOn = false;
                isFloodTabOn = false;
            }
            return false;
        case MA_LTHUMB:
            if (!(record->event.pressed)) {
                isWeakLaMouseStarted = false;
                layer_off(LA_LTHUMBMS);
                layer_off_lmouse();
                isScrollLeftOn = false;
                isScrollUpOn = false;
                isScrollDownOn = false;
                isScrollRightOn = false;
                isFloodUpOn = false;
                isFloodDownOn = false;
                isFloodLeftOn = false;
                isFloodRightOn = false;
                isFloodDelOn = false;
                isFloodEntOn = false;
                isFloodBspcOn = false;
                isFloodTabOn = false;
            }
            return false;
        case MA_LTHUMBD:
            if (record->event.pressed) {
                if (!isWeakLaMouseStarted) {
                    layer_on_weak_layer(LA_LTHUMBDWEAK);
                }
                layer_on(LA_LTHUMBDMO);
            }
            return false;
        case MA_LTHUMBE:
            if (record->event.pressed) {
                if (!isWeakLaMouseStarted) {
                    layer_on_weak_layer(LA_LTHUMBEWEAK);
                }
                layer_on(LA_LTHUMBEMO);
            }
            return false;
        case MA_LTHUMB1:
            if (record->event.pressed) {
                layer_on_weak_layer(LA_LTHUMB1WEAK);
            }
            return false;
        case MA_LTHUMB2:
            if (record->event.pressed) {
                layer_on_weak_layer(LA_LTHUMB2WEAK);
            }
            return false;
        case MA_LTHUMB3:
            if (record->event.pressed) {
                layer_on_weak_layer(LA_LTHUMB3WEAK);
            }
            return false;
        case KC_MS_BTN1:
            if (isWeakLaMouseStarted) {
                return true;
            } else {
                if (record->event.pressed) {
                    tap_code16(KC_AUDIO_VOL_UP);
                }
            }
            return false;
        case KC_MS_BTN3:
            if (isWeakLaMouseStarted) {
                return true;
            } else {
                if (record->event.pressed) {
                    tap_code16(KC_AUDIO_VOL_DOWN);
                }
            }
            return false;
        case KC_TAB:
            if (!isCtlTabStarted && !isAltTabStarted && !isSftTabStarted && record->event.pressed) {
                isCtlTabStarted = true;
                register_mods(MOD_MASK_CTRL);
                tap_code16(KC_TAB);
                return false;
            }
            return true;
    }
    return true;
}
bool processKeycodeIfLThumb1Weak(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case MA_LTHUMB:
        case MA_LTHUMBMS:
            if (!(record->event.pressed)) {
                layer_off_weak_layer(LA_LTHUMB1WEAK);
                inMemoryPreviousWeakLayer = 0;
            }
            return true;
        case MA_LTHUMBE:
            if (isLThumbWeakPristine) {
                if (record->event.pressed) {
                    isLThumbWeakPristine = false;
                    tap_code16(KC_F5);
                }
                return false;
            } else {
                if (record->event.pressed) {
                    layer_off_weak_layer(LA_LTHUMB1WEAK);
                }
                return true;
            }
        case MA_LTHUMBD:
            if (isLThumbWeakPristine) {
                if (record->event.pressed) {
                    isLThumbWeakPristine = false;
                    tap_code16(C(S(KC_C)));
                }
                return false;
            } else {
                if (record->event.pressed) {
                    layer_off_weak_layer(LA_LTHUMB1WEAK);
                }
                return true;
            }
        case MA_LTHUMB1:
            if (record->event.pressed) {
                if (isLThumbWeakPristine) {
                    isLThumbWeakPristine = false;
                    tap_code16(C(KC_L));
                } else {
                    isLThumbWeakPristine = true;
                }
            }
            return false;
        case MA_LTHUMB2:
            if (isLThumbWeakPristine) {
                if (record->event.pressed) {
                    isLThumbWeakPristine = false;
                    tap_code16(C(KC_T));
                }
                return false;
            } else {
                if (record->event.pressed) {
                    layer_off_weak_layer(LA_LTHUMB1WEAK);
                }
                return true;
            }
        case MA_LTHUMB3:
            if (isLThumbWeakPristine) {
                if (record->event.pressed) {
                    isLThumbWeakPristine = false;
                    tap_code16(C(A(KC_P)));
                }
                return false;
            } else {
                if (record->event.pressed) {
                    layer_off_weak_layer(LA_LTHUMB1WEAK);
                }
                return true;
            }
        default:
            isLThumbWeakPristine = false;
            return true;
    }
}
bool processKeycodeIfLThumb2Weak(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case MA_LTHUMB:
        case MA_LTHUMBMS:
            if (!(record->event.pressed)) {
                layer_off_weak_layer(LA_LTHUMB2WEAK);
                inMemoryPreviousWeakLayer = 0;
            }
            return true;
        case MA_LTHUMBE:
            if (isLThumbWeakPristine) {
                if (record->event.pressed) {
                    isLThumbWeakPristine = false;
                    tap_code16(RCS(KC_Y));
                }
                return false;
            } else {
                if (record->event.pressed) {
                    layer_off_weak_layer(LA_LTHUMB2WEAK);
                }
                return true;
            }
        case MA_LTHUMBD:
            if (isLThumbWeakPristine) {
                if (record->event.pressed) {
                    isLThumbWeakPristine = false;
                    tap_code16(RCS(KC_G));
                }
                return false;
            } else {
                if (record->event.pressed) {
                    layer_off_weak_layer(LA_LTHUMB2WEAK);
                }
                return true;
            }
        case MA_LTHUMB2:
            if (record->event.pressed) {
                if (isLThumbWeakPristine) {
                    isLThumbWeakPristine = false;
                    tap_code16(RCS(KC_O));
                } else {
                    isLThumbWeakPristine = true;
                }
            }
            return false;
        case MA_LTHUMB1:
            if (isLThumbWeakPristine) {
                if (record->event.pressed) {
                    isLThumbWeakPristine = false;
                    tap_code16(RCS(KC_I));
                }
                return false;
            } else {
                if (record->event.pressed) {
                    layer_off_weak_layer(LA_LTHUMB2WEAK);
                }
                return true;
            }
        case MA_LTHUMB3:
            if (isLThumbWeakPristine) {
                if (record->event.pressed) {
                    isLThumbWeakPristine = false;
                    tap_code16(RCS(KC_P));
                }
                return false;
            } else {
                if (record->event.pressed) {
                    layer_off_weak_layer(LA_LTHUMB2WEAK);
                }
                return true;
            }
        default:
            isLThumbWeakPristine = false;
            return true;
    }
}
bool processKeycodeIfLThumb3Weak(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case MA_LTHUMB:
        case MA_LTHUMBMS:
            if (!(record->event.pressed)) {
                layer_off_weak_layer(LA_LTHUMB3WEAK);
                inMemoryPreviousWeakLayer = 0;
            }
            return true;
        case MA_LTHUMBE:
            if (isLThumbWeakPristine) {
                if (record->event.pressed) {
                    isLThumbWeakPristine = false;
                    tap_code16(LALT(KC_Y));
                }
                return false;
            } else {
                if (record->event.pressed) {
                    layer_off_weak_layer(LA_LTHUMB3WEAK);
                }
                return true;
            }
        case MA_LTHUMBD:
            if (isLThumbWeakPristine) {
                if (record->event.pressed) {
                    isLThumbWeakPristine = false;
                    tap_code16(LALT(KC_G));
                }
                return false;
            } else {
                if (record->event.pressed) {
                    layer_off_weak_layer(LA_LTHUMB3WEAK);
                }
                return true;
            }
        case MA_LTHUMB3:
            if (record->event.pressed) {
                if (isLThumbWeakPristine) {
                    isLThumbWeakPristine = false;
                    tap_code16(LALT(KC_P));
                } else {
                    isLThumbWeakPristine = true;
                }
            }
            return false;
        case MA_LTHUMB1:
            if (isLThumbWeakPristine) {
                if (record->event.pressed) {
                    isLThumbWeakPristine = false;
                    tap_code16(LALT(KC_I));
                }
                return false;
            } else {
                if (record->event.pressed) {
                    layer_off_weak_layer(LA_LTHUMB3WEAK);
                }
                return true;
            }
        case MA_LTHUMB2:
            if (isLThumbWeakPristine) {
                if (record->event.pressed) {
                    isLThumbWeakPristine = false;
                    tap_code16(LALT(KC_O));
                }
                return false;
            } else {
                if (record->event.pressed) {
                    layer_off_weak_layer(LA_LTHUMB3WEAK);
                }
                return true;
            }
        default:
            isLThumbWeakPristine = false;
            return true;
    }
}
bool processKeycodeIfLThumbEWeak(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case MA_LTHUMB:
        case MA_LTHUMBMS:
            if (!(record->event.pressed)) {
                layer_off_weak_layer(LA_LTHUMBEWEAK);
                inMemoryPreviousWeakLayer = 0;
            }
            return true;
        case MA_LTHUMBE:
            if (record->event.pressed) {
                if (isLThumbWeakPristine
                && !isFloodUpOn
                && !isFloodDownOn
                && !isFloodLeftOn
                && !isFloodRightOn
                && !isFloodDelOn
                && !isFloodEntOn
                && !isFloodBspcOn
                && !isFloodTabOn
                && !isScrollLeftOn
                && !isScrollRightOn
                && !isScrollUpOn
                && !isScrollDownOn) {
                    tap_code16(C(KC_Q));
                    isLThumbWeakPristine = false;
                    layer_off(LA_LTHUMBESTRONG);
                    return false;
                } else {
                    isLThumbWeakPristine = true;
                }
            }
            return true;
        case MA_LTHUMBD:
        case MA_LTHUMB1:
        case MA_LTHUMB2:
        case MA_LTHUMB3:
            if (record->event.pressed) {
                layer_off_weak_layer(LA_LTHUMBEWEAK);
            }
            return true;
        default:
            isLThumbWeakPristine = false;
            return true;
    }
}
bool processKeycodeIfLThumbDWeak(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case MA_LTHUMB:
        case MA_LTHUMBMS:
            if (!(record->event.pressed)) {
                layer_off_weak_layer(LA_LTHUMBDWEAK);
                inMemoryPreviousWeakLayer = 0;
            }
            return true;
        case MA_LTHUMBD:
            if (record->event.pressed) {
                if (isLThumbWeakPristine
                && !isFloodUpOn
                && !isFloodDownOn
                && !isFloodLeftOn
                && !isFloodRightOn
                && !isFloodDelOn
                && !isFloodEntOn
                && !isFloodBspcOn
                && !isFloodTabOn
                && !isScrollLeftOn
                && !isScrollRightOn
                && !isScrollUpOn
                && !isScrollDownOn) {
                    tap_code16(G(KC_UP));
                    isLThumbWeakPristine = false;
                    layer_off(LA_LTHUMBDSTRONG);
                    return false;
                } else {
                    isLThumbWeakPristine = true;
                }
            }
            return true;
        case MA_LTHUMBE:
        case MA_LTHUMB1:
        case MA_LTHUMB2:
        case MA_LTHUMB3:
            if (record->event.pressed) {
                layer_off_weak_layer(LA_LTHUMBDWEAK);
            }
            return true;
        case MA_WIN_LEFT:
            if (record->event.pressed) {
                register_code16(KC_LGUI);
                register_code16(KC_LSFT);
                tap_code16(KC_LEFT);
                unregister_code16(KC_LSFT);
                unregister_code16(KC_LGUI);
                isLThumbWeakPristine = false;
            }
            return false;
        case MA_WIN_RIGHT:
            if (record->event.pressed) {
                register_code16(KC_LGUI);
                register_code16(KC_LSFT);
                tap_code16(KC_RIGHT);
                unregister_code16(KC_LSFT);
                unregister_code16(KC_LGUI);
                isLThumbWeakPristine = false;
            }
            return false;
        case MA_SW_BT_HST:
            switch_to_previous_bt(record);
            return false;
        case BT_HST1 ... BT_HST6:
            register_bt_host(keycode);
            return true;
        default:
            isLThumbWeakPristine = false;
            return true;
    }
}
bool processKeycodeIfLThumbEMo(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case MA_LTHUMB:
        case MA_LTHUMBMS:
        case MA_LTHUMBE:
            if (!(record->event.pressed)) {
                if (!isWeakLaMouseStarted) {
                    if (isFloodUpOn
                    || isFloodDownOn
                    || isFloodLeftOn
                    || isFloodRightOn
                    || isFloodDelOn
                    || isFloodEntOn
                    || isFloodBspcOn
                    || isFloodTabOn
                    || isScrollLeftOn
                    || isScrollRightOn
                    || isScrollUpOn
                    || isScrollDownOn) {
                        reverse_weak_layer();
                    } else {
                        if (isLThumbMoPristine && isLThumbWeakPristine) {
                            layer_on(LA_LTHUMBESTRONG);
                        }
                    }
                }
                layer_off_mo_layer(LA_LTHUMBEMO);
            }
            return true;
        case KC_UP:
        case KC_DOWN:
        case KC_LEFT:
        case KC_RIGHT:
        case KC_DEL:
        case KC_ENT:
        case KC_BSPC:
            if (record->event.pressed && !isWeakLaMouseStarted) {
                reverse_weak_layer();
            }
            return true;
        default:
            return true;
    }
}
bool processKeycodeIfLThumbDMo(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case MA_LTHUMB:
        case MA_LTHUMBMS:
        case MA_LTHUMBD:
            if (!(record->event.pressed)) {
                if (!isWeakLaMouseStarted) {
                    if (isFloodUpOn
                    || isFloodDownOn
                    || isFloodLeftOn
                    || isFloodRightOn
                    || isFloodDelOn
                    || isFloodEntOn
                    || isFloodBspcOn
                    || isFloodTabOn
                    || isScrollLeftOn
                    || isScrollRightOn
                    || isScrollUpOn
                    || isScrollDownOn) {
                        reverse_weak_layer();
                    } else {
                        if (isLThumbMoPristine && isLThumbWeakPristine) {
                            layer_on(LA_LTHUMBDSTRONG);
                        }
                    }
                }
                layer_off_mo_layer(LA_LTHUMBDMO);
            }
            return true;
        case KC_UP:
        case KC_DOWN:
        case KC_LEFT:
        case KC_RIGHT:
        case KC_DEL:
        case KC_ENT:
        case KC_BSPC:
            if (record->event.pressed && !isWeakLaMouseStarted) {
                reverse_weak_layer();
            }
            return true;
        default:
            return true;
    }
}
