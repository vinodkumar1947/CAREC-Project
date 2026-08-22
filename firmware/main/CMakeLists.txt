idf_component_register(
    SRCS "CAREC_main.cpp"
    INCLUDE_DIRS "." "../config"
    REQUIRES Seeed_Arduino_SSCMA esp_lcd driver
)
