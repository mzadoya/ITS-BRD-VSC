#include "buttons.h"
#include <stdbool.h>
#include <stdint.h>

#define MASK_R6 (0x01U << 6)

int readButtons(uint8_t eingabe, bool* fehler ) {
    if (MASK_R6 != (MASK_R6 & eingabe) ) {
        *fehler = false;
    }
    return 0;
}