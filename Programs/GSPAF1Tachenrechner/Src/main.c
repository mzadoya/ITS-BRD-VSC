/**
  ******************************************************************************
  * @file    main.c
  * @author  Maksym Zadoya
  * @brief   Praktikumsaufgabe 1 GS WS25/26 - Tachenrechner
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx_hal.h"
#include "init.h"
#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "lcd.h"
#include "fontsFLASH.h"
#include "additionalFonts.h"
#include "error.h"
#include <stdbool.h>
#include "display.h"
#include "scanner.h"
#include "stack.h"
#include "token.h"
#include <stdlib.h>
#include "converter.h"
#include "operations.h"
#include <stdio.h>

#define VALUE_SIZE 12
#define MESSAGE_EINGABE "Eingabe: "
#define MESSAGE_TOP "Letzte Eingabe: "
#define MESSAGE_STACK_ALL "Alle Eingaben:\n"
#define MESSAGE_SWAP_OK "Swap erfolgreich!\n"
#define MESSAGE_DOUBLE_OK "Doppelung erfolgreich!\n"
#define ERR_OVERFLOW_STACK "Fehler: Stackoverflow!\n"
#define ERR_OVERFLOW "Fehler: Ueberlauf bei Berechnung\n"
#define ERR_UNDERFLOW "Fehler: Stackunderflow\n"
#define ERR_DIVISION "Fehler: Division durch 0!\n"
#define ERR_NOT_ENOUGH_VALUES "Fehler: Nicht genug Zahlen im Stack\n"

void printTopValue() {
	char r[VALUE_SIZE];
	char res[] = MESSAGE_TOP;
	int topValue = 0;
	int rc = top(&topValue);
	if (rc == -2) {
		printStdout(ERR_NOT_ENOUGH_VALUES __FILE__);
	}
	else {
		valueToString(topValue, r);
		printStdout(r);
	}
}

int main(void) {
	initITSboard();   
	char d;
	GUI_init(DEFAULT_BRIGHTNESS);  
	TP_Init(false);                
	initDisplay();
	
	while(1) {
		T_token t = nextToken();

		switch (t.tok)
		{
		case NUMBER:
			{
				char r[VALUE_SIZE];
				char res[] = MESSAGE_EINGABE;
				valueToString(t.val, r);
				addDescription(res, r);

				printStdout(res);
				push(t.val);
				break;

			}

		case PLUS: 
		{
			int rc = sumStack();
			if (rc == -2) {
				printStdout(ERR_NOT_ENOUGH_VALUES __FILE__);
			}
			else if (rc == -1) {
				printStdout(ERR_OVERFLOW_STACK __FILE__);
			}
			else if (rc == -3) {
				printStdout(ERR_OVERFLOW __FILE__);
				
			}
			else if (rc == -4) {
				printStdout(ERR_UNDERFLOW __FILE__);
			}
			else if (rc == 0){
				printStdout( "Result: ");
				printTopValue();
				printStdout("\n");
			}
			break;
		}
		case MINUS:
		{ 
			int rc = subStack();
			if (rc == -2) {
				printStdout(ERR_NOT_ENOUGH_VALUES __FILE__);
			}
			else if (rc == -1) {
				printStdout(ERR_OVERFLOW_STACK __FILE__);
			}
			else if (rc == -3) {
				printStdout(ERR_OVERFLOW __FILE__);
			}
			else if (rc == -4) {
				printStdout(ERR_UNDERFLOW __FILE__);
			}
			else {
				printStdout( "Result: ");
				printTopValue();
				printStdout("\n");
			}
 			break;
		}
		case MULT:
		{
			int rc = mulStack();
			if (rc == -2) {
				printStdout(ERR_NOT_ENOUGH_VALUES __FILE__);
			}
			else if (rc == -1) {
				printStdout(ERR_OVERFLOW_STACK __FILE__);
			}
			else if (rc == -3) {
				printStdout(ERR_OVERFLOW __FILE__);
			}
			else if (rc == -4) {
				printStdout(ERR_UNDERFLOW __FILE__);
			}
			else {
				printStdout( "Result: ");
				printTopValue();
				printStdout("\n");
			}

			break;
		}
		case DIV:
		{
			int rc = divStack();
			if (rc == -2) {
				printStdout(ERR_NOT_ENOUGH_VALUES __FILE__);
			}
			else if (rc == -1) {
				printStdout(ERR_OVERFLOW_STACK __FILE__);
			}
			else if (rc == -5) {
				printStdout(ERR_DIVISION __FILE__);
			}
			else if (rc == -4) {
				printStdout(ERR_UNDERFLOW __FILE__);
			}
			else {
				printStdout( "Result: ");
				printTopValue();
				printStdout("\n");
			}
			
			break;
		}
		case CLEAR:
			clear();
		    clearStdout();
		    clearEchoTerm();
		    break;
		
		case PRT_ALL:
		{
			int all[VALUE_SIZE];
			int stackSize = getAll(all);
			if (stackSize == -2) {
				printStdout(ERR_NOT_ENOUGH_VALUES __FILE__);
			}

			else {
				printStdout(MESSAGE_STACK_ALL);
				for (int i = 0; i < stackSize; i++) {
					char r[VALUE_SIZE];
					valueToString(all[i], r);
					printStdout(r);
					printStdout("\n");
				}
			}
			break;
		}
		case PRT:
		{
			int topValue = 0;
			printStdout("Letzte Eingabe:");
			printTopValue();
			printStdout("\n");
			break;
		}

		case SWAP:
		{
			int rc = swapTop();
			if (rc == -2) {
				printStdout(ERR_NOT_ENOUGH_VALUES __FILE__);
			}
			else if (rc == -1) {
			    printStdout(ERR_OVERFLOW_STACK __FILE__);
			}
			else {
				printStdout(MESSAGE_SWAP_OK);
			}
		}
		break;

		case DOUBLE:
		{
			int rc = dupeTop();
			if (rc == -2) {
				printStdout(ERR_NOT_ENOUGH_VALUES __FILE__);
			}
			else {
				printStdout(MESSAGE_DOUBLE_OK __FILE__);
			}
		}
		break;
			
		case OVERFLOW:
		printStdout("Overflow!\n" __FILE__ );
		break;
/*
		case UNEXPECTED:
		printStdout("Unexpected command / button!\n");
		break;

		case ENTER:

		break;
*/
		default:
			break;
		}
	}
}


