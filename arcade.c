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

static char event_str[128];
static uint32_t valid_events[] = {GPIO_IRQ_LEVEL_LOW, GPIO_IRQ_LEVEL_HIGH, GPIO_IRQ_EDGE_FALL, GPIO_IRQ_EDGE_RISE};
static uint32_t gpio_pin_values = 0;
static uint8_t event_detected = 0;

static char button_name[][10] = {"0", "1", "START", "A", "X", "L", "SELECT", "B", "Y", "R"};

void gpio_event_string(char *buf, uint32_t events, uint gpio);

void gpio_callback(uint gpio, uint32_t events) {
    gpio_event_string(event_str, events, gpio);
    printf("GPIO %d-%s %s\n", gpio, button_name[gpio], event_str);
    event_detected = 1;
}

int main() 
{
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    uint led_state = 1;

    const uint BUTTON_A = 3;
    const uint BUTTON_B = 7;
    const uint BUTTON_X = 4;
    const uint BUTTON_Y = 8;
    const uint BUTTON_L = 5;
    const uint BUTTON_R = 9;
    const uint BUTTON_START = 2;
    const uint BUTTON_SELECT = 6;

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
        if (event_detected)
        {
            printf("BUTTON A %d | B %d | X %d | Y %d | L %d | R %d | START %d | SELECT %d \n",
                    (gpio_pin_values & BUTTON_A_MASK) == 0,
                    (gpio_pin_values & BUTTON_B_MASK) == 0,
                    (gpio_pin_values & BUTTON_X_MASK) == 0,
                    (gpio_pin_values & BUTTON_Y_MASK) == 0,
                    (gpio_pin_values & BUTTON_L_MASK) == 0,
                    (gpio_pin_values & BUTTON_R_MASK) == 0,
                    (gpio_pin_values & BUTTON_START_MASK) == 0,
                    (gpio_pin_values & BUTTON_SELECT_MASK) == 0);
            printf("--------------------\n\n");
            event_detected = 0;

        }

        gpio_put(LED_PIN, led_state);
        if (led_state == 1)
            led_state = 0;
        else
            led_state = 1;

        sleep_ms(100);
    }
}

void start_gpio(uint pin) {
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    gpio_pull_up(pin);
    gpio_set_irq_enabled_with_callback(pin, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
}

static const char *gpio_irq_str[] = {
        "LEVEL_LOW",  // 0x1
        "LEVEL_HIGH", // 0x2
        "EDGE_FALL",  // 0x4
        "EDGE_RISE"   // 0x8
};

void gpio_event_string(char *buf, uint32_t events, uint gpio) {
    for (uint i = 0; i < 4; i++) {
        uint mask = (1 << i);
        if (events & mask) {
            if (mask == GPIO_IRQ_EDGE_FALL)
            {
                gpio_pin_values &= ~(1 << gpio);
            }
            else if (mask == GPIO_IRQ_EDGE_RISE)
            {
                gpio_pin_values |= (1 << gpio);
            }
            // Copy this event string into the user string
            const char *event_str = gpio_irq_str[i];
            while (*event_str != '\0') {
                *buf++ = *event_str++;
            }
            events &= ~mask;

            // If more events add ", "
            if (events) {
                *buf++ = ',';
                *buf++ = ' ';
            }
        }
    }
    *buf++ = '\0';
}