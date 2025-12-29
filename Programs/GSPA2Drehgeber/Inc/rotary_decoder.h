#ifndef ROTARY_DECODER_H
#define ROTARY_DECODER_H

#include <stdint.h>

extern int32_t transitionCount;
/**
 * @brief Initialisiert den Drehgeber-Dekoder und setzt alle internen Zustände zurück.
 */
void initDecoder(void);

/**
 * @brief Aktualisiert den Zustand des Drehgebers anhand der aktuellen Eingangssignaler.
 * 
 *        Erkennt Phasenwechsel, bestimmt die Drehrichtung, zählt die
 *        Phasenübergänge und berechnet daraus den Drehwinkel sowie die
 *        Winkelgeschwindigkeit in definierten Zeitfenstern.
 * 
 * @param gpioState Aktueller Zustand der Eingangspins 
 * @param direction Zeiger auf erkannte Drehrichtung 
 * @param time2 Aktoeller Zeitstempel des Timers
 * 
 * @return ENCODER_NO_CHANGE     wenn kein Phasenwechsel erkannt wurde,
 *         ENCODER_CHANGED       bei gültigem Phasenwechsel,
 *         ENCODER_TIME_UPDATED  wenn Winkelgeschwindigkeit neu berechnet wurde,
 *         ENCODER_ERROR         im Fehlerfall
 */
int encoderUpdate(uint16_t gpioState, uint16_t *direction, uint32_t time2);
extern double speed ;
extern double angle;

#endif