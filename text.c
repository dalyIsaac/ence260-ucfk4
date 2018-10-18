/**
 * @file text.c
 * @author Isaac Daly (idd17@uclive.ac.nz)
 * @author Divyean Sivarman (dsi3@uclive.ac.nz)
 * @brief Contains definitions for the text displayed to the user through the
 * display.
 * @version 1.0
 * @date 2018-10-18
 *
 * @copyright Copyright (c) 2018
 *
 * @note Comments for non-static functions and variables are inside the
 * associated header file.
 *
 */

#include "text.h"

#include "../fonts/font5x7_1.h"
#include "ball.h"
#include "navswitch.h"
#include "pacer.h"
#include "tinygl.h"

void text_init(void)
{
    pacer_init(PACER_RATE);
    tinygl_init(LOOP_RATE);
    tinygl_font_set(&font5x7_1);
    tinygl_text_speed_set(MESSAGE_RATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
}

void show_initial_text(void)
{
    bool start_game = false;
    tinygl_text("PRESS THE NAVSWITCH ONCE THE BOARDS ARE ALIGNED TO PLAY.");
    while (!start_game) {
        pacer_wait();
        tinygl_update();

        navswitch_update();
        if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
            start_game = true;
        }
    }
}

void notify(void)
{
    if (lost_game) {
        tinygl_text("LOST. PRESS NAVSWITCH DOWN TO PLAY AGAIN. PRESS RESET "
                    "BUTTON TO END GAME.");
    } else {
        tinygl_text("WON. PRESS NAVSWITCH DOWN TO PLAY AGAIN. PRESS RESET "
                    "BUTTON TO END GAME.");
    }
    while (1) {
        pacer_wait();
        tinygl_update();

        navswitch_update();
        if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
            break;
        }
    }
    display_init();
}