/*
MIT License

Copyright (c) 2022 David González Filoso

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

void start_gpio(uint pin);

int main() 
{
#ifndef PICO_DEFAULT_LED_PIN
#warning blink example requires a board with a regular LED
#else
    uint32_t gpio_pin_values;

    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    uint led_state = 1;

    const uint BUTTON_A = 2;
    const uint BUTTON_B = 3;
    const uint BUTTON_X = 4;
    const uint BUTTON_Y = 5;
    const uint BUTTON_L = 6;
    const uint BUTTON_R = 7;
    const uint BUTTON_START = 8;
    const uint BUTTON_SELECT = 9;

    const uint32_t BUTTON_A_MASK = 1 << BUTTON_A;
    const uint32_t BUTTON_B_MASK = 1 << BUTTON_B;
    const uint32_t BUTTON_X_MASK = 1 << BUTTON_X;
    const uint32_t BUTTON_Y_MASK = 1 << BUTTON_Y;
    const uint32_t BUTTON_L_MASK = 1 << BUTTON_L;
    const uint32_t BUTTON_R_MASK = 1 << BUTTON_R;
    const uint32_t BUTTON_START_MASK = 1 << BUTTON_START;
    const uint32_t BUTTON_SELECT_MASK = 1 << BUTTON_SELECT;

    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    printf("Welcome to the Arcade controller\n");
    gpio_put(LED_PIN, led_state);

    start_gpio(BUTTON_A);
    start_gpio(BUTTON_B);
    start_gpio(BUTTON_X);
    start_gpio(BUTTON_Y);
    start_gpio(BUTTON_L);
    start_gpio(BUTTON_R);
    start_gpio(BUTTON_START);
    start_gpio(BUTTON_SELECT);

    while(1)
    {
        gpio_pin_values = gpio_get_all();

        printf("GPIOs State : 0x%08X\n", gpio_pin_values);
        printf("BUTTON A : %d\n", (gpio_pin_values & BUTTON_A_MASK) == 0);
        printf("BUTTON B : %d\n", (gpio_pin_values & BUTTON_B_MASK) == 0);
        printf("BUTTON X : %d\n", (gpio_pin_values & BUTTON_X_MASK) == 0);
        printf("BUTTON Y : %d\n", (gpio_pin_values & BUTTON_Y_MASK) == 0);
        printf("BUTTON L : %d\n", (gpio_pin_values & BUTTON_L_MASK) == 0);
        printf("BUTTON R : %d\n", (gpio_pin_values & BUTTON_R_MASK) == 0);
        printf("BUTTON START : %d\n", (gpio_pin_values & BUTTON_START_MASK) == 0);
        printf("BUTTON SELECT : %d\n", (gpio_pin_values & BUTTON_SELECT_MASK) == 0);
        printf("--------------------\n\n");

        gpio_put(LED_PIN, led_state);
        if (led_state == 1)
            led_state = 0;
        else
            led_state = 1;

        sleep_ms(500);
    }
#endif
}

void start_gpio(uint pin) {
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    gpio_pull_up(pin);
}