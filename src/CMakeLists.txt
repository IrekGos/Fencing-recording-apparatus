add_library(score_display score_display.c)
add_library(speaker speaker.c)
add_library(gpio_wrappers gpio_wrappers.c)
add_library(time_display time_display.c)

target_link_libraries(score_display pico_stdlib hardware_spi)
target_link_libraries(speaker pico_stdlib)
target_link_libraries(gpio_wrappers pico_stdlib)
target_link_libraries(time_display pico_stdlib PicoTM1637 speaker)