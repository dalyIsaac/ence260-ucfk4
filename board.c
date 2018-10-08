/**
 * @file board.c
 * @author Isaac Daly (idd17@uclive.ac.nz)
 * @brief Contains definitions for the board/display, and the puck (at this point)
 * @version 0.1
 * @date 2018-10-08
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#include "pio.h"
#include "pacer.h"
#include "board.h"
#include "navswitch.h"

static uint8_t previous_column = 5;

/**
 * @brief Boolean array which represents the board/display.
 * Consists of 5 column x 7 rows
 */
static bool board[LEDMAT_COLS_NUM][LEDMAT_ROWS_NUM] = {false};

/**
 * @brief Puck for this user in the game
 */
Puck puck = {0};

/**
 * @brief The PIO pins driving the LED matrix rows
 */
static const pio_t rows[] =
    {
        LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO,
        LEDMAT_ROW4_PIO, LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO,
        LEDMAT_ROW7_PIO};

/**
 * @brief The PIO pins driving the LED matrix columns
 */
static const pio_t cols[] =
    {
        LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
        LEDMAT_COL4_PIO, LEDMAT_COL5_PIO};

/**
 * @brief Assigns the integer value to the board, at the specified column
 * @param value The value to write
 * @param column The column to write the value to write to
 */
void assign_column(uint8_t value, uint8_t column)
{
    for (uint8_t i = 0; i < LEDMAT_ROWS_NUM; i++)
    {
        board[column][i] = (value >> i) & 1;
    }
}

/**
 * @brief Gets the integer value of the column from the board
 * @param column The current column to acquire
 * @return int The value from the specified column
 */
uint8_t get_column(uint8_t column)
{
    uint8_t value = 0;
    uint8_t placeValue = 1;

    for (uint8_t i = 0; i < LEDMAT_ROWS_NUM; i++)
    {
        value += placeValue * board[column][i];
        placeValue *= 2;
    }

    return value;
}

/**
 * @brief Displays the current column based on the board's definition for that
 * column
 * @param current_column The current column to display
 */
void display_column(uint8_t current_column)
{
    uint8_t column_pattern = get_column(current_column);
    pio_output_high(cols[previous_column]);

    for (uint8_t current_row = 0; current_row < LEDMAT_ROWS_NUM; current_row++)
    {
        if ((column_pattern >> current_row) & 1)
        {
            pio_output_low(rows[current_row]);
        }
        else
        {
            pio_output_high(rows[current_row]);
        }
    }
    pio_output_low(cols[current_column]);
    previous_column = current_column;
}

void board_set_cell(uint8_t col, uint8_t row, bool value)
{
    board[col][row] = value;
}

/**
 * @brief Updates the puck in the board
 */
void puck_update_display(void)
{
    // wipe the old puck
    for (uint8_t i = puck.old_bottom; i <= puck.old_top; i++)
    {
        board[PUCK_COL][i] = false;
    }

    // set the new puck
    for (uint8_t i = puck.new_bottom; i <= puck.new_top; i++)
    {
        board[PUCK_COL][i] = true;
    }
}

/**
 * @brief Creates a puck, and adds it to the board
 */
void puck_create(void)
{
    Puck new_puck = {.old_top = 0, .old_bottom = 0, .new_top = 4, .new_bottom = 2};
    puck = new_puck;
    puck_update_display();
}

/**
 * @brief Initialises the display/board
 */
void board_init(void)
{
    // Sets the display to empty
    pio_config_set(LEDMAT_COL1_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_COL2_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_COL3_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_COL4_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_COL5_PIO, PIO_OUTPUT_HIGH);

    pio_config_set(LEDMAT_ROW1_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW2_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW3_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW4_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW5_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW6_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW7_PIO, PIO_OUTPUT_HIGH);

    puck_create();
}

void puck_update(NavMovement change)
{
    if (puck.new_bottom + change >= 0 && puck.new_top + change < LEDMAT_ROWS_NUM)
    {
        Puck new_puck = {
            .old_bottom = puck.new_bottom,
            .old_top = puck.new_top,
            .new_bottom = puck.new_bottom + change,
            .new_top = puck.new_top + change};
        puck = new_puck;
        puck_update_display();
    }
}

/**
 * @brief Displays the board
 */
void display_board(void)
{
    for (uint8_t current_column = 0; current_column < LEDMAT_COLS_NUM; current_column++)
    {
        pacer_wait();
        display_column(current_column);
    }
}

void puck_task(void)
{
    navswitch_update();

    if (navswitch_push_event_p(NAVSWITCH_NORTH))
    {
        puck_update(down);
    }

    if (navswitch_push_event_p(NAVSWITCH_SOUTH))
    {
        puck_update(up);
    }
}