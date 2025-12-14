#include "ds18x20Common.h"

#define PUSH_PULL_DURATION 750000

uint64_t registration = 0;

void sensorFullThrottle() {
    GPIOD->OTYPER &= ~PD0_MASK;
    GPIOD->BSRR = PD0_MASK;
    wait(PUSH_PULL_DURATION);
    GPIOD->OTYPER |= PD0_MASK;
}

int sensorSelect(uint64_t targetID) {
    oneWireReset();
    int rc = OK;
    oneWireWriteByte(0x55);
    for (int i = 0; i < 8; i++) {
        rc = oneWireWriteByte((uint8_t)((targetID >> (8*i)))); //aendern wenn mehrere sensoren betrachtet werden sollen
        if (rc!=OK) {
            return rc;
        }
    }
    return OK;

}



