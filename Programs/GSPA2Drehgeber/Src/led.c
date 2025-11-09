#include "led.h"
#include <stdint.h>
#include "global.h"
#include "stm32f4xx_hal.h"

#define MASK_LED_FORWARD  (0x01U << 7)
#define MASK_LED_BACKWARD (0x01U << 6)
#define MASK_LED_IDLE 0x00U
#define MASK_LED_ERROR (0x01U << 5)
#define MASK_INIT 0xFFU 

void initLed() {
     GPIOD->BSRR = MASK_INIT << 16;
     GPIOF->BSRR = MASK_INIT << 16;
}

void ledSetError() {
    ledClear();
     GPIOE->BSRR = MASK_LED_ERROR;
}

void ledSetDirection(uint8_t direction) {
    if (direction == FORWARD) {
        ledClear();
        GPIOE->BSRR = MASK_LED_FORWARD;
    }
    else if (direction == BACKWARD) {
        ledClear();
        GPIOE->BSRR = MASK_LED_BACKWARD;
    }
    
    else if (direction == IDLE) {
        ledClear();
    }
}

void ledClear() {
    GPIOE->BSRR = ((MASK_LED_ERROR << 16) |  (MASK_LED_BACKWARD << 16) |  (MASK_LED_FORWARD << 16));
}