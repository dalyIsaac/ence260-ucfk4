#ifndef GAME_H
#define GAME_H

#include "system.h"

/**
 * @brief The rate at which the board's display updates.
 *
 */
#define BOARD_DISPLAY_TASK_RATE 250

/**
 * @brief The rate at which the puck's task runs.
 *
 */
#define PUCK_TASK_RATE 100

/**
 * @brief The rate at which the ball's task runs.
 *
 */
#define BALL_TASK_RATE 100

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

#endif