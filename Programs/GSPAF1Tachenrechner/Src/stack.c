#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_SIZE 1000

static int position = 0;
static int stack[MAX_SIZE];

//-----------------------Helpers

/**
 ********************************************************************************  
 * @brief Prüft, ob im Stack min. zwei Werte vorhanden sind.
 * 
 * @return true, wenn zwei oder mehr Werte im Stack vorhanden sind, anderfalls 
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
    if (position >= MAX_SIZE) {
        return false;
    }
    return true;
 }

  bool underflowCheck(void) {
    if (0 >= position) {
        return false;
    }
    return true;
 }

//-----------------------End Helpers

void init(void) {
    for (int i = 0; i < MAX_SIZE; i++) {
        stack[i] = 0;
    }
}

int top(void) {
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
        return 0;
    }
    return -1;
} //Correct

void clear(void) {
    for (int i = position - 1; i >= 0; i-- ) {
        stack[i] = 0;  
    }
    position = 0;
} //correct

int dupeTop(void) {
    if (safetyCheckOne()) {
        stack[position] = stack[position-1];  
        position++;
        return 0;
    }
    return -2;
} //correct

/**
 ********************************************************************************  
 * @brief Vertauscht die beiden zuletzt auf den Stack gelegten Werte
 * 
 * @return 0 wenn die Werte erfolgreich vertauscht wurden 
 *        -1 wenn der Stack voll ist (Overflow)
 *        -2 wenn sich weniger als zwei Elemente im Stack befinden 
 *********************************************************************************/
int swapTop(void) {
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

/**
 ********************************************************************************  
 * @brief Legt den uebergebenen Wert auf den Stack 
 * 
 * @param v Der Wert, der auf den Stack gelegt werden soll
 * 
 * @return 0 wenn das Element erfolgreich auf den Stack gelegt wurde, 
 *        -1 wenn der Stack voll ist (Overflow)
 *********************************************************************************/
int push(int v) {
    if (overflowCheck()) {
        stack[position] = v;
        position++;   
        return 0;
    }
     return -1;
} //Correct

/**
 ********************************************************************************  
 * @brief Entfernt das oberste Elemente vom Stack und gibt es zurück.
 * 
 * @param v Zeiger auf eine Variable, in der das entfernte Element gespeichert wird.
 * 
 * @return 0 wenn das Element erfolgreich entfernt wurde, -1 wenn der Stack leer ist
 *********************************************************************************/
int pop(int *v) {
    if (underflowCheck()) {
        *v = stack[position - 1];
        position--;
        return 0;   
    }
    return -1;
} //Correct

