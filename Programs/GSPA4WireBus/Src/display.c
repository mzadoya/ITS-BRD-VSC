#include "display.h"
#include "LCD_GUI.h"
#include "lcd.h"
#include <stdio.h>
#define MAX_TEMP_STR_SIZE 10
#define MAX_ID_STR_SIZE 18

void initDisplay() {
    lcdGotoXY(0, 0);
    lcdPrintS("Sensor");
    lcdGotoXY(9, 0);
    lcdPrintS("PDROM");
    lcdGotoXY(30,  0);
    lcdPrintS("Temp. [C]");
    
};
void drawInfo(char* sensorName, uint64_t ID, int sensorNumber) {
    int y = 2*sensorNumber;
    char textID[MAX_ID_STR_SIZE];
    lcdGotoXY(0, y);
    lcdPrintS(sensorName);
    lcdGotoXY(9, y);
    snprintf(textID, MAX_ID_STR_SIZE, "0x%llX", ID);
    lcdPrintS(textID);
}

void lcdPrintLong(uint64_t ID){

}

void drawTemperature(float temp, int sensorNumber) {
    char textTemp[MAX_TEMP_STR_SIZE];
    int y = 2*sensorNumber;
    lcdGotoXY(30, y);
    snprintf(textTemp, MAX_TEMP_STR_SIZE, "%.5f", temp);
    lcdPrintS(textTemp);
}