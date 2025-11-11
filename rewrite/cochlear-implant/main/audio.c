#include "audio.h"

static i2s_chan_handle_t rx_handle = NULL;

__attribute__((aligned(16))) float fft_input[AUDIO_BUFFER_SIZE * 2];

__attribute((aligned(16))) float wind[AUDIO_BUFFER_SIZE];

void init_audio()
{
    esp_err_t ret = ESP_OK;

    ret = dsps_fft2r_init_fc32(NULL, CONFIG_DSP_MAX_FFT_SIZE);
    if (ret != ESP_OK)
    {
        printf("DSP initialization failed: %s\n", esp_err_to_name(ret));
        return;
    }

    // Generate Hann window coefficients once during initialization
    dsps_wind_hann_f32(wind, AUDIO_BUFFER_SIZE);

    // Create new I2S channel
    i2s_chan_config_t chan_config = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);
    ret = i2s_new_channel(&chan_config, NULL, &rx_handle);
    if (ret != ESP_OK)
    {
        printf("[AUDIO] - Failed to create I2S RX channel: %s", esp_err_to_name(ret));
        return;
    }

    i2s_std_config_t std_cfg = {
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(SAMPLE_RATE),
        .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_32BIT, I2S_SLOT_MODE_STEREO),
        .gpio_cfg = {
            .mclk = I2S_GPIO_UNUSED,
            .bclk = MICROPHONE_SCK_PIN,
            .ws = MICROPHONE_WS_PIN,
            .dout = I2S_GPIO_UNUSED,
            .din = MICROPHONE_SD_PIN,
        }};

    std_cfg.slot_cfg.slot_mask = I2S_STD_SLOT_LEFT;

    // Initialize I2S channel with standard configuration
    ret = i2s_channel_init_std_mode(rx_handle, &std_cfg);
    if (ret != ESP_OK)
    {
        printf("[AUDIO] - Failed to init I2S standard mode: %s", esp_err_to_name(ret));
        return;
    }

    // Enable the I2S channel
    ret = i2s_channel_enable(rx_handle);
    if (ret != ESP_OK)
    {
        printf("[AUDIO] - Failed to enable I2S channel: %s\n", esp_err_to_name(ret));
        return;
    }

    printf("[AUDIO] - I2S microphone initialized successfully!\n");
}

esp_err_t read_audio_data(void *buffer, size_t buffer_size, size_t *bytes_read)
{
    if (rx_handle == NULL)
    {
        printf("[AUDIO] - I2S not initialized properly\n");
        return ESP_ERR_INVALID_STATE;
    }

    return i2s_channel_read(rx_handle, buffer, buffer_size, bytes_read, portMAX_DELAY);
}

void process_fft(int32_t *audio_samples, int num_samples)
{
    if (num_samples < AUDIO_BUFFER_SIZE)
    {
        return; // Not enough samples for FFT
    }

    // Convert samples to floats and normalize
    for (int i = 0; i < AUDIO_BUFFER_SIZE; i++)
    {
        // Shift to drop lower 8 bits (24bit I2S, convert from int32 to float)
        float sample = (float)((audio_samples[i] >> 8) >> 23);
        fft_input[i << 1] = sample * wind[i];
        fft_input[(i << 1) + 1] = 0.0f; // Imaginary
    }
 
    // Perform FFT
    esp_err_t ret = dsps_fft2r_fc32(fft_input, AUDIO_BUFFER_SIZE);
    if (ret != ESP_OK)
    {
        printf("FFT failed: %s\n", esp_err_to_name(ret));
        return;
    }

    // Bit reverse the result
    dsps_bit_rev_fc32(fft_input, AUDIO_BUFFER_SIZE);

    // Convert to magnitude spectrum
    // dsps_cplx2reC_fc32(fft_input, AUDIO_BUFFER_SIZE);

    // Calculate magnitude and find dominant frequencies
    float max_pow = 0.0f;
    int peak = 0;

    for (int i = 1; i < (AUDIO_BUFFER_SIZE >> 1); i++)
    {
        float real = fft_input[i << 1];
        float imag = fft_input[(i << 1) + 1];
        float p = real * real + imag * imag;

        if (p > max_pow) {
            max_pow = p;
            peak = i;
        }
    }

    float dominant_frequency = (float)((peak * SAMPLE_RATE) >> AUDIO_BUFFER_BITS);
    printf("Frequency: %.1f Hz\n", dominant_frequency);
}
