#include <stdbool.h>

void valueToString(int value, char* str) {

    int pos = 0;
    bool wasNegative = false;

    if (value == 0) {
        str[pos] = '0';
        pos++;
        str[pos] = '\0';
    }
    else {
        if (value < 0) {
            wasNegative = true;
            value = value * (-1);
        }

        while (value > 0 ) {
            int temp = value % 10;
            str[pos] = '0'+ temp;
            value = value / 10;
            pos++;
        }

        if (wasNegative) {
            str[pos] = '-';
            pos++;
        }
        
        str[pos] = '\0';
        
        

        for (int i = 0; i < pos/2; i++) {
            char temp = str[i];
            str[i] = str[pos-i-1];
            str[pos-i-1] = temp;
        }
    }
}

void addDescription(char* info, char* str) {

    int lenghtInfo = 0;
    while(info[lenghtInfo] != '\0') {
        lenghtInfo++;
    }

    int i = 0;
    while (str[i] != '\0') {
        info[lenghtInfo+i] = str[i];
        i++;
    }
    info[lenghtInfo + i] = '\n';
    info[lenghtInfo + i + 1] = '\0';

}