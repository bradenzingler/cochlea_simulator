#ifndef AUDIO_H
#define AUDIO_H

#include "esp_log.h"
#include <stdio.h>
#include "driver/gpio.h"
#include "driver/i2s_std.h"
#include "freertos/FreeRTOS.h"
#include "esp_dsp.h"
#include "math.h"

/* Word-select pin */
#define MICROPHONE_WS_PIN GPIO_NUM_15
/* Serial clock pin */
#define MICROPHONE_SCK_PIN GPIO_NUM_16
/* Serial data pin */
#define MICROPHONE_SD_PIN GPIO_NUM_17

#define SAMPLE_RATE 44100

#define AUDIO_BUFFER_SIZE 256

void init_audio(void);

esp_err_t read_audio_data(void *buffer, size_t buffer_size, size_t *bytes_read);

void process_fft(int32_t *audio_samples, int num_samples);

#endif