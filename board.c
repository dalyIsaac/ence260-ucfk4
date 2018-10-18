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
 * @note Comments for non-static functions and variables are inside the associated header file.
 *
 */

#include "board.h"

#include "ball.h"
#include "display.h"
#include "ir_uart.h"
#include "pacer.h"

void negotiate_first_player(void)
{
    int8_t received_data = 0;

    pacer_wait();
    if (ir_uart_read_ready_p()) { // receives data first, so is player 2
        received_data = ir_uart_getc();
        if (received_data == I_AM_PLAYER_ONE) {
            ir_uart_putc(I_AM_PLAYER_TWO);
            have_ball = false;
        }
    } else {
        while (received_data != I_AM_PLAYER_TWO) { // sends data first, so is player 1
            ir_uart_putc(I_AM_PLAYER_ONE);         // this board has "claimed" player 1
            pacer_wait();
            received_data = ir_uart_getc();
        }
        have_ball = true;
    }
}

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