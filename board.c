/**
 * @file board.c
 * @author Isaac Daly (idd17@uclive.ac.nz)
 * @brief Contains definitions for the board/display
 * @version 0.1
 * @date 2018-10-08
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#include "pio.h"
#include "pacer.h"

static int previous_column = 5;

bool board[5][7] = {false}; // 5 columns x  7 rows

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
void assignColumn(int value, int column)
{
    for (int i = 0; i < 7; i++)
    {
        board[column][i] = (value >> i) & 1;
    }
}

/**
 * @brief Gets the integer value of the column from the board
 * @param column The current column to process
 * @return int The value from the specified column
 */
int getColumn(int column)
{
    int value = 0;
    int placeValue = 1;

    for (int i = 0; i < 7; i++)
    {
        int temp = placeValue * board[column][i];
        value += temp;
        placeValue *= 2;
    }

    return value;
}

void display_column(uint8_t row_pattern, uint8_t current_column)
{
    pio_output_high(cols[previous_column]);

    for (uint8_t current_row = 0; current_row < LEDMAT_ROWS_NUM; current_row++)
    {
        if ((row_pattern >> current_row) & 1)
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

/**
 * @brief Initialises the display/board
 */
void board_init(void)
{
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
}

const uint8_t bitmap[] = {0x30, 0x46, 0x40, 0x46, 0x30};

void display(void)
{
    uint8_t current_column = 0;
    for (uint8_t i = 0; i < 5; i++)
    {
        pacer_wait();

        display_column(bitmap[current_column], current_column);

        current_column++;

        if (current_column > (LEDMAT_COLS_NUM - 1))
        {
            current_column = 0;
        }
    }
}