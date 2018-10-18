/**
 * @file puck.h
 * @author Isaac Daly (idd17@uclive.ac.nz)
 * @author Divyean Sivarman (dsi3@uclive.ac.nz)
 * @brief Contains the puck's function declarations and macro definitions which
 * are to be shared with other files.
 * @version 1.0
 * @date 2018-10-18
 *
 * @copyright Copyright (c) 2018
 *
 */
#ifndef PUCK_H
#define PUCK_H
#include "navswitch.h"
#include "system.h"

/**
 * @brief The column which the puck resides in
 */
#define PUCK_COL 4

/**
 * @brief Arbitrary numbers for the starting old_* attributes
 */
#define STARTING_OLD 0

/**
 * @brief The initial top column for the puck.
 */
#define STARTING_TOP 4

/**
 * @brief The initial bottom column for the puck.
 */
#define STARTING_BOTTOM 2

/**
 * @brief Corrected the name, according to the compass scheme (see
 * media/compass.png). South on this game's compass is defined as North in
 * navswitch.h
 */
#define NAVSWITCH_COMPASS_SOUTH NAVSWITCH_NORTH

/**
 * @brief Corrected the name, according to the compass scheme (see
 * media/compass.png). North on this game's compass is defined as South in
 * navswitch.h
 */
#define NAVSWITCH_COMPASS_NORTH NAVSWITCH_SOUTH

/**
 * @brief Specifies the values for the movement of the navswitch.
 * It is assumed that the orientation of the device is such that the IR I/O is
 * faced away from the player.
 */
typedef enum nav_movement_e {
    PUCK_MOVE_SOUTH = -1,
    PUCK_MOVE_NORTH = 1
} NavMovement;

/**
 * @brief Definition for the Puck type. The old values are kept in order to wipe
 * them from the display, so that the new position can be written without
 * retaining the old position.
 */
typedef struct puck_s
{
    int8_t old_bottom;
    int8_t old_top;
    int8_t new_bottom;
    int8_t new_top;
} Puck;

/**
 * @brief The puck for this board.
 *
 */
Puck puck;

/**
 * @brief Creates a puck, and adds it to the board.
 * CAN ONLY BE USED AFTER board_init().
 *
 */
void puck_init(void);

/**
 * @brief Updates the puck's position based on the user's interaction with the
 * navswitch.
 *
 */
void puck_task(__unused__ void* data);

#endif