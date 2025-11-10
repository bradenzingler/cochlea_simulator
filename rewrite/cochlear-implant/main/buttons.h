#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdbool.h>
#include <stdint.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define BTN_TYPICAL GPIO_NUM_11
#define BTN_COCHLEAR GPIO_NUM_12
#define BTN_HEARING_LOSS GPIO_NUM_13
#define DEBOUNCE_TIME 5 // ticks
#define NUM_BUTTONS 3

typedef struct {
    gpio_num_t gpio;
    uint32_t last_state;
    uint32_t last_press_time;
    bool pressed;
} button_t;

extern button_t buttons[];

void init_buttons(void);
bool read_button(button_t *btn);

#endif