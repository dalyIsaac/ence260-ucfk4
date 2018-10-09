/**
 * @file board.h
 * @author Isaac Daly (idd17@uclive.ac.nz)
 * @brief Contains the board/display's function declarations and macro definitions which are to 
 * be shared with other files. 
 * @version 0.1
 * @date 2018-10-08
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#ifndef BOARD_H
#define BOARD_H
#include "system.h"

void board_init(void);

void board_task(__unused__ void *data);
#endif