#include "converter.h"
#include <stdbool.h>
#include <stdint.h>

int convertDoubleValue(char* str, double value) {

    bool wasNegative = false;
    int pos = 0;
    int ganzzahl = (int)value;
    int nachkomma = (int)((value - ganzzahl) * 100);

    if (ganzzahl < 0) {
        wasNegative = true;
        ganzzahl = ganzzahl * (-1);
    }

    while (nachkomma > 0) {
        int temp = nachkomma % 10;
        str[pos] = '0'+ temp;
        nachkomma = nachkomma / 10;
        pos++;
    }

    str[pos] = '.';

    while (ganzzahl > 0) {
        int temp = ganzzahl % 10;
        str[pos] = '0'+ temp;
        ganzzahl = ganzzahl / 10;
        pos++;
    }

    if (wasNegative) {
        str[pos] = '-';
        pos++;
    }

    str[pos] = '\0';

    for (int i = 0; i < pos / 2; i++) {
        char temp = str[i];
        str[i] = str[pos-1-i];
        str[pos-1-i] = temp;
    }

    return 0;
}