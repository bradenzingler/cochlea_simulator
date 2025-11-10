idf_component_register(SRCS "main.c" "buttons.c" "mode.c"
                       PRIV_REQUIRES esp_driver_gpio spi_flash
                       INCLUDE_DIRS "")
