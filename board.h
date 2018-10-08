#include "system.h"

/**
 * @brief The column which the puck resides in
 */
#define PUCK_COL 4

typedef struct puck_s
{
    int8_t old_bottom;
    int8_t old_top;
    int8_t new_bottom;
    int8_t new_top;
} Puck;

typedef enum nav_movement
{
    up = 1,
    down = -1
} NavMovement;

void board_init(void);

void display_column(uint8_t current_column);

void display_board(void);

void puck_update(NavMovement change);

void puck_task(void);