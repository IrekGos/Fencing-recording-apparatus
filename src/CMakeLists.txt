add_library(score_display score_display.c)
add_library(speaker speaker.c)

target_link_libraries(score_display pico_stdlib hardware_spi)
target_link_libraries(speaker pico_stdlib)