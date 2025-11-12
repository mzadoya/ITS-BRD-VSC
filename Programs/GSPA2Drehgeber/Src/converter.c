#include "converter.h"
#include <stdbool.h>
#include <stdint.h>
#include "global.h"

int convertDoubleValue(char* str, double value) {

    bool wasNegative = false;
    int pos = 0;
    if (value < 0) {
        wasNegative = true;
        value = value * (-1);
    }
    int ganzzahl = (int)value;
    int nachkomma = (int)((value - ganzzahl) * 100);

    int startNachkomma = pos;
    int nachkommaLen = 0;

    if (nachkomma == 0) {
        str[pos++] = '0';
        str[pos++] = '0';
        nachkommaLen = 2;
    }
    else {
        while (nachkomma > 0) {
            int temp = nachkomma % 10;
            str[pos] = '0'+ temp;
            nachkomma = nachkomma / 10;
            nachkommaLen++;
            pos++;
        }
    }

    if (nachkommaLen == 1) {
        str[pos++] = '0';
        nachkommaLen++;
    }

    str[pos++] = '.';

    if (ganzzahl == 0) {
        str[pos++] = '0';
    }
    else {
        while (ganzzahl > 0) {
            int temp = ganzzahl % 10;
            str[pos] = '0'+ temp;
            ganzzahl = ganzzahl / 10;
            pos++;
        }
    }
    if (wasNegative) {
        str[pos] = '-';
        pos++;
    }

    for (int i = 0; i < pos / 2; i++) {
        char temp = str[i];
        str[i] = str[pos-1-i];
        str[pos-1-i] = temp;
    }

    for (int i = pos; i < VALUE_MAX_SIZE - 1; i++) {
        str[i] = ' ';
    }
    str[VALUE_MAX_SIZE-1] = '\0';
    return 0;
}