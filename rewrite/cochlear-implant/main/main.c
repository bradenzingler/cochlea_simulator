#include <stdio.h>
#include "buttons.h"
#include "mode.h"
#include "audio.h"
#include "math.h"

void app_main(void)
{
    init_buttons();
    init_mode_indicators();
    init_audio();

    int32_t audio_buffer[AUDIO_BUFFER_SIZE];
    size_t bytes_read;
    static int fft_count = 0;

    while (1)
    {

        if (read_button(&buttons[0]))
        {
            printf("Typical button pressed!\n");
            set_mode(MODE_TYPICAL);
        }

        if (read_button(&buttons[1]))
        {
            printf("Cochlear button pressed!\n");
            set_mode(MODE_COCHLEAR);
        }

        if (read_button(&buttons[2]))
        {
            printf("Hearing loss button pressed!\n");
            set_mode(MODE_HEARING_LOSS);
        }

        esp_err_t ret = read_audio_data(audio_buffer, sizeof(audio_buffer), &bytes_read);
        if (ret != ESP_OK || bytes_read <= 0)
        {
            printf("Error reading audio: %s\n", esp_err_to_name(ret));
            continue;
        }

        int16_t samples_read = bytes_read / sizeof(int16_t);
        // uint64_t sum_squares = 0;
        // for (int i = 0; i < samples_read; i++)
        // {
        //     sum_squares += (uint64_t)(audio_buffer[i] * audio_buffer[i]);
        // }
        // uint32_t rms = sqrt(sum_squares / samples_read);
        process_fft(audio_buffer, samples_read);

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}