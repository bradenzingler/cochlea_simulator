#include <stdio.h>
#include "buttons.h"
#include "mode.h"
#include "audio.h"
#include "math.h"

static void fft_task(void *arg)
{
    static int32_t audio_buffer[AUDIO_BUFFER_SIZE];
    size_t bytes_read;
    while (1)
    {
        esp_err_t ret = read_audio_data(audio_buffer, sizeof(audio_buffer), &bytes_read);
        if (ret != ESP_OK || bytes_read <= 0)
        {
            printf("Error reading audio: %s\n", esp_err_to_name(ret));
            continue;
        }
        int samples_read = bytes_read / sizeof(int32_t);
        if (samples_read >= AUDIO_BUFFER_SIZE) {
            process_fft(audio_buffer, samples_read);
        }
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

void app_main(void)
{
    init_buttons();
    init_mode_indicators();
    init_audio();

    BaseType_t ok = xTaskCreatePinnedToCore(fft_task, "fft_task", 8192, NULL, 5, NULL, 0);
    if (ok != pdPASS) {
        printf("Failed to create fft_task\n");
    }

    while (1)
    {
        if (read_button(&buttons[0]))
            set_mode(MODE_TYPICAL);
        if (read_button(&buttons[1]))
            set_mode(MODE_COCHLEAR);
        if (read_button(&buttons[2]))
            set_mode(MODE_HEARING_LOSS);

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}