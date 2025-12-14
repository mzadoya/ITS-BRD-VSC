#include "global.h"
#include "lcd.h"
#include "onewire.h"
#include "errCodes.h"
#include "crc.h"
#include "sensorDS18B20.h"
#include "stm32f429xx.h"
#include "timing.h"
#include <stdint.h>
#include "math.h"
#include "ds18x20Common.h"
#include "display.h"

unsigned char crcPackDS18B20[8];
unsigned char tempDataDS18B20[9];

ThermometerDS18B20 DS18B20;

int sensorDS18B20ReadRom(){
    DS18B20.romID = 0;
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
        DS18B20.romID = DS18B20.romID | (temp << (8*i));
        crcPackDS18B20[i] = (unsigned char) byte;
    }
    if (checkCRC(sizeof(crcPackDS18B20), crcPackDS18B20) == false) {
        return ERR_BIT_FLIPPED;
    }

    drawInfo("DS18B20", DS18B20.romID, 1); 

    return rc;
} 

int sensorDS18B20ParseData() {
    DS18B20.raw.temperature = tempDataDS18B20[0] | tempDataDS18B20[1] << 8;
    DS18B20.raw.th = tempDataDS18B20[2];
    DS18B20.raw.tl = tempDataDS18B20[3];
    DS18B20.raw.config = tempDataDS18B20[4];
    DS18B20.raw.reserved1 = tempDataDS18B20[5];
    DS18B20.raw.reserved2 = tempDataDS18B20[6];
    DS18B20.raw.reserved3 = tempDataDS18B20[7];
    DS18B20.raw.crc = tempDataDS18B20[8];
    return OK;
}

int sensorDS18B20ReadData() {
    uint8_t byte = 0;
    int rc = OK;
    oneWireWriteByte(0xBE);
    for (int i = 0; i < 9; i++) {
        rc = oneWireReadByte(&byte);
        if (rc!=OK) {
            return rc;
        }
        tempDataDS18B20[i] = (unsigned char) byte; 
    }

    if (checkCRC(sizeof(tempDataDS18B20), tempDataDS18B20) == false) {
        return ERR_BIT_FLIPPED;
    }
    rc = sensorDS18B20ParseData();
    return rc;
}

int sensorDS18B20GetTemperature() {
    
    int rc = OK;

   rc = sensorSelect(DS18B20.romID);
   if (rc!=OK) {
    return rc;
   }
   oneWireWriteByte(0x44);

   sensorFullThrottle();

   rc = sensorSelect(DS18B20.romID);
   if (rc!=OK) {
    return rc;
   }

   rc = sensorDS18B20ReadData();
   if (rc!=OK) {
    return rc;
   }

   rc = tempCalcCelciusDS18B20(&DS18B20);
    drawTemperature(DS18B20.valueCelcius, 1);   
   return rc;
}

