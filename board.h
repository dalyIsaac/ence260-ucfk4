/**
 * @file board.h
 * @author Isaac Daly (idd17@uclive.ac.nz)
 * @brief Contains the board/display's function declarations and macro definitions which are to
 * be shared with other files.
 * @version 0.2
 * @date 2018-10-10
 *
 * @copyright Copyright (c) 2018
 *
 */
#ifndef BOARD_H
#define BOARD_H
#include "system.h"

/**
 * @brief The bottom row of the display
 */
#define BOTTOM_ROW 0

/**
 * @brief The top row of the display.
 */
#define TOP_ROW LEDMAT_ROWS_NUM - 1

void board_init(void);

void board_task(__unused__ void* data);
#endif