/**
 * @file text.h
 * @author Isaac Daly (idd17@uclive.ac.nz)
 * @author Divyean Sivarman (dsi3@uclive.ac.nz)
 * @brief Contains the text's function declarations and macro definitions which
 * are to be shared with other files.
 * @version 1.0
 * @date 2018-10-18
 *
 * @copyright Copyright (c) 2018
 *
 */

#ifndef TEXT_H
#define TEXT_H

/**
 * @brief The rate at which the message moves.
 *
 */
#define MESSAGE_RATE 10

/**
 * @brief The rate at which the pacer refreshes.
 *
 */
#define PACER_RATE 500

/**
 * @brief The rate at which tinygl loops.
 *
 */
#define LOOP_RATE PACER_RATE

/**
 * @brief Initialises tinygl and the pacer.
 *
 */
void text_init(void);

/**
 * @brief Displays the initial text for the game. It exits when the user pushes
 * the navswitch.
 *
 */
void show_initial_text(void);

/**
 * @brief Notifies the user whether they won, and how to restart the game.
 *
 */
void notify(void);

#endif