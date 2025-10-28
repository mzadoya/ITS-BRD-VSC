#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stack.h"
#include <limits.h>

/**
 ********************************************************************************  
 * @brief
 * 
 * @param
 * 
 * @return
 * 
 *********************************************************************************/
int sumStack() {

    if (!safetyCheckTwo()) {
        return -2;
    }

    if (overflowCheck()) {
        int topValue;
        int nextValue;
        if (pop(&topValue) != 0) return -4;
        if (pop(&nextValue)!=0) return -4;
        if (topValue <= INT_MAX - nextValue) { //TODO: if nextValue negative -> false number -> crash 
            int ergebnis = topValue + nextValue;
            push(ergebnis);
            return 0;
        }
        else {
             printf("Fehler in Datei %s, Zeile %d: Error bei der Berechnung von %d + %d - Overflow\n", __FILE__, __LINE__, topValue, nextValue);
             return -3;
        }
    }
    else {
         printf("Fehler in Datei %s, Zeile %d: Stack is full!\n", __FILE__, __LINE__);
         return -1;
    }
} //Correct - test Bedarf

/**
 ********************************************************************************  
 * @brief
 * 
 * @param
 * 
 * @return
 * 
 *********************************************************************************/
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
             printf("Fehler in Datei %s, Zeile %d: Error bei der Berechnung von %d - %d - Overflow\n", __FILE__, __LINE__,nextValue, topValue);
             return -3;
        }
       
    }
    else {
         printf("Fehler in Datei %s, Zeile %d: Stack is full!\n", __FILE__, __LINE__);
         return -1;
    }
    
}

/**
 ********************************************************************************  
 * @brief
 * 
 * @param
 * 
 * @return
 * 
 *********************************************************************************/
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
              printf("Fehler in Datei %s, Zeile %d: Error bei der Berechnung von %d * %d - Overflow\n", __FILE__, __LINE__, topValue, nextValue);
              return -3;
        }
    }
    else {
         printf("Fehler in Datei %s, Zeile %d: Stack is full!\n", __FILE__, __LINE__);
        return -1;
    }
} //Correct - test Bedarf


/**
 ********************************************************************************  
 * @brief
 * 
 * @param
 * 
 * @return
 * 
 *********************************************************************************/
int divStack() {
    if (!safetyCheckTwo()) {
        return -2;
    }
    int topValue;
    if (pop(&topValue) != 0) return -4;

    if(topValue == 0) {
        return -3;
    }
    
    if (overflowCheck()) {
        int nextValue;
        if (pop(&nextValue)!=0) return -4;
        if (topValue != -1 && topValue != 0) {
            int ergebnis = nextValue / topValue;
            push(ergebnis);
            return 0;
        }
        else {
             printf("Fehler in Datei %s, Zeile %d: Error bei der Berechnung von %d / %d - Overflow\n", __FILE__, __LINE__, nextValue, topValue);
        }
    }
    else {
         printf("Fehler in Datei %s, Zeile %d: Stack is full!\n", __FILE__, __LINE__);
    }
    return -1;
}