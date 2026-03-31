
uint16_t floodTimerX3 = 0;
uint16_t floodTimerX2 = 0;
uint16_t floodTimerX1 = 0;
uint16_t floodKeyCodeIntervalX3 = 80;
uint16_t floodKeyCodeIntervalX2 = 150;
uint16_t floodScrollIntervalX1 = 40;
uint16_t floodScrollIntervalX2 = 100;
uint16_t floodScrollIntervalX3 = 100;
uint16_t floodMouseIntervalX1 = 10;
uint16_t floodMouseIntervalX2 = 100;
uint16_t floodMouseIntervalX3 = 100;
uint16_t floodBlueToothInterval = 500;

extern bool bt_wake_pending;
extern uint32_t bt_wake_timer;

extern bool isFloodUpOn;
extern bool isFloodDownOn;
extern bool isFloodLeftOn;
extern bool isFloodRightOn;
extern bool isFloodDelOn;
extern bool isFloodEntOn;
extern bool isFloodBspcOn;

void matrix_scan_user(void) {
    if (bt_wake_pending && timer_elapsed32(bt_wake_timer) > floodBlueToothInterval) {
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
                if (timer_elapsed(floodTimerX3) > floodScrollIntervalX3) {
                    report_mouse_t currentReport = pointing_device_get_report();
                    currentReport.v = isScrollUpOn ? 20 : -20;
                    pointing_device_set_report(currentReport);
                    pointing_device_send();
                    floodTimerX3 = timer_read();
                }
            }
            if (isScrollLeftOn || isScrollRightOn) {
                if (timer_elapsed(floodTimerX3) > floodScrollIntervalX3) {
                    report_mouse_t currentReport = pointing_device_get_report();
                    currentReport.h = isScrollLeftOn ? -20 : 20;
                    pointing_device_set_report(currentReport);
                    pointing_device_send();
                    floodTimerX3 = timer_read();
                }
            }
            if (isFloodUpOn || isFloodDownOn || isFloodLeftOn || isFloodRightOn
            || isFloodDelOn || isFloodEntOn || isFloodBspcOn) {
                if (timer_elapsed(floodTimerX3) > floodKeyCodeIntervalX3) {
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
                if (timer_elapsed(floodTimerX2) > floodScrollIntervalX2) {
                    report_mouse_t currentReport = pointing_device_get_report();
                    currentReport.v = isScrollUpOn ? 10 : -10;
                    pointing_device_set_report(currentReport);
                    pointing_device_send();
                    floodTimerX2 = timer_read();
                }
            }
            if (isScrollLeftOn || isScrollRightOn) {
                if (timer_elapsed(floodTimerX2) > floodScrollIntervalX2) {
                    report_mouse_t currentReport = pointing_device_get_report();
                    currentReport.h = isScrollLeftOn ? -10 : 10;
                    pointing_device_set_report(currentReport);
                    pointing_device_send();
                    floodTimerX2 = timer_read();
                }
            }
            if (isFloodUpOn || isFloodDownOn || isFloodLeftOn || isFloodRightOn
            || isFloodDelOn || isFloodEntOn || isFloodBspcOn) {
                if (timer_elapsed(floodTimerX2) > floodKeyCodeIntervalX2) {
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
                if (timer_elapsed(floodTimerX1) > floodMouseIntervalX1) {
                    report_mouse_t currentReport = pointing_device_get_report();
                    currentReport.y = isMouseDownOn ? 1 : -1;
                    pointing_device_set_report(currentReport);
                    pointing_device_send();
                    floodTimerX1 = timer_read();
                }
            }
            if (isMouseRightOn || isMouseLeftOn) {
                if (timer_elapsed(floodTimerX1) > floodMouseIntervalX1) {
                    report_mouse_t currentReport = pointing_device_get_report();
                    currentReport.x = isMouseRightOn ? 1 : -1;
                    pointing_device_set_report(currentReport);
                    pointing_device_send();
                    floodTimerX1 = timer_read();
                }
            }
            if (isScrollDownOn || isScrollUpOn) {
                if (timer_elapsed(floodTimerX1) > floodScrollIntervalX1) {
                    report_mouse_t currentReport = pointing_device_get_report();
                    currentReport.v = isScrollUpOn ? 1 : -1;
                    pointing_device_set_report(currentReport);
                    pointing_device_send();
                    floodTimerX1 = timer_read();
                }
            }
            if (isScrollLeftOn || isScrollRightOn) {
                if (timer_elapsed(floodTimerX1) > floodScrollIntervalX1) {
                    report_mouse_t currentReport = pointing_device_get_report();
                    currentReport.h = isScrollLeftOn ? -1 : 1;
                    pointing_device_set_report(currentReport);
                    pointing_device_send();
                    floodTimerX1 = timer_read();
                }
            }
        }
    }
}


