#include <stdio.h>
#include "buttons.h"
#include "mode.h"

void app_main(void)
{
    printf("Hello world!\n");

    init_buttons();
    init_mode_indicators();

    while (1)
    {

        if (read_button(&buttons[0])) {
            printf("Typical button pressed!\n");
            set_mode(MODE_TYPICAL);
        }

        if (read_button(&buttons[1])) {
            printf("Cochlear button pressed!\n");
            set_mode(MODE_COCHLEAR);
        }

        if (read_button(&buttons[2])) {
            printf("Hearing loss button pressed!\n");
            set_mode(MODE_HEARING_LOSS);
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}