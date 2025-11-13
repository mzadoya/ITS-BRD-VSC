#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stack.h"
#include <limits.h>
#include "operations.h"
#include "global.h"
#include "fehler.h"

int sumStack() {

    int topValue;
    int nextValue;

    int rc = pop(&topValue);
    if (rc != OK) {
        return rc;
    }
    rc = pop(&nextValue);
    if (rc != OK) {
        return rc;
    }
    
    if (nextValue <= 0 && topValue < INT_MIN - nextValue) {
        return ERR_CALCULATION_OVERFLOW;
    }
    if (nextValue >= 0 && topValue > INT_MAX - nextValue) {
        return ERR_CALCULATION_OVERFLOW;
    }
    int ergebnis = topValue + nextValue;
        if (overflowCheck()) {
            push(ergebnis);
            return OK;
        }
        else {
            return ERR_STACK_OVERFLOW;
        }
}

int subStack() {
     
    int topValue;
    int nextValue;
    int rc = pop(&topValue);
    if (rc != OK) {
        return rc;
    }
    rc = pop(&nextValue);
    if (rc != OK) {
        return rc;
    }
    if (topValue < 0 && nextValue > INT_MAX + topValue) {
        return ERR_CALCULATION_OVERFLOW;
    }
    if (topValue > 0 && nextValue < INT_MIN + topValue) {
        return ERR_CALCULATION_OVERFLOW; 
    }     
    int ergebnis = nextValue - topValue;

        if (overflowCheck()) {
            push(ergebnis);
            return OK;
        }
        else {
            return ERR_STACK_OVERFLOW;
        }
}

int mulStack() {

    if (!safetyCheckTwo()) {
        return ERR_NOT_ENOUGH_VALUES;
    }

    int topValue;
    int nextValue;
    int rc = pop(&topValue);
    if (rc != OK) {
        return rc;
    }
    rc = pop(&nextValue);
    if (rc != OK) {
        return rc;
    }
    if (topValue != 0 && nextValue != 0) { 
       if ((nextValue > 0 && (topValue > INT_MAX / nextValue || topValue < INT_MIN / nextValue)) ||
           (nextValue < 0 && (topValue == INT_MIN || topValue < INT_MAX / nextValue || topValue > INT_MIN / nextValue))) {
            return ERR_CALCULATION_OVERFLOW;
        }
        int ergebnis = topValue * nextValue;
        if (overflowCheck()) {
            push(ergebnis);
            return OK;
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
    int rc = pop(&nextValue);
    if (rc != OK) {
        return rc;
    } 

     if (nextValue == INT_MIN && topValue == -1 ) {
        return ERR_CALCULATION_OVERFLOW;
    }
    
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