/* Copyright 2021 Colin Lam (Ploopy Corporation)
 * Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2019 Sunjun Kim
 * Copyright 2019 Hiroyuki Okada
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

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[0] = LAYOUT( QK_MOUSE_BUTTON_1 )
};

bool is_scrolling = false;

int16_t scroll_accum_h = 0;
int16_t scroll_accum_v = 0;

int wiggle_count = 0;
bool last_direction = false;
uint16_t last_wiggle_time = 0;
uint16_t last_switch_time = 0;

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    int16_t x_before = mouse_report.x;
    int16_t y_before = mouse_report.y;

    if (timer_read() - last_switch_time > 250) {
        if (timer_read() - last_wiggle_time > 150) {
            wiggle_count = 0;
        }

        if (x_before > 1 && y_before < 3 && !last_direction) {
            wiggle_count++;
            last_wiggle_time = timer_read();
            last_direction = !last_direction;
        }
        if (x_before < -1 && y_before < 3 && last_direction) {
            wiggle_count++;
            last_wiggle_time = timer_read();
            last_direction = !last_direction;
         }

        if (wiggle_count > 3) {
            is_scrolling = !is_scrolling;
            wiggle_count = 0;
            last_wiggle_time = 0;
            last_switch_time = 0;
         }
      }

    if (is_scrolling) {
        scroll_accum_h += x_before;
        scroll_accum_v += y_before;

        if (scroll_accum_h > 150) {
            mouse_report.h = 1;
            scroll_accum_h = 0;
         } else if (scroll_accum_h < -150) {
            mouse_report.h = -1;
            scroll_accum_h = 0;
         }

        if (scroll_accum_v > 150) {
            mouse_report.v = -1;
            scroll_accum_v = 0;
         } else if (scroll_accum_v < -150) {
            mouse_report.v = 1;
            scroll_accum_v = 0;
         }

        mouse_report.x = 0;
        mouse_report.y = 0;
      } else {
        mouse_report.h = 0;
        mouse_report.v = 0;
      }

    return mouse_report;
}


