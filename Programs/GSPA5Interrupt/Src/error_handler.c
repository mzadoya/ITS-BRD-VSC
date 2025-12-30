/**
 ******************************************************************************
 * @file    error_handler.c
 * @author  Maksym Zadoya
 * @brief   Modul für Fehlermeldungen und den Reset-Taster.
 *          Zeigt Fehler auf dem Display an und wartet, bis der Nutzer 
 *          S7 (R6) drückt, um das System neu zu starten.
 * @date    2025/12/30
 * @version 1.0 vom 2025/12/30
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/

// Standard C Libs
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

// App modules
#include "encoder_interrupt.h"
#include "error_handler.h"
#include "fehler.h"
#include "stm32f429xx.h"
#include "display.h"

#define MASK_R6 (0x01U << 6)
#define MSG_SIZE 32

/**
 *  @brief Prüft den Zustand der Tasten anhand des Eingabewerts und aktualisiert
 *         den Fehlerzustand.
 *
 *  @param eingabe Aktueller Wert der Eingabe Pins (GPIO)
 *  @param fehler Zeiger auf Fehlerzustand, wird entsprechend angepasst
 *
 *  @return OK
 */
static int readButtons(bool* fehler ) {
    if (MASK_R6 != (MASK_R6 & GPIOF->IDR) ) {
        *fehler = false;
    }
    return OK;
}

/**
 *  @brief Wählt eine Fehlermeldung basierend auf dem Fehlercode aus.
 * 
 *  @param errorMsg Zielpuffer für die Nachricht. 
 *  @param rc Der Fehlercode (Return Code).
 */
static void takeErrorMsg(char *errorMsg, int rc) {
    
    switch (rc)
    {
    case ENCODER_TIME_INVALID:
        strncpy(errorMsg, MSG_ENCODER_TIME_INVLAID, MSG_SIZE);  
        break;
    case ENCODER_DATA_INCOSISTENT:
        strncpy(errorMsg, MSG_ENCODER_DATA_INCOSISTENT, MSG_SIZE);
        break;
    default:
       {
        char unknownErr[] = "Unknown Error";
        for (int i = 0; i < strlen(unknownErr); i++) {
            errorMsg[i] = unknownErr[i];
        }
        errorMsg[strlen(unknownErr)] = '\0';
        break;
    }
    }
}


void errorState(int rc) {
    char errorMsg[MSG_SIZE];

    takeErrorMsg(errorMsg, rc);

    displayPrintError(errorMsg);
    bool fehler = true;
    while (fehler == true) {
        readButtons(&fehler);
    }
    displayClear();
    initDisplay();
    resetEncoder();
}

//EOF