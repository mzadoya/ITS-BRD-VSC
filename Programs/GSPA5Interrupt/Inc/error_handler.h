#ifndef BUTTONS_H
#define BUTTONS_H
#include <stdbool.h>
#include <stdint.h>

#define MSG_ENCODER_DATA_INCOSISTENT "Data Unstable"
#define MSG_ENCODER_TIME_INVLAID "Timer Error"

/**
 * @brief Behandelt den Fehlerzustand und wartet auf einen Reset durch den Nutzer.
 * 
 * @param rc Der Fehlercode, der den Fehler beschreibt.
 */
void errorState(int rc);
#endif