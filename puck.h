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
typedef enum nav_movement
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
