add_executable(${CMAKE_PROJECT_NAME} app.c)

target_link_libraries(${CMAKE_PROJECT_NAME} pico_stdlib score_display)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/include"
                          )

# create map/bin/hex file etc.
pico_add_extra_outputs(${CMAKE_PROJECT_NAME})