/**
  * @file converter.h
  * @author Maksym Zadoya
  * @date 2025/10/27 #1
  * @brief Header file of converter module. 
  */

#ifndef CONVERTER_H
#define CONVERTER_H

/**
 ********************************************************************************  
 * @brief Konvertiert eine ganzzahlige Eingabe in ein Zeichen-Array (String) zur 
 *        Anzeige auf dem Display
 * 
 * @param value Die Zahl, die konvertiert werden soll
 * @param str   Zeiger auf das Array, in dem der resultierende String gespeichert wird
 * 
 *********************************************************************************/
void valueToString(int value, char* str);

/**
 ********************************************************************************  
 * @brief Fügt eine Beschreibung zur in String konvertierte Zahl hinzu
 * 
 * @param info Zeiger auf die Beschreibung, die vor der Zahl eingefügt werden soll
 * @param str Zeiger auf den Zielstring, an den die Beschreibung angehängt wird
 * 
 *********************************************************************************/
void addDescription(char* info, char* str);

#endif