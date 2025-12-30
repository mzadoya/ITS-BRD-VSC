#ifndef ENCODER_INTERRUPT
#define ENCODER_INTERRUPT

#include <stdbool.h>
#include <stdint.h>

typedef struct encoderData
{
    volatile uint32_t timestamp;
    volatile int32_t ledTransitionCounter;
    volatile uint32_t encoderPosition;
    volatile bool error;
}EncoderData;
/**
 * @brief Aktualisiert den Zustand des Drehgebers anhand der aktuellen Eingangssignaler.
 * 
 *        Erkennt Phasenwechsel, bestimmt die Drehrichtung, zählt die
 *        Phasenübergaenge und berechnet daraus den Drehwinkel sowie die
 *        Winkelgeschwindigkeit in definierten Zeitfenstern.
 * 
 * @param angle Zeiger auf angle
 * @param speed Zeiger auf speed 
 * 
 * @return ENCODER_DATA_INCOSISTENT   wenn die Daten nicht eindeutig bestimmt werden koennen
 *         ENCODER_TIME_INVALID       bei der fehlerhaften Berechnung der Zeit
 *         ENCODER_TIME_UPDATED       wenn Winkelgeschwindigkeit neu berechnet wurde,
 */
int encoderUpdater(double *angle, double *speed);

/**
 * @brief Initialisiert und konfiguriert die Hardware-Interrupts für den Encoder.
 */
void initInterrupt(void);

/**
 * @brief Übergibt den aktuellen Stand des LED-Uebergangszählers an die Main-Schleife.
 * 
 * @param dataToMain Zeiger auf die Variable, in die der Zaehlerwert kopiert wird.
 */
void getEncoderLedTransitionCounter(int32_t *dataToMain);

/**
 * @brief Setzt alle Encoder-Variablen, Zähler und Zeitstempel urück.
 */
void resetEncoder(void);
#endif


