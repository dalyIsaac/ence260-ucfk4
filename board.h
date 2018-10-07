#include "system.h"

void board_init(void);

void display_column(uint8_t current_column);

void display_board(void);

typedef struct puck_s
{
    uint8_t old_top;
    uint8_t old_bottom;
    uint8_t new_top;
    uint8_t new_bottom;
} Puck;
