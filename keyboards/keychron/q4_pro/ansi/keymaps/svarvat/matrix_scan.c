
uint16_t scrollDelayTimer;
uint16_t floodTimerX3 = 0;
uint16_t floodTimerX2 = 0;
uint16_t floodIntervalX3 = 80;
uint16_t floodIntervalX2 = 150;

extern bool bt_wake_pending;
extern uint32_t bt_wake_timer;

extern bool isFloodUpOn;
extern bool isFloodDownOn;
extern bool isFloodLeftOn;
extern bool isFloodRightOn;
extern bool isFloodDelOn;
extern bool isFloodEntOn;
extern bool isFloodBspcOn;
extern bool isFloodUpOn;
extern bool isFloodDownOn;
extern bool isFloodLeftOn;
extern bool isFloodRightOn;
extern bool isFloodDelOn;
extern bool isFloodEntOn;
extern bool isFloodBspcOn;

void matrix_scan_user(void) {
    if (bt_wake_pending && timer_elapsed32(bt_wake_timer) > 500) {
        bt_wake_pending = false;
        tap_code16(KC_RSFT);
    }
    if (IS_LAYER_ON(LA_MOUSE)) {
        if (IS_LAYER_ON(LA_LTHUMBEMO)) {
            if (isMouseRightOn) {
                report_mouse_t currentReport = pointing_device_get_report();
                currentReport.x = 8;
                pointing_device_set_report(currentReport);
                pointing_device_send();
            } else if (isMouseLeftOn) {
                report_mouse_t currentReport = pointing_device_get_report();
                currentReport.x = -8;
                pointing_device_set_report(currentReport);
                pointing_device_send();
            }
            if (isMouseDownOn) {
                report_mouse_t currentReport = pointing_device_get_report();
                currentReport.y = 8;
                pointing_device_set_report(currentReport);
                pointing_device_send();
            } else if (isMouseUpOn) {
                report_mouse_t currentReport = pointing_device_get_report();
                currentReport.y = -8;
                pointing_device_set_report(currentReport);
                pointing_device_send();
            }
            if (isScrollDownOn || isScrollUpOn) {
                if (timer_elapsed(scrollDelayTimer) > 100) {
                    report_mouse_t currentReport = pointing_device_get_report();
                    currentReport.v = isScrollUpOn ? 20 : -20;
                    pointing_device_set_report(currentReport);
                    pointing_device_send();
                    scrollDelayTimer = timer_read();
                }
            }
            if (isScrollLeftOn || isScrollRightOn) {
                if (timer_elapsed(scrollDelayTimer) > 100) {
                    report_mouse_t currentReport = pointing_device_get_report();
                    currentReport.h = isScrollLeftOn ? -20 : 20;
                    pointing_device_set_report(currentReport);
                    pointing_device_send();
                    scrollDelayTimer = timer_read();
                }
            }
            if (isFloodFloodUpOn || isFloodDownOn || isFloodLeftOn || isRightOn
            || isFloodDelX3Started || isEntX3Started || isBspcOn) {
                if (timer_elapsed(floodTimerX3) > floodIntervalX3) {
                    if (isFloodUpOn)    tap_code16(KC_UP);
                    if (isFloodDownOn)  tap_code16(KC_DOWN);
                    if (isFloodLeftOn)  tap_code16(KC_LEFT);
                    if (isFloodRightOn) tap_code16(KC_RIGHT);
                    if (isFloodDelOn)   tap_code16(KC_DEL);
                    if (isFloodEntOn)   tap_code16(KC_ENT);
                    if (isFloodBspcOn)  tap_code16(KC_BSPC);
                    floodTimerX3 = timer_read();
                }
            }
        } else if (IS_LAYER_ON(LA_LTHUMBDMO)) {
            if (isMouseDownOn) {
                report_mouse_t currentReport = pointing_device_get_report();
                currentReport.y = 3;
                pointing_device_set_report(currentReport);
                pointing_device_send();
            } else if (isMouseUpOn) {
                report_mouse_t currentReport = pointing_device_get_report();
                currentReport.y = -3;
                pointing_device_set_report(currentReport);
                pointing_device_send();
            }
            if (isMouseRightOn) {
                report_mouse_t currentReport = pointing_device_get_report();
                currentReport.x = 3;
                pointing_device_set_report(currentReport);
                pointing_device_send();
            } else if (isMouseLeftOn) {
                report_mouse_t currentReport = pointing_device_get_report();
                currentReport.x = -3;
                pointing_device_set_report(currentReport);
                pointing_device_send();
            }
            if (isScrollDownOn || isScrollUpOn) {
                if (timer_elapsed(scrollDelayTimer) > 100) {
                    report_mouse_t currentReport = pointing_device_get_report();
                    currentReport.v = isScrollUpOn ? 10 : -10;
                    pointing_device_set_report(currentReport);
                    pointing_device_send();
                    scrollDelayTimer = timer_read();
                }
            }
            if (isScrollLeftOn || isScrollRightOn) {
                if (timer_elapsed(scrollDelayTimer) > 100) {
                    report_mouse_t currentReport = pointing_device_get_report();
                    currentReport.h = isScrollLeftOn ? -10 : 10;
                    pointing_device_set_report(currentReport);
                    pointing_device_send();
                    scrollDelayTimer = timer_read();
                }
            }
            if (isFloodFloodUpOn || isFloodDownOn || isFloodLeftOn || isRightOn
            || isFloodDelX2Started || isEntX2Started || isBspcOn) {
                if (timer_elapsed(floodTimerX2) > floodIntervalX2) {
                    if (isFloodUpOn)    tap_code16(KC_UP);
                    if (isFloodDownOn)  tap_code16(KC_DOWN);
                    if (isFloodLeftOn)  tap_code16(KC_LEFT);
                    if (isFloodRightOn) tap_code16(KC_RIGHT);
                    if (isFloodDelOn)   tap_code16(KC_DEL);
                    if (isFloodEntOn)   tap_code16(KC_ENT);
                    if (isFloodBspcOn)  tap_code16(KC_BSPC);
                    floodTimerX2 = timer_read();
                }
            }
        } else {
            if (isMouseDownOn || isMouseUpOn) {
                if (timer_elapsed(scrollDelayTimer) > 10) {
                    report_mouse_t currentReport = pointing_device_get_report();
                    currentReport.y = isMouseDownOn ? 1 : -1;
                    pointing_device_set_report(currentReport);
                    pointing_device_send();
                    scrollDelayTimer = timer_read();
                }
            }
            if (isMouseRightOn || isMouseLeftOn) {
                if (timer_elapsed(scrollDelayTimer) > 10) {
                    report_mouse_t currentReport = pointing_device_get_report();
                    currentReport.x = isMouseRightOn ? 1 : -1;
                    pointing_device_set_report(currentReport);
                    pointing_device_send();
                    scrollDelayTimer = timer_read();
                }
            }
            if (isScrollDownOn || isScrollUpOn) {
                if (timer_elapsed(scrollDelayTimer) > 40) {
                    report_mouse_t currentReport = pointing_device_get_report();
                    currentReport.v = isScrollUpOn ? 1 : -1;
                    pointing_device_set_report(currentReport);
                    pointing_device_send();
                    scrollDelayTimer = timer_read();
                }
            }
            if (isScrollLeftOn || isScrollRightOn) {
                if (timer_elapsed(scrollDelayTimer) > 40) {
                    report_mouse_t currentReport = pointing_device_get_report();
                    currentReport.h = isScrollLeftOn ? -1 : 1;
                    pointing_device_set_report(currentReport);
                    pointing_device_send();
                    scrollDelayTimer = timer_read();
                }
            }
        }
    }
}


