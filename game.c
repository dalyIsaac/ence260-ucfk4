#include "system.h"
#include "pio.h"
#include "pacer.h"
#include "board.h"

static const uint8_t bitmap[] = {0x30, 0x46, 0x40, 0x46, 0x30};

int main(void)
{
    uint8_t current_column = 0;

    system_init();
    pacer_init(500);

    /* Initialise LED matrix pins.  */
    board_init();

    while (1)
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
