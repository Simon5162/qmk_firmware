
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

static void move_mouse(int8_t x, int8_t y) {
    report_mouse_t r = pointing_device_get_report();
    r.x = x;
    r.y = y;
    pointing_device_set_report(r);
    pointing_device_send();
}

static void scroll_mouse(int8_t v, int8_t h) {
    report_mouse_t r = pointing_device_get_report();
    r.v = v;
    r.h = h;
    pointing_device_set_report(r);
    pointing_device_send();
}

static void dispatch_flood_keys(void) {
    if (isFloodUpOn)    tap_code16(KC_UP);
    if (isFloodDownOn)  tap_code16(KC_DOWN);
    if (isFloodLeftOn)  tap_code16(KC_LEFT);
    if (isFloodRightOn) tap_code16(KC_RIGHT);
    if (isFloodDelOn)   tap_code16(KC_DEL);
    if (isFloodEntOn)   tap_code16(KC_ENT);
    if (isFloodBspcOn)  tap_code16(KC_BSPC);
}

void matrix_scan_user(void) {
    if (bt_wake_pending && timer_elapsed32(bt_wake_timer) > floodBlueToothInterval) {
        bt_wake_pending = false;
        tap_code16(KC_RSFT);
    }
    if (IS_LAYER_ON(LA_MOUSE)) {
        if (IS_LAYER_ON(LA_LTHUMBEMO)) {
            int8_t mx = isMouseRightOn ? 8 : (isMouseLeftOn ? -8 : 0);
            int8_t my = isMouseDownOn  ? 8 : (isMouseUpOn   ? -8 : 0);
            if (mx || my) move_mouse(mx, my);

            if (isScrollDownOn || isScrollUpOn || isScrollLeftOn || isScrollRightOn) {
                if (timer_elapsed(floodTimerX3) > floodScrollIntervalX3) {
                    scroll_mouse(isScrollUpOn   ? 20 : (isScrollDownOn  ? -20 : 0),
                                 isScrollRightOn ? 20 : (isScrollLeftOn ? -20 : 0));
                    floodTimerX3 = timer_read();
                }
            }
            if (isFloodUpOn || isFloodDownOn || isFloodLeftOn || isFloodRightOn
            || isFloodDelOn || isFloodEntOn || isFloodBspcOn) {
                if (timer_elapsed(floodTimerX3) > floodKeyCodeIntervalX3) {
                    dispatch_flood_keys();
                    floodTimerX3 = timer_read();
                }
            }
        } else if (IS_LAYER_ON(LA_LTHUMBDMO)) {
            int8_t mx = isMouseRightOn ? 3 : (isMouseLeftOn ? -3 : 0);
            int8_t my = isMouseDownOn  ? 3 : (isMouseUpOn   ? -3 : 0);
            if (mx || my) move_mouse(mx, my);

            if (isScrollDownOn || isScrollUpOn || isScrollLeftOn || isScrollRightOn) {
                if (timer_elapsed(floodTimerX2) > floodScrollIntervalX2) {
                    scroll_mouse(isScrollUpOn   ? 10 : (isScrollDownOn  ? -10 : 0),
                                 isScrollRightOn ? 10 : (isScrollLeftOn ? -10 : 0));
                    floodTimerX2 = timer_read();
                }
            }
            if (isFloodUpOn || isFloodDownOn || isFloodLeftOn || isFloodRightOn
            || isFloodDelOn || isFloodEntOn || isFloodBspcOn) {
                if (timer_elapsed(floodTimerX2) > floodKeyCodeIntervalX2) {
                    dispatch_flood_keys();
                    floodTimerX2 = timer_read();
                }
            }
        } else {
            if (isMouseRightOn || isMouseLeftOn || isMouseDownOn || isMouseUpOn) {
                if (timer_elapsed(floodTimerX1) > floodMouseIntervalX1) {
                    move_mouse(isMouseRightOn ? 1 : (isMouseLeftOn ? -1 : 0),
                               isMouseDownOn  ? 1 : (isMouseUpOn   ? -1 : 0));
                    floodTimerX1 = timer_read();
                }
            }
            if (isScrollDownOn || isScrollUpOn || isScrollLeftOn || isScrollRightOn) {
                if (timer_elapsed(floodTimerX1) > floodScrollIntervalX1) {
                    scroll_mouse(isScrollUpOn   ? 1 : (isScrollDownOn  ? -1 : 0),
                                 isScrollRightOn ? 1 : (isScrollLeftOn ? -1 : 0));
                    floodTimerX1 = timer_read();
                }
            }
        }
    }
}
