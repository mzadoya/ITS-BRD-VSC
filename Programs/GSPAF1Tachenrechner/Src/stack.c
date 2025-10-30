#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "stack.h"
#include "global.h"
#include "fehler.h"g

static int position = 0;
static int stack[STACK_MAX_SIZE];

/**
 ********************************************************************************  
 * @brief Prüft, ob im Stack min. ein Wert vorhanden sind.
 * 
 * @param void
 *
 * @return true, wenn zwei oder mehr Werte im Stack vorhanden sind, andernfalls 
 *         false
 *********************************************************************************/
bool safetyCheckOne(void) {
    if (position == 0) {
        return false;
    }
    return true;
 }

 bool safetyCheckTwo(void) {  
    if (position <= 1) {
        return false;
    }
    return true;
 }

 bool overflowCheck(void) {
    if (position >= STACK_MAX_SIZE) {
        return false;
    }
    return true;
 }
/**
 ********************************************************************************  
 * @brief prueft ob ein Stack leer ist
 * 
 * @param void
 *
 * @return true wenn sich mind. ein Element im Stack befindet, andernfalls false
 *********************************************************************************/
  bool underflowCheck(void) {
    if (0 >= position) {
        return false;
    }
    return true;
 }

void init(void) {
    for (int i = 0; i < STACK_MAX_SIZE; i++) {
        stack[i] = 0;
    }
}

int top(int* topValue) { //TODO : return 
    if (safetyCheckOne()) {
       *topValue = stack[position-1];
       return 0;
    }
    return ERR_NOT_ENOUGH_VALUES;
} 

int getAll(int* all){ //TODO stackoverflow 

    if (safetyCheckOne()) {
       for (int i = position - 1; i >= 0; i-- ) {
            all[i] = stack[i];
        }   
        return position;
    }
    return ERR_NOT_ENOUGH_VALUES;
} 

void clear(void) {
    for (int i = position - 1; i >= 0; i-- ) {
        stack[i] = 0;  
    }
    position = 0;
} 

int dupeTop(void) {
    if (!safetyCheckOne()) {
        return ERR_STACK_OVERFLOW;
    }
    if (overflowCheck()) {
        stack[position] = stack[position-1];  
        position++;
        return 0;
    }
    return ERR_NOT_ENOUGH_VALUES;
}

int swapTop(void) {
    if (!safetyCheckTwo()) {
        return ERR_NOT_ENOUGH_VALUES;
    }
    if (overflowCheck()) {
        int temporary = stack[position - 1]; 
        stack[position - 1] = stack[position - 2];
        stack[position - 2] = temporary;
        return 0;
    }
    return ERR_STACK_OVERFLOW;
} 

int push(int v) {
    if (overflowCheck()) {
        stack[position] = v;
        position++;   
        return 0;
    }
     return ERR_STACK_OVERFLOW;
} 

int pop(int *v) {
    if (underflowCheck()) {
        *v = stack[position - 1];
        position--;
        return 0;   
    }
    return ERR_STACK_OVERFLOW;
} 

