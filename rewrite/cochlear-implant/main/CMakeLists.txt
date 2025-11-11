idf_component_register(SRCS "main.c" "buttons.c" "mode.c" "audio.c"
                       PRIV_REQUIRES esp_driver_gpio esp_driver_i2s spi_flash esp-dsp
                       INCLUDE_DIRS "")
