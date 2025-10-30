#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stack.h"
#include <limits.h>
#include "operations.h"


int sumStack() {

    if (!safetyCheckTwo()) {
        return -2;
    }

    if (overflowCheck()) {
        int topValue;
        int nextValue;
        if (pop(&topValue) != 0) return -4;
        if (pop(&nextValue)!=0) return -4;
        if (topValue <= INT_MAX - nextValue) { 
            int ergebnis = topValue + nextValue;
            push(ergebnis);
            return 0;
        }
        else {
             return -3;
        }
    }
    else {
         return -1;
    }
} 

int subStack() {

    if (!safetyCheckTwo()) {
        return -2;
    }
    
    if (overflowCheck()) {
        int topValue;
        int nextValue;
        if (pop(&topValue) != 0) return -4;
        if (pop(&nextValue)!=0) return -4;
        if (topValue - nextValue <= INT_MAX && topValue - nextValue >= INT_MIN) {
            int ergebnis = nextValue - topValue;
            push(ergebnis);
            return 0;
        }
        else {
             return -3;
        }
       
    }
    else {
         return -1;
    }
    
}

int mulStack() {

    if (!safetyCheckTwo()) {
        return -2;
    }

    if (overflowCheck()) {
        int topValue;
        int nextValue;
        if (pop(&topValue) != 0) return -4;
        if (pop(&nextValue)!=0) return -4;
        if (nextValue != 0 && topValue <= INT_MAX / nextValue) {
            int ergebnis = topValue * nextValue;
            push(ergebnis);
            return 0;
        }
        else {
              return -3;
        }
    }
    else {
        return -1;
    }
} 

int divStack() {
    if (!safetyCheckTwo()) {
        return -2;
    }
    int topValue;
    if (pop(&topValue) != 0) return -4;

    if(topValue == 0) {
        return -5;
    }
    
    if (overflowCheck()) {
        int nextValue;
        if (pop(&nextValue)!=0) return -4;
        if (topValue != -1 && topValue != 0) {
            int ergebnis = nextValue / topValue;
            push(ergebnis);
            return 0;
        }
    }
    return -1;
}