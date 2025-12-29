/**
 ******************************************************************************
 * @file    buttons.c
 * @author  Maksym Zadoya
 * @brief   Auswertung der Tastereingänge des ITS-Boards.
 * @date    2025/11/20
 * @version 1.1 vom 2025/11/20
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/

// Standard C Libs
#include <stdbool.h>
#include <stdint.h>

// App modules
#include "buttons.h"
#include "fehler.h"

#define MASK_R6 (0x01U << 6)

int readButtons(uint8_t eingabe, bool* fehler ) {
    if (MASK_R6 != (MASK_R6 & eingabe) ) {
        *fehler = false;
    }
    return OK;
}

//EOF