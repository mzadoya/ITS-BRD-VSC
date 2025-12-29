#ifndef LED_H
#define LED_H

#include <stdint.h>

/**
 * @brief Initialisiert alle LEDs auf den Ausgangszustand.
 */
void initLed(void);

/**
 * @brief Setzt die Fehler-LED und zeigt einen Fehlerzustand an.
 */
void ledSetError(void);

/**
 * @brief Setzt die LED entsprechend der uebergebenen Drehrichtung.
 * 
 * @param direction FORWARD, BACKWARD oder IDLE
 */
void ledSetDirection(uint8_t direction);

/**
 * @brief Loescht alle Status-LED (Richtung und Fehler)
 */
void ledStatusClear(void);

/**
 * @brief Setzt die Phasen-LED entsprechend der Anzahl der durchgelaufenen Phasen
 * 
 * @param phasenCounter Anzahl der Phasenwechsel 
 */
void ledPhaseValue(uint32_t phasenCounter);

#endif