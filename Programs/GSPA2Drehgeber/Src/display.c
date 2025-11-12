#include "display.h"
#include "LCD_GUI.h"
#include "lcd.h"
#include "global.h"
#include "converter.h"

void initDisplay(void) {
    lcdGotoXY(0, 2);
    lcdPrintS("Winkel:");
    lcdGotoXY(0, 6);
    lcdPrintS("Geschwindigkeit:");
}

int printAngle(double angle) {
    char str[VALUE_MAX_SIZE];
    lcdGotoXY(18, 2);
    //convertDoubleValue(str, angle);
    //lcdPrintReplS(str);
    lcdPrintInt(angle);
    return 0;
}
int printAngularVelocity(double speed) {
    char str[VALUE_MAX_SIZE];
    lcdGotoXY(18, 6);
    //convertDoubleValue(str, speed);
    //lcdPrintReplS(str);
    lcdPrintInt(speed);
    return 0;
}