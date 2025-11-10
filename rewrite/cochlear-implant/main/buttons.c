#include "buttons.h"

button_t buttons[] = {
    {BTN_TYPICAL, 1, 0, false},      // Use false for bool
    {BTN_COCHLEAR, 1, 0, false},    
    {BTN_HEARING_LOSS, 1, 0, false},
};

void init_buttons() {
    for (int i = 0; i < NUM_BUTTONS; i++) {
        gpio_reset_pin(buttons[i].gpio);
        gpio_set_direction(buttons[i].gpio, GPIO_MODE_INPUT);
        gpio_set_pull_mode(buttons[i].gpio, GPIO_PULLUP_ONLY);
        
        // Read and set initial state
        buttons[i].last_state = gpio_get_level(buttons[i].gpio);
        buttons[i].last_press_time = xTaskGetTickCount();
    }
    printf("Buttons initialized!\n");
}

bool read_button(button_t *btn) {
    uint32_t current_state = gpio_get_level(btn->gpio);
    uint32_t current_time = xTaskGetTickCount();

    // Active-low buttons (pressed when GPIO reads 0)
    bool is_pressed = (current_state == 0);

    // Check for state change
    if (current_state != btn->last_state) {
        btn->last_press_time = current_time;
        btn->last_state = current_state;
    }

    // Debounce logic
    if ((current_time - btn->last_press_time) > DEBOUNCE_TIME) {
        if (is_pressed && !btn->pressed) {
            btn->pressed = true;
            return true;
        } else if (!is_pressed && btn->pressed) {
            btn->pressed = false;
            return false;
        }
    }

    return false;
}