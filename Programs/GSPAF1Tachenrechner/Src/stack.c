#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_SIZE 1000

static int position = 0;
static int stack[MAX_SIZE];

//-----------------------Helpers
bool safetyCheckOne() {
    if (position == 0) {
        return false;
    }
    return true;
 }

 bool safetyCheckTwo() {  
    if (position <= 1) {
        return false;
    }
    return true;
 }

 bool overflowCheck() {
    if (position >= MAX_SIZE) {
        return false;
    }
    return true;
 }

  bool underflowCheck() {
    if (0 >= position) {
        return false;
    }
    return true;
 }

 int getPos() {
    return position;
 }

 int getSecondValue() {
    if (safetyCheckOne()) {
       return stack[position-2];
    }
    return -2;
 }
//-----------------------End Helpers

void init() {
    for (int i = 0; i < MAX_SIZE; i++) {
        stack[i] = 0;
    }
}

int top() {
    if (safetyCheckOne()) {
       return stack[position-1];
    }
    return -1;
} //Correct

int getAll(int* all){

    if (safetyCheckOne()) {
       for (int i = position - 1; i >= 0; i-- ) {
            all[i] = stack[i];
        }   
        return position;
    }
    return -1;
} //Correct

void clear() {
    for (int i = position - 1; i >= 0; i-- ) {
        stack[i] = 0;  
    }
    position = 0;
} //correct

int dupeTop() {
    if (safetyCheckOne()) {
        stack[position] = stack[position-1];  
        position++;
        return 0;
    }
    return -2;
} //correct

int swapTop() {
    if (!safetyCheckTwo()) {
        return -2;
    }
    if (overflowCheck()) {
        int temporary = stack[position - 1]; //сделать проверку что есть предыдущий
        stack[position - 1] = stack[position - 2];
        stack[position - 2] = temporary;
        return 0;
    }
    else {
         printf("Fehler in Datei %s, Zeile %d: Stack is full!\n", __FILE__, __LINE__);
    }
    return -1;
} //correct

int push(int v) {
    if (overflowCheck()) {
        stack[position] = v;
        position++;   
        return 0;
    }
     return -1;
} //Correct

int pop(int *v) {
    if (underflowCheck()) {
        *v = stack[position - 1];
        position--;
        return 0;   
    }
    return -1;
} //Correct

