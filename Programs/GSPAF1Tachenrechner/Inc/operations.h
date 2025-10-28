/**
  * @file operations.h
  * @author Maksym Zadoya
  * @date 2025/10/27 #1
  * @brief Header file of operations module. 
  */

#ifndef OPERATIONS_H
#define OPERATIONS_H

/**
 ******************************************************************************
 * @brief   Subtrahiert die beiden obersten Elemente des Stacks.
 *
 * @param   void
 *
 * @return  Bei erfolgreicher Ausführung wird 0 zurückgegeben. 
 *          Andernfalls wird einer der folgenden Fehlercodes zurückgegeben:
 *          - (-1): Stack ist voll (Overflow)
 *          - (-2): Zu wenige Elemente auf dem Stack
 *          - (-3): Arithmetischer Überlauf bei der Subtraktion
 *          - (-4): Fehler beim Entfernen (Pop) der Werte vom Stack
 ******************************************************************************
 */

int subStack(void);

/**
 ******************************************************************************
 * @brief   Addiert die beiden obersten Elemente des Stacks.
 *
 * @param   void
 *
 * @return  Bei erfolgreicher Ausführung wird 0 zurückgegeben. 
 *          Andernfalls wird einer der folgenden Fehlercodes zurückgegeben:
 *          - (-1): Stack ist voll (Overflow)
 *          - (-2): Zu wenige Elemente auf dem Stack
 *          - (-3): Arithmetischer Überlauf bei der Addition
 *          - (-4): Fehler beim Entfernen (Pop) der Werte vom Stack
 ******************************************************************************
 */

int sumStack(void);

/**
 ******************************************************************************
 * @brief   Dividiert die beiden obersten Elemente des Stacks.
 *
 * @param   void
 *
 * @return  Bei erfolgreicher Ausführung wird 0 zurückgegeben. 
 *          Andernfalls wird einer der folgenden Fehlercodes zurückgegeben:
 *          - (-1): Stack ist voll (Overflow)
 *          - (-2): Zu wenige Elemente auf dem Stack
 *          - (-3): Division durch Null oder arithmetischer Fehler
 *          - (-4): Fehler beim Entfernen (Pop) der Werte vom Stack
 ******************************************************************************
 */

int divStack(void);

/**
 ******************************************************************************
 * @brief   Multipliziert die beiden obersten Elemente des Stacks.
 *
 * @param   void
 *
 * @return  Bei erfolgreicher Ausführung wird 0 zurückgegeben. 
 *          Andernfalls wird einer der folgenden Fehlercodes zurückgegeben:
 *          - (-1): Stack ist voll (Overflow)
 *          - (-2): Zu wenige Elemente auf dem Stack
 *          - (-3): Arithmetischer Überlauf bei der Multiplikation
 *          - (-4): Fehler beim Entfernen (Pop) der Werte vom Stack
 ******************************************************************************
 */

int mulStack(void);

#endif