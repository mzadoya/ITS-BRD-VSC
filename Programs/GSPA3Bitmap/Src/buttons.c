#include "buttons.h"

#define BUTTON_S7_MASK 0x01U << 7

int buttonHandler(bool* isFileCompleted, uint16_t eingabe) {
    if (BUTTON_S7_MASK != (eingabe & BUTTON_S7_MASK)) {
        *isFileCompleted = true;
    }
    return 0;
}