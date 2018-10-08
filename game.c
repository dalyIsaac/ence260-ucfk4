/**
 * @file game.c
 * @author Isaac Daly (idd17@uclive.ac.nz)
 * @brief Main module for the game
 * @version 0.1
 * @date 2018-10-08
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#include "system.h"
#include "pio.h"
#include "pacer.h"
#include "navswitch.h"
#include "board.h"
#include "puck.h"
#include "ball.h"

/**
 * @brief Main function for the game
 * @return int 
 */
int main(void)
{
    system_init();
    pacer_init(500);
    navswitch_init();
    board_init();
    puck_init();
    ball_init();

    while (1)
    {
        // ball_update_value();
        puck_task();
        board_display();
    }
}
