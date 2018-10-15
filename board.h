/**
 * @file board.h
 * @author Isaac Daly (idd17@uclive.ac.nz)
 * @author Divyean Sivarman (dsi3@uclive.ac.nz)
 * @brief Contains the board/display's function declarations and macro definitions which are to
 * be shared with other files.
 * @version 0.3
 * @date 2018-10-16
 *
 * @copyright Copyright (c) 2018
 *
 */
#ifndef BOARD_H
#define BOARD_H
#include "system.h"

#define MESSAGE_RATE 10
#define PACER_RATE 500
#define LOOP_RATE PACER_RATE

/**
 * @brief The bottom row of the display
 */
#define BOTTOM_ROW 0

/**
 * @brief The top row of the display.
 */
#define TOP_ROW LEDMAT_ROWS_NUM - 1

void negotiate_first_player(void);

void text_init(void);

void show_initial_text(void);

void board_init(void);

void board_task(__unused__ void* data);
#endif