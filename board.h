/**
 * @file board.h
 * @author Isaac Daly (idd17@uclive.ac.nz)
 * @author Divyean Sivarman (dsi3@uclive.ac.nz)
 * @brief Contains the board/display's function declarations and macro definitions which are to
 * be shared with other files.
 * @version 1.0
 * @date 2018-10-18
 *
 * @copyright Copyright (c) 2018
 *
 */

#ifndef BOARD_H
#define BOARD_H

#include "system.h"

/**
 * @brief The rate at which the message moves.
 *
 */
#define MESSAGE_RATE 10

/**
 * @brief The rate at which the pacer refreshes.
 *
 */
#define PACER_RATE 500

/**
 * @brief The rate at which tinygl loops.
 *
 */
#define LOOP_RATE PACER_RATE

/**
 * @brief The bottom row of the display
 */
#define BOTTOM_ROW 0

/**
 * @brief The top row of the display.
 */
#define TOP_ROW LEDMAT_ROWS_NUM - 1

/**
 * @brief Negotiates between the two boards who the first player is.
 *
 */
void negotiate_first_player(void);

/**
 * @brief Initialises tinygl and the pacer.
 *
 */
void text_init(void);

/**
 * @brief Displays the initial text for the game. It exits when the user pushes the navswitch.
 *
 */
void show_initial_text(void);

/**
 * @brief Notifies the user whether they won, and how to restart the game.
 *
 */
void notify(void);

/**
 * @brief Initialises the display/board for a new game.
 *
 */
void board_init(void);

/**
 * @brief Displays the board.
 *
 */
void board_task(__unused__ void* data);

#endif