#ifndef BUTTONS_H
#define BUTTONS_H
#include <stdbool.h>
#include <stdint.h>
/**
 *  @brief Prüft den Zustand der Tasten anhand des Eingabewerts und aktualisiert
 *         den Fehlerzustand.
 *
 *  @param eingabe Aktueller Wert der Eingabe Pins (GPIO)
 *  @param fehler Zeiger auf Fehlerzustand, wird entsprechend angepasst
 *
 *  @return OK
 */
int readButtons(uint8_t eingabe, bool *fehler);
#endif