#include "pico/stdlib.h"

int main() {
    gpio_init(15);
    gpio_set_dir(15, GPIO_OUT);
    while(true) {
        gpio_put(15, 1);
        sleep_ms(1500);
        gpio_put(15,0);
        sleep_ms(500);
        gpio_put(15, 1);
        sleep_ms(100);
        gpio_put(15,0);
        sleep_ms(500);
    }
}
