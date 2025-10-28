/**
  * @file stack.h
  * @author Maksym Zadoya
  * @date 2025/10/27 #1
  * @brief Header file of stack module. 
  */

#include <stdbool.h>

#ifndef STACK_H
#define STACK_H

/**
 ******************************************************************************
 * @brief   Initialisierung des Stacks
 *
 * @param   void
 *
 * @return  kein
 ******************************************************************************
 */

void init(void);

/**
 ******************************************************************************
 * @brief   zeigt der oberste Element im Stack
 *
 * @param   void
 *
 * @return  beim erfolgreich ergibt der oberste Element vom Stack
 *          beim Fehlern ergibt (-1)
 ******************************************************************************
 */

int top(void);

/**
 ******************************************************************************
 * @brief   fügt alle Elemente vom Stack in einen Array
 *
 * @param   all Zeiger auf das Array, in dem die Stack-Werte gespeichert werden
 *
 * @return  ergibt (0) beim Erfolg ider (-1) beim Fehlern
 ******************************************************************************
 */

int getAll(int* all);

/**
 ******************************************************************************
 * @brief   setz alle Elemente und position auf (0)
 *
 * @param   void
 *
 * @return  Kein return
 ******************************************************************************
 */

void clear(void);

/**
 ******************************************************************************
 * @brief   erzuegt ein Duplikat des obersten Elements und fuegt ihm in Stack
 *
 * @param   void
 *
 * @return  beim Erfolg (0) und beim Fehlern (-1)
 ******************************************************************************
 */

int dupeTop(void);

/**
 ********************************************************************************  
 * @brief Vertauscht die beiden zuletzt auf den Stack gelegten Werte
 * 
 * @return 0 wenn die Werte erfolgreich vertauscht wurden 
 *        -1 wenn der Stack voll ist (Overflow)
 *        -2 wenn sich weniger als zwei Elemente im Stack befinden 
 *********************************************************************************/

int swapTop();

/**
 ********************************************************************************  
 * @brief Legt den uebergebenen Wert auf den Stack 
 * 
 * @param v Der Wert, der auf den Stack gelegt werden soll
 * 
 * @return 0 wenn das Element erfolgreich auf den Stack gelegt wurde, 
 *        -1 wenn der Stack voll ist (Overflow)
 *********************************************************************************/

int push(int v);

/**
 ********************************************************************************  
 * @brief Entfernt das oberste Elemente vom Stack und gibt es zurück.
 * 
 * @param v Zeiger auf eine Variable, in der das entfernte Element gespeichert wird.
 * 
 * @return 0 wenn das Element erfolgreich entfernt wurde, -1 wenn der Stack leer ist
 *********************************************************************************/

int pop(int *v);


/**
 ********************************************************************************  
 * @brief prueft ob es mehr als MaxSize Elemente in Stack gibt
 * 
 * @param void
 *
 * @return true wenn es kein Overflow gibt und false wenn es ein Overflow gibt
 *********************************************************************************/

bool overflowCheck();

/**
 ********************************************************************************  
 * @brief prueft ob es zwei Elemente im Stack gibt
 * 
 * @param void
 *
 * @return true wenn es mind. zwei Elemente gibt und false wenn es weniger oder gleich 1
 *********************************************************************************/

bool safetyCheckTwo();


#endif