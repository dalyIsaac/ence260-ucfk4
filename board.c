/**
 * @file board.c
 * @author Isaac Daly (idd17@uclive.ac.nz)
 * @brief Contains definitions for the board/display
 * @version 0.1
 * @date 2018-10-08
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#include "pio.h"
#include "pacer.h"
#include "board.h"
#include "display.h"

/**
 * @brief Initialises the display/board
 */
void board_init(void)
{
    display_init();
}

/**
 * @brief Displays the board
 */
void board_task(__unused__ void *data)
{
    display_update();
}