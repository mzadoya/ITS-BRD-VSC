#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>
typedef struct sensor
{
    int16_t temperature; 
    int8_t th;
    int8_t tl;
    int8_t config;
    int8_t reserved;
    int8_t countRemain;
    int8_t countPerC;
    int8_t crc; 
} __attribute__((__packed__)) SensorRawData;

typedef struct temp {
    uint64_t romID; 
    SensorRawData raw;
    float valueCelcius;
}__attribute__((__packed__)) Thermometer;

int sensorReadRom();
int sensorGetTemperature();

#endif