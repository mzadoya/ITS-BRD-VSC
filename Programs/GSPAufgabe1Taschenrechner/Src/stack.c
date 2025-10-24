#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_SIZE 1000

static int position = 0;
static int stack[MAX_SIZE];

//-----------------------Helpers
bool safetyCheck() {
    if (stack[position - 1] == NULL || stack[position] == NULL) {
        printf("", __FILE__, __LINE__);
        return false;
    }
    return true;
 }

 bool overflowCheck() {
    if (position == 0 || position == MAX_SIZE) {
        printf("", __FILE__, __LINE__);
        return false;
    }
    return true;
 }
//-----------------------End Helpers


void init() {
    
}

void top() {
    printf("%d", stack[position]);   //return? 
}

void getAll(){
    for (int i = position; i >= 0; i-- ) {
        printf("%d", stack[i]);
    }
}

void clear() {
    for (int i = position; i >= 0; i-- ) {
        stack[i] = NULL; // TODO: correct? 
        position--;
    }
}

int dupeTop() {
    if (safetyCheck()) {
        stack[position+1] = stack[position];  //проверка что в пределах 
        position++;
        return 0;
    }
    return -1;
}

int swapTop() {
    if (safetyCheck() && overflowCheck()) {
        int temporary = stack[position]; //сделать проверку что есть предыдущий
        stack[position] = stack[position - 1];
        stack[position - 1] = temporary;
        return 0;
    }
    else {
        printf("", __FILE__, __LINE__);
    }
    return -1;
}

int push(int v) {
    if (overflowCheck()) {
        stack[position] = v;
        position++;   
        return 0;
    }
     return -1;
}

int pop(int *v) {
    if (overflowCheck()) {
        int toReturn = stack[position];
        position--;
        printf("%d", toReturn);
        return 0;   
    }
    return -1;
}

int sumStack() {
    if (safetyCheck() && overflowCheck()) {
        if (stack[position] <= INT_MAX - stack[position-1]) {
            int ergebnis = stack[position-1] + stack[position];
            position++;
            stack[position] = ergebnis;
            return 0;
        }
        else {
            printf("", __FILE__, __LINE__);
        }
    }
    return -1;
}

int subStack() {
    if (safetyCheck() && overflowCheck()) {
        if (stack[position] <= INT_MAX + stack[position-1]) {
            int ergebnis = stack[position] - stack[position-1];
            position++;
            stack[position] = ergebnis;
            return 0;
        }
        else {
            printf("", __FILE__, __LINE__);
        }
    }
    return -1;
}

int mulStack() {
    if (safetyCheck() && overflowCheck()) {
        if (stack[position-1] != 0 && stack[position] <= INT_MAX / stack[position-1]) {
            int ergebnis = stack[position] * stack[position-1];
            position++;
            stack[position] = ergebnis;
            return 0;
        }
        else {
            printf("", __FILE__, __LINE__);
        }
    }
    return -1;
}

int divStack() {
    if (safetyCheck() && overflowCheck() && stack[position] != 0) {
        if (stack[position] != -1) {
            int ergebnis = stack[position-1] / stack[position];
            position++;
            stack[position] = ergebnis;
            return 0;
        }
        else {
            printf("", __FILE__, __LINE__);
        }
    }
    return -1;
}