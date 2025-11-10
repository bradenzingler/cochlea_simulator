#ifndef MODE_H
#define MODE_H

#include <esp_mac.h>

typedef enum {
    MODE_NONE = 0,
    MODE_TYPICAL,
    MODE_COCHLEAR,
    MODE_HEARING_LOSS
} system_mode_t;

#define LED_TYPICAL GPIO_NUM_4
#define LED_COCHLEAR GPIO_NUM_5
#define LED_HEARING_LOSS GPIO_NUM_6
#define NUM_INDICATOR_LEDS 3

void set_mode(system_mode_t new_mode);
void init_mode_indicators(void);

#endif