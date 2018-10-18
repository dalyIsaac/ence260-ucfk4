/**
 * @file board.c
 * @author Isaac Daly (idd17@uclive.ac.nz)
 * @author Divyean Sivarman (dsi3@uclive.ac.nz)
 * @brief Contains definitions for the board/display.
 * @version 1.0
 * @date 2018-10-18
 *
 * @copyright Copyright (c) 2018
 *
 * @note Comments for non-static functions and variables are inside the
 * associated header file.
 *
 */

#include "board.h"

#include "ball.h"
#include "display.h"
#include "ir_uart.h"
#include "pacer.h"

void board_init(void)
{
    lost_game = false;
    continue_game = true;
    display_init();
}

void board_task(__unused__ void* data)
{
    display_update();
}