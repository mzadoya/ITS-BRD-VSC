#include "sensor.h"
#include "global.h"
#include "onewire.h"
#include "errCodes.h"
#include "crc.h"
#include "stm32f429xx.h"
#include "timing.h"
#include <stdint.h>

#define PUSH_PULL_DURATION 750000
uint64_t registration = 0;
unsigned char crcPack[8];
unsigned char tempData[9];

int sensorReadRom(){
    registration = 0;
    uint8_t byte = 0;
    int rc = OK;
    rc = oneWireReset(); 
    if (rc != OK) {
        return rc;
    }
    oneWireWriteByte(0x33);
    for (int i = 0; i < 8; i++) {
        rc = oneWireReadByte(&byte);
        if (rc!=OK) {
            return rc;
        }
        uint64_t temp = byte;
        registration = registration | (temp << (8*i));
        crcPack[i] = (unsigned char) byte;
    }
    if (checkCRC(sizeof(crcPack), crcPack) == false) {
        return ERR_BIT_FLIPPED;
    }
    return rc;
} 

int sensorSelect() {
    oneWireReset();
    int rc = OK;
    oneWireWriteByte(0x55);
    for (int i = 0; i < 8; i++) {
        rc = oneWireWriteByte((uint8_t)((registration >> (8*i)))); //aendern wenn mehrere sensoren betrachtet werden sollen
        if (rc!=OK) {
            return rc;
        }
    }
    return OK;
}

void sensorFullThrottle() {
    GPIOD->OTYPER &= ~PD0_MASK;
    GPIOD->BSRR = PD0_MASK;
    wait(PUSH_PULL_DURATION);
    GPIOD->OTYPER |= PD0_MASK;
}

int sensorReadData() {
    uint8_t byte = 0;
    int rc = OK;
    oneWireWriteByte(0xBE);
    for (int i = 0; i < 9; i++) {
        rc = oneWireReadByte(&byte);
        if (rc!=OK) {
            return rc;
        }
        tempData[i] = (unsigned char) byte; 
    }
    return OK;
}

int sensorReadTemperature() {
    int rc = OK;

   rc = sensorSelect();
   if (rc!=OK) {
    return rc;
   }

   sensorFullThrottle();

   rc = sensorSelect();
   if (rc!=OK) {
    return rc;
   }

   sensorReadData();
   return rc;
}