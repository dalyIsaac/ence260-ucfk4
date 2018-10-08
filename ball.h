#ifndef BALL_H
#define BALL_H

#include "system.h"

/**
 * @brief Starting column for the ball
 */
#define STARTING_COLUMN 0

/**
 * @brief Specifies the values for the various directions
 */
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

/**
 * @brief Definition for the Ball type. The old values are kept in order to wipe them from
 * the display, so that the new position can be written without retaining the old position.
 */
typedef struct ball_s
{
    int8_t old_row;
    int8_t old_column;
    int8_t new_row;
    int8_t new_column;
    uint8_t velocity;
    Direction direction;

} Ball;

#endif