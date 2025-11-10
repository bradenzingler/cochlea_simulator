#include "mode.h"
#include "driver/gpio.h"

static system_mode_t mode = MODE_NONE;

void clear_all_leds() {
    gpio_set_level(LED_TYPICAL, 0);
    gpio_set_level(LED_COCHLEAR, 0);
    gpio_set_level(LED_HEARING_LOSS, 0);
}

void set_mode(system_mode_t new_mode)
{
    if (mode == new_mode) return;

    mode = new_mode;
    
    clear_all_leds();
    if (mode == MODE_TYPICAL)
    {
        gpio_set_level(LED_TYPICAL, 1);
    }
    else if (mode == MODE_COCHLEAR)
    {
        gpio_set_level(LED_COCHLEAR, 1);
    }
    else if (mode == MODE_HEARING_LOSS)
    {
        gpio_set_level(LED_HEARING_LOSS, 1);
    }
}

void init_mode_indicators()
{
    gpio_reset_pin(LED_TYPICAL);
    gpio_set_direction(LED_TYPICAL, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(LED_TYPICAL, GPIO_PULLDOWN_ONLY);

    gpio_reset_pin(LED_COCHLEAR);
    gpio_set_direction(LED_COCHLEAR, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(LED_COCHLEAR, GPIO_PULLDOWN_ONLY);

    gpio_reset_pin(LED_HEARING_LOSS);
    gpio_set_direction(LED_HEARING_LOSS, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(LED_HEARING_LOSS, GPIO_PULLDOWN_ONLY);

    printf("Mode indicator LEDs initialized!\n");
}