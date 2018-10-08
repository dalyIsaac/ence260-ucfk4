/**
 * @file puck.h
 * @author Isaac Daly (idd17@uclive.ac.nz)
 * @brief Contains the puck's function declarations and macro definitions which are to 
 * be shared with other files. 
 * @version 0.1
 * @date 2018-10-08
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#ifndef PUCK_H
#define PUCK_H
#include "system.h"

/**
 * @brief The column which the puck resides in
 */
#define PUCK_COL 4

/**
 * @brief Specifies the values for the movement of the navswitch.
 * It is assumed that the orientation of the device is such that the IR I/O is faced away
 * from the player.
 */
typedef enum nav_movement_e
{
    left = -1,
    right = 1
} NavMovement;

/**
 * @brief Definition for the Puck type. The old values are kept in order to wipe them from
 * the display, so that the new position can be written without retaining the old position.
 */
typedef struct puck_s
{
    int8_t old_bottom;
    int8_t old_top;
    int8_t new_bottom;
    int8_t new_top;
} Puck;

void puck_init(void);

void puck_update_value(NavMovement change);

void puck_task(void);

#define STARTING_COLUMN 0

typedef enum direction_e
{
    north = 1,
    north_east = 2,
    east = 3,
    south_east = 4,
    south = -1,
    south_west = -2,
    west = -3,
    north_west = -4
} Direction;

typedef struct ball_s
{
    int8_t old_row;
    int8_t old_column;
    int8_t new_row;
    int8_t new_column;
    uint8_t velocity;
    Direction direction;

} Ball;

void ball_init(void);

#endif