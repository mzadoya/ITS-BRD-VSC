#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stack.h"
#include <limits.h>
#include "operations.h"
#include "global.h"
#include "fehler.h"

int sumStack() {

    if (!safetyCheckTwo()) {
        return ERR_NOT_ENOUGH_VALUES;
    }
    int topValue;
    int nextValue;
    if (pop(&topValue) != 0) return ERR_STACK_UNDERFLOW;
    if (pop(&nextValue)!=0) return ERR_STACK_UNDERFLOW;
    if (topValue <= INT_MAX - nextValue) { 
        int ergebnis = topValue + nextValue;
        if (overflowCheck()) {
            push(ergebnis);
            return 0;
        }
        else {
            return ERR_STACK_OVERFLOW;
        }
    }
    else {
        return ERR_CALCULATION_OVERFLOW;
    }
}

int subStack() {

    if (!safetyCheckTwo()) {
        return ERR_NOT_ENOUGH_VALUES;
    }
     
    int topValue;
    int nextValue;
    if (pop(&topValue) != 0) return ERR_STACK_UNDERFLOW;
    if (pop(&nextValue)!=0) return ERR_STACK_UNDERFLOW;
    if (topValue - nextValue <= INT_MAX && topValue - nextValue >= INT_MIN) {
        int ergebnis = nextValue - topValue;
        if (overflowCheck()) {
            push(ergebnis);
            return 0;
        }
        else {
            return ERR_STACK_OVERFLOW;
        }
    }
    else {
        return ERR_CALCULATION_OVERFLOW;
    }
}

int mulStack() {

    if (!safetyCheckTwo()) {
        return ERR_NOT_ENOUGH_VALUES;
    }

    int topValue;
    int nextValue;
    if (pop(&topValue) != 0) return ERR_STACK_UNDERFLOW; //1073 741 824 
    if (pop(&nextValue)!=0) return ERR_STACK_UNDERFLOW; //-2 
    if (nextValue != 0 && topValue <= (INT_MAX / nextValue) || nextValue != 0 && topValue >= INT_MIN/nextValue) {
        if (nextValue == -1 && topValue == INT_MIN || topValue == -1 && nextValue == INT_MIN) {
             return ERR_CALCULATION_OVERFLOW;
        }
        int ergebnis = topValue * nextValue;
        if (overflowCheck()) {
            push(ergebnis);
            return 0;
        }
        else {
            return ERR_STACK_OVERFLOW;
        }
    }
    else {
        return ERR_CALCULATION_OVERFLOW;
    }
}

int divStack() {
    if (!safetyCheckTwo()) {
        return ERR_NOT_ENOUGH_VALUES;
    }
    int topValue;
    if (pop(&topValue) != 0) return ERR_STACK_UNDERFLOW;

    if(topValue == 0) {
        return ERR_DIVISION_BY_ZERO;
    }
    
    int nextValue;
    if (pop(&nextValue)!=0) return ERR_STACK_UNDERFLOW;
    if (topValue != -1 && topValue != 0) {
        int ergebnis = nextValue / topValue;
        if (overflowCheck()) {
            push(ergebnis);
            return 0;
        }
        else {
            return ERR_STACK_OVERFLOW;
        }
    }
    else {
        return ERR_CALCULATION_OVERFLOW;
    }
}