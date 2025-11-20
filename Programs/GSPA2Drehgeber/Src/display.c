#include "display.h"
#include "LCD_GUI.h"
#include "lcd.h"
#include "global.h"
#include "converter.h"
#include <stdio.h>

void initDisplay(void) {
    lcdGotoXY(0, 2);
    lcdPrintS("Winkel:");
    lcdGotoXY(33, 2);
    lcdPrintS("GRAD");
    lcdGotoXY(0, 6);
    lcdPrintS("Geschwindigkeit:");
    lcdGotoXY(33, 6);
    lcdPrintS("GRAD/S");
}

void convertValue(int x, int y, int *pos, char* str) {
    lcdGotoXY(x + *pos, y);
    lcdPrintC(str[*pos]);
    (*pos)++;
    if(*pos == 12)
    {
        *pos = 0;
    }
}

int printAngle(double angle) {
    static int pos = 0;
    static char str[VALUE_MAX_SIZE];
    if(pos == 0)
    {
        snprintf(str, VALUE_MAX_SIZE, "%12.1f", angle);
    }

    convertValue(16, 2, &pos, str);
    return 0;
}
int printAngularVelocity(double speed) {
    static int pos = 0;
    static char str[VALUE_MAX_SIZE];
    if(pos == 0)
    {
        snprintf(str, VALUE_MAX_SIZE, "%12.1f", speed);
    }
    convertValue(16, 6, &pos, str);
    return 0;
}