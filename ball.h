#ifndef BALL_H
#define BALL_H

#include "system.h"
#include "ledmat.h"

/**
 * @brief Starting column for the ball
 */
#define STARTING_COLUMN 0
#define TOP_ROW 0
#define BOTTOM_ROW LEDMAT_ROWS_NUM - 1
#define INITIAL_VELOCITY 1
#define UNINITIALISED 8

/**
 * @brief Specifies the values for the various directions
 */
typedef enum direction_e
{
    north_west = 1,
    north = 2,
    north_east = 3,
    east = 4,
    south_east = -3,
    south = -2,
    south_west = -1,
    west = -4,
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

/**
 * @brief Definition for the ImpactPoint type. It indicates where on the puck the ball impacted.
 */
typedef enum impact_point_e
{
    bottom = 0,
    middle = 1,
    top = 2
} ImpactPoint;

void ball_update_value(void);

void ball_init(void);

void ball_task(__unused__ void *data);

#endif