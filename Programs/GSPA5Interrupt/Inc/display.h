#ifndef DISPLAY_H
#define DISPLAY_H
#include <stdbool.h>
#include <stdint.h>
/**
 * @brief Initialisiert die statischen Texte des LCD-Displays
 */
void initDisplay(void);

/**
 * @brief Steuert die Ausgabe von Winkel und Winkelgeschwindigkeit. 
 * 
 * @param angle Aktueller Drehwinkel in Grad 
 * @param speed Aktuelle Winkelgeschwindigkeit in Grd pro Sekunde
 * 
 * @return COMPLETED wenn beide Werte vollstaendig asugegeben wurden
 *         OK sonst 
 */
int displayPrintHandler(double angle, double speed);
void displayPrintError(char *errorMsg);
void displayClear(void);
#endif