/**
 ******************************************************************************
 * @file    display.c
 * @author  Maksym Zadoya
 * @brief   Ausgabe von Messwerten und Fehlermeldungen auf dem LCD-Display.
 *          Initialisiert das Display und gibt Drehwinkel sowie
 *          Winkelgeschwindigkeit zeichenweise aus.
 *          
 * @date    2025/12/30
 * @version 1.0 vom 2025/12/29
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/

// Standard C Libs
#include <stdio.h>

// Board Libs
#include "LCD_GUI.h"

// App modules
#include "display.h"
#include "lcd.h"
#include "global.h"
#include "fehler.h"


#define WEISS 0xFFFF

void displayClear(void) {
    GUI_clear(WEISS);
}
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

/**
 * @brief Gibt ein Zeichen an der angegebenen Pos aus
 * 
 * @param x Position in X Richtung
 * @param y Position in Y Richtung
 * @param pos Zeiger auf die aktuelle Zeichenposition im String
 * @param str Zu druckender String 
 */
static void convertValue(int x, int y, int *pos, char* str) {
    lcdGotoXY(x + *pos, y);
    lcdPrintC(str[*pos]);
    (*pos)++;
    if(*pos == 12)
    {
        *pos = 0;
    }
}

/**
 * @brief Gibt einen Gleitkommawert zeichenweise aufm Displaty aus.
 * 
 * @param toDisplay Wert der ausgegeben werden muss
 * @param x Position in X Richtung
 * @param y Position in Y Richtung
 * 
 * @return PRINTED wenn der Wert komplett ausgegeben wurde,
 *         PRINTING solange die Ausgabe noch laeuft
 *         OK sonst
 */
static int lcdValuePrinter(double toDisplay, int x, int y) {
  static int pos = 0;
  static char str[VALUE_MAX_SIZE];
  int rc = OK;
  if (pos == 0) {
    snprintf(str, VALUE_MAX_SIZE, "%12.1f", toDisplay);
  }
  convertValue(x, y, &pos, str);
  if (pos == 0) {
    return PRINTED;
  } 
  else {
    return PRINTING;
  }
  return OK;
}
int displayPrintHandler(double angle, double speed) {

    static int currentPrinter = 0;
    double toDisplay;
    int y;
    if (currentPrinter == 0) {
        toDisplay = angle;
        y = 2;
    }
    else if (currentPrinter == 1) {
        toDisplay = speed;
        y = 6;
    }
    int rc = lcdValuePrinter(toDisplay, 16, y);

    if (rc == PRINTED) {
        if (currentPrinter== 1) {
            currentPrinter = 0;
            return COMPLETED;
        }

        else if (currentPrinter== 0) {
            currentPrinter++;
        }
    }
    return OK;
}

void displayPrintError(char* errorMsg) {
    lcdGotoXY(1, 9);
    lcdPrintS (errorMsg);
    lcdGotoXY(1, 12);
    lcdPrintS("S6 druecken zum Bestaetigen");
}
//EOF

void debugLowSpeed(uint32_t lEC, uint32_t encoderPos) {
     char buffer[50]; 
     lcdGotoXY(1, 9);
     lcdPrintInt(lEC);
     lcdGotoXY(1,  12);
     lcdPrintInt(encoderPos);
}