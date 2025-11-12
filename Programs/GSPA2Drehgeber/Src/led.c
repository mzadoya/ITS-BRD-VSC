#include "led.h"
#include <stdint.h>
#include "global.h"
#include "stm32f4xx_hal.h"

#define MASK_LED_FORWARD  (0x01U << 7)
#define MASK_LED_BACKWARD (0x01U << 6)
#define MASK_LED_IDLE 0x00U
#define MASK_LED_ERROR (0x01U << 5)
#define MASK_INIT 0xFFU 
#define MASK_LED_PHASE 0xFFU

void initLed(void) {
     GPIOD->BSRR = MASK_INIT << 16;
     GPIOE->BSRR = MASK_INIT << 16;
}

void ledSetError(void) {
    ledStatusClear();
     GPIOE->BSRR = MASK_LED_ERROR;
}

void ledSetDirection(uint8_t direction) {
    if (direction == FORWARD) {
        ledStatusClear();
        GPIOE->BSRR = MASK_LED_FORWARD;
    }
    else if (direction == BACKWARD) {
        ledStatusClear();
        GPIOE->BSRR = MASK_LED_BACKWARD;
    }
    
    else if (direction == IDLE) {
        ledStatusClear();
    }
}

void ledStatusClear(void) {
    GPIOE->BSRR = ((MASK_LED_ERROR << 16) |  (MASK_LED_BACKWARD << 16) |  (MASK_LED_FORWARD << 16));
}

void ledPhaseCountClear(void) {
     GPIOD->BSRR = MASK_INIT << 16;
}

void ledSetPhaseCount(uint32_t phasenCounter) { //TODO sinnvole Name
    ledPhaseCountClear();
    GPIOD->BSRR = (MASK_LED_PHASE & phasenCounter);
}