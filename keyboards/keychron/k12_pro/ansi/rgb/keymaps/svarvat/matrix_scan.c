
uint16_t scrollDelayTimer;

void matrix_scan_user(void) {
    if (IS_LAYER_ON(LA_MOUSE)) {
        if (isMouseX4Started) {
            if (mouseRight) {
                report_mouse_t currentReport = pointing_device_get_report();
                currentReport.x = 8;
                pointing_device_set_report(currentReport);
                pointing_device_send();
            } else if (mouseLeft) {
                report_mouse_t currentReport = pointing_device_get_report();
                currentReport.x = -8;
                pointing_device_set_report(currentReport);
                pointing_device_send();
            }
            if (mouseDown) {
                report_mouse_t currentReport = pointing_device_get_report();
                currentReport.y = 8;
                pointing_device_set_report(currentReport);
                pointing_device_send();
            } else if (mouseUp) {
                report_mouse_t currentReport = pointing_device_get_report();
                currentReport.y = -8;
                pointing_device_set_report(currentReport);
                pointing_device_send();
            }
        } else if (isMouseX2Started) {
            if (mouseDown) {
                report_mouse_t currentReport = pointing_device_get_report();
                currentReport.y = 3;
                pointing_device_set_report(currentReport);
                pointing_device_send();
            } else if (mouseUp) {
                report_mouse_t currentReport = pointing_device_get_report();
                currentReport.y = -3;
                pointing_device_set_report(currentReport);
                pointing_device_send();
            }
            if (mouseRight) {
                report_mouse_t currentReport = pointing_device_get_report();
                currentReport.x = 3;
                pointing_device_set_report(currentReport);
                pointing_device_send();
            } else if (mouseLeft) {
                report_mouse_t currentReport = pointing_device_get_report();
                currentReport.x = -3;
                pointing_device_set_report(currentReport);
                pointing_device_send();
            }
        } else if (isMouseX1Started && (mouseDown || mouseLeft || mouseUp || mouseRight)) {
            if (timer_elapsed(scrollDelayTimer) > 10) {
                if (mouseDown || mouseUp) {
                    report_mouse_t currentReport = pointing_device_get_report();
                    currentReport.y = mouseDown ? 1 : -1;
                    pointing_device_set_report(currentReport);
                    pointing_device_send();
                }
                if (mouseRight || mouseLeft) {
                    report_mouse_t currentReport = pointing_device_get_report();
                    currentReport.x = mouseRight ? 1 : -1;
                    pointing_device_set_report(currentReport);
                    pointing_device_send();
                }
                scrollDelayTimer = timer_read();
            }
        }
        if (isScrollX4Started) {
            if (scrollDown || scrollUp) {
                if (timer_elapsed(scrollDelayTimer) > 100) {
                    report_mouse_t currentReport = pointing_device_get_report();
                    currentReport.v = scrollUp ? 20 : -20;
                    pointing_device_set_report(currentReport);
                    pointing_device_send();
                    scrollDelayTimer = timer_read();
                }
            }
            if (scrollLeft || scrollRight) {
                if (timer_elapsed(scrollDelayTimer) > 100) {
                    report_mouse_t currentReport = pointing_device_get_report();
                    currentReport.h = scrollLeft ? -20 : 20;
                    pointing_device_set_report(currentReport);
                    pointing_device_send();
                    scrollDelayTimer = timer_read();
                }
            }
        } else if (isScrollX2Started) {
            if (scrollDown || scrollUp) {
                if (timer_elapsed(scrollDelayTimer) > 100) {
                    report_mouse_t currentReport = pointing_device_get_report();
                    currentReport.v = scrollUp ? 10 : -10;
                    pointing_device_set_report(currentReport);
                    pointing_device_send();
                    scrollDelayTimer = timer_read();
                }
            }
            if (scrollLeft || scrollRight) {
                if (timer_elapsed(scrollDelayTimer) > 100) {
                    report_mouse_t currentReport = pointing_device_get_report();
                    currentReport.h = scrollLeft ? -10 : 10;
                    pointing_device_set_report(currentReport);
                    pointing_device_send();
                    scrollDelayTimer = timer_read();
                }
            }
        } else
        if (isScrollX1Started) {
            if (scrollDown || scrollUp) {
                if (timer_elapsed(scrollDelayTimer) > 40) {
                    report_mouse_t currentReport = pointing_device_get_report();
                    currentReport.v = scrollUp ? 1 : -1;
                    pointing_device_set_report(currentReport);
                    pointing_device_send();
                    scrollDelayTimer = timer_read();
                }
            }
            if (scrollLeft || scrollRight) {
                if (timer_elapsed(scrollDelayTimer) > 40) {
                    report_mouse_t currentReport = pointing_device_get_report();
                    currentReport.h = scrollLeft ? -1 : 1;
                    pointing_device_set_report(currentReport);
                    pointing_device_send();
                    scrollDelayTimer = timer_read();
                }
            }
        }
    }
}


