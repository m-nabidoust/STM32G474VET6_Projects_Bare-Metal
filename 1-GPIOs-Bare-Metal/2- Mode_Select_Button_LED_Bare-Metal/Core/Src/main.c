#include "stm32g474xx.h"

void delay(volatile uint32_t s) {
    for (; s > 0; s--);
}

int main(void) {
    //  RCC_AHB2ENR
    RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOEEN | RCC_AHB2ENR_GPIOFEN);

    //  (Input Mode: 00)
    GPIOE->MODER &= ~(3U << (0 * 2));

    GPIOE->PUPDR |= (1U << (0 * 2));

    //  (Output Mode: 01)
    GPIOF->MODER &= ~((3U << (2 * 2)) | (3U << (9 * 2)) | (3U << (10 * 2)));
    GPIOF->MODER |=  ((1U << (2 * 2)) | (1U << (9 * 2)) | (1U << (10 * 2)));

    uint8_t mode = 0;

    while (1) {

        if (!(GPIOE->IDR & (1U << 0))) {
            delay(100000); // Debounce

            if (!(GPIOE->IDR & (1U << 0))) {
                mode++;
                if (mode > 3) mode = 0;


                GPIOF->BSRR = (1U << (2 + 16)) | (1U << (9 + 16)) | (1U << (10 + 16));

                if (mode == 1)      GPIOF->BSRR = (1U << 9);
                else if (mode == 2) GPIOF->BSRR = (1U << 2);
                else if (mode == 3) GPIOF->BSRR = (1U << 10);

                while (!(GPIOE->IDR & (1U << 0)));
                delay(100000);
            }
        }
    }
}
