add_executable(${CMAKE_PROJECT_NAME} app.c)

target_link_libraries(${CMAKE_PROJECT_NAME}
    pico_stdlib
    score_display
    speaker
    gpio_wrappers
    time_display)

# create map/bin/hex file etc.
pico_add_extra_outputs(${CMAKE_PROJECT_NAME})