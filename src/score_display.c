#include "score_display.h"

#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "pico/stdlib.h"

#define LEFT_SCORE_PIN 20
#define RIGHT_SCORE_PIN 21

const uint8_t numbers[10] = {0b00111111, 0b00000110, 0b01011011, 0b01001111,
                             0b01100110, 0b01101101, 0b01111101, 0b00000111,
                             0b01111111, 0b01101111};

volatile uint8_t left_score, right_score;

void score_display_init(void) {
    // Enable SPI at 250 kHz and connect to GPIOs
    spi_init(spi_default, 250 * 1000);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);

    gpio_init(LEFT_SCORE_PIN);
    gpio_set_dir(LEFT_SCORE_PIN, GPIO_OUT);
    gpio_init(RIGHT_SCORE_PIN);
    gpio_set_dir(RIGHT_SCORE_PIN, GPIO_OUT);
}

void reset_score(void) {
    left_score = right_score = 0;
    gpio_put(LEFT_SCORE_PIN, 1);
    gpio_put(RIGHT_SCORE_PIN, 1);
    spi_write_blocking(spi_default, &numbers[0], 1);
    spi_write_blocking(spi_default, &numbers[0], 1);
    gpio_put(LEFT_SCORE_PIN, 0);
    gpio_put(RIGHT_SCORE_PIN, 0);
}

void change_score(uint8_t player, uint8_t direction) {
    // first spi transfer set the right digit on the display
    // second transfer set the left one
    switch (player) {
        case LEFT:
            gpio_put(LEFT_SCORE_PIN, 1);
            if (direction == UP && left_score != 99)
                left_score++;
            else if (direction == DOWN && left_score != 0)
                left_score--;

            spi_write_blocking(spi_default, &numbers[left_score % 10], 1);
            spi_write_blocking(spi_default, &numbers[left_score / 10], 1);
            gpio_put(LEFT_SCORE_PIN, 0);
            break;
        case RIGHT:
            gpio_put(RIGHT_SCORE_PIN, 1);
            if (direction == UP && right_score != 99)
                right_score++;
            else if (direction == DOWN && right_score != 0)
                right_score--;
            spi_write_blocking(spi_default, &numbers[right_score % 10], 1);
            spi_write_blocking(spi_default, &numbers[right_score / 10], 1);
            gpio_put(RIGHT_SCORE_PIN, 0);
            break;
        default:
            break;
    }
}