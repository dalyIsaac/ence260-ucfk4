/**
 * @file ball.c
 * @author Isaac Daly (idd17@uclive.ac.nz)
 * @author Divyean Sivarman (dsi3@uclive.ac.nz)
 * @brief Contains header definitions for the ball module.
 * @version 1.0
 * @date 2018-10-18
 *
 * @copyright Copyright (c) 2018
 *
 * @note The bottom of the board is the side closest to the USB port. The south
 * side is the bottom side of the board.
 */

#ifndef BALL_H
#define BALL_H

#include "ledmat.h"
#include "system.h"

/**
 * @brief Wipes bit X when used with a bitwise and.
 *
 */
#define WIPE_BIT(X) ~(1 << X)

/**
 * @brief The shift for which new_row has to be shifted into the transmitted
 * integer.
 *
 */
#define NEW_ROW_SHIFT 5

/**
 * @brief The shift for which the velocity has to be shifted into the
 * transmitted integer.
 *
 */
#define VELOCITY_SHIFT 3

/**
 * @brief The length of velocity inside the transmitted integer.
 *
 */
#define VELOCITY_BIT_LENGTH 2

/**
 * @brief The length of direction inside the transmitted integer.
 *
 */
#define DIRECTION_BIT_LENGTH 2

/**
 * @brief Starting row for the ball.
 *
 */
#define STARTING_ROW 3

/**
 * @brief Starting column for the ball.
 *
 */
#define STARTING_COLUMN 0

/**
 * @brief Starting direction for the ball.
 *
 */
#define STARTING_DIRECTION WEST

/**
 * @brief Starting velocity for the ball.
 *
 *  */
#define STARTING_VELOCITY 1

/**
 * @brief The first value which is checked against to see if the ball can
 * update.
 *
 */
#define FIRST_VALUE_FOR_UPDATE 99

/**
 * @brief Used for determining the subtract_value, which in turn allows the ball
 * to be updated depending on its velocity.
 *
 */
#define VARIABLE_PERIOD_NUMERATOR 100

/**
 * @brief Gets the timer, based on the continually incrementing counter.
 *
 */
#define GET_TIMER(counter) ((counter / 10) % 10) * 10 + counter % 10

/**
 * @brief Sent by the board which has just lost the game. This value was chosen
 * because the enum Direction has a maximum value of 5.
 *
 */
#define I_HAVE_LOST 7

/**
 * @brief The column in which every ball that has been just received starts off
 * in, or the row/column which the ball starts off in when the player does not
 * start the game.
 *
 */
#define BALL_RECEIVED_START STARTING_OLD - 1

/**
 * @brief The number of the last column in the board (per zero-based indexing).
 *
 */
#define LAST_COLUMN LEDMAT_COLS_NUM - 1

/**
 * @brief The number of the last row in the board (per zero-based indexing).
 *
 */
#define LAST_ROW LEDMAT_ROWS_NUM - 1

/**
 * @brief The column at which data is transmitted to the other board
 *
 */
#define TRANSMIT_COLUMN -1
/**
 * @brief The maximum velocity of the ball.
 *
 */
#define MAX_VELOCITY 4

/**
 * @brief Specifies the values for the various directions.
 *
 */
typedef enum direction_e {
    NORTH_EAST = 0,
    EAST = 1,
    SOUTH_EAST = 2,
    SOUTH_WEST = 3,
    WEST = 4,
    NORTH_WEST = 5,
} Direction;

/**
 * @brief Definition for the Ball type. The old values are kept in order to wipe
 * them from the display, so that the new position can be written without
 * retaining the old position.
 *
 */
typedef struct ball_s
{
    int8_t old_row;
    int8_t old_column;
    int8_t new_row;
    int8_t new_column;
    int8_t velocity;
    Direction direction;

} Ball;

/**
 * @brief Definition for the ImpactPoint type. It indicates where on the puck
 * the ball impacted.
 *
 */
typedef enum impact_point_e {
    IMPACT_BOTTOM = 0,
    IMPACT_MIDDLE = 1,
    IMPACT_TOP = 2,
    NO_IMPACT = -1
} ImpactPoint;

/**
 * @brief Indicates whether this board has the ball.
 *
 */
bool have_ball;

/**
 * @brief Indicates whether this game has lost the game. This is checked prior
 * to notifying the player of the result.
 *
 */
bool lost_game;

/**
 * @brief Used to indicate to the custom task scheduler whether the game is
 * still continuing.
 *
 */
bool continue_game;

/**
 * @brief Creates a ball, and adds it to the board.
 * CAN ONLY BE USED AFTER board_init().
 *
 */
void ball_init(void);

/**
 * @brief Updates the ball when it should.
 * If the velocity is 1, it updates when value is 99.
 * If the velocity is 2, it updates when value is 99, 49.
 * If the velocity is 3, it updates when value is 99, 66, 33.
 * ...
 *
 * @param void
 */
void ball_task(__unused__ void* data);

#endif