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
#include "fehler.h"
#include "global.h"

#define ERRMSG_OVERFLOW_STACK "Fehler: Stackoverflow!\n"
#define ERRMSG_OVERFLOW "Fehler: Ueberlauf bei Berechnung\n"
#define ERRMSG_UNDERFLOW "Fehler: Stackunderflow\n"
#define ERRMSG_DIVISION "Fehler: Division durch 0!\n"
#define ERRMSG_NOT_ENOUGH_VALUES "Fehler: Nicht genug Zahlen im Stack\n"

#define MESSAGE_EINGABE "Eingabe: "
#define MESSAGE_TOP "Letzte Eingabe: "
#define MESSAGE_STACK_ALL "Alle Eingaben:\n"
#define MESSAGE_SWAP_OK "Swap erfolgreich!\n"
#define MESSAGE_DOUBLE_OK "Doppelung erfolgreich!\n"


void printTopValue() {
	char r[VALUE_SIZE];
	int topValue = 0;
	int rc = top(&topValue);
	if (rc == -2) {
		printStdout(ERRMSG_NOT_ENOUGH_VALUES __FILE__);
	}
	else {
		valueToString(topValue, r);
		printStdout(r);
	}
}

void printResultIfOk(int rc){
	if(rc == OK) {
		printStdout( "Result: ");
		printTopValue();
		printStdout("\n");
	}
}

void errorHandler(int rc) {
	switch(rc) {
		case ERR_NOT_ENOUGH_VALUES: 
		{
			printStdout(ERRMSG_NOT_ENOUGH_VALUES __FILE__);
			break;
		}
		case ERR_STACK_OVERFLOW:
		{
			printStdout(ERRMSG_OVERFLOW_STACK __FILE__);
			break;
		}
		case ERR_DIVISION_BY_ZERO :
		{
			printStdout(ERRMSG_DIVISION __FILE__);
			break;
		}
		case ERR_STACK_UNDERFLOW: {
			printStdout(ERRMSG_UNDERFLOW __FILE__);
			break;
		}
		case ERR_CALCULATION_OVERFLOW:
		{
			printStdout(ERRMSG_OVERFLOW __FILE__);
			break;
		}
		default:
			break;
	}
}

int main(void) {
	initITSboard();   
	char d;
	GUI_init(DEFAULT_BRIGHTNESS);  
	TP_Init(false);                
	initDisplay();
	int rc = OK;
	while(1) {
		T_token t = nextToken();

		switch (t.tok)
		{
		case NUMBER:
			{
				char r[VALUE_SIZE];
				char res[] = MESSAGE_EINGABE;
				rc = push(t.val);
				if (rc == OK) {
					valueToString(t.val, r);
					addDescription(res, r);
					printStdout(res);
				} 
				break;
			}

		case PLUS: 
		{
			rc = sumStack();
			printResultIfOk(rc);
			break;
		}
		case MINUS:
		{ 
			rc = subStack();
			printResultIfOk(rc);
 			break;
		}
		case MULT:
		{
			rc = mulStack();
			printResultIfOk(rc);
			break;
		}
		case DIV:
		{
			rc = divStack();
			printResultIfOk(rc);
			break;
		}
		case CLEAR:
			clear();
		    clearStdout();
		    clearEchoTerm();
			rc = OK;
		    break;
		
		case PRT_ALL:
		{
			int all[VALUE_SIZE];
			int stackSize = getAll(all);
			if (stackSize == ERR_NOT_ENOUGH_VALUES) {
				printStdout(ERRMSG_NOT_ENOUGH_VALUES __FILE__);
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
			printStdout("Letzte Eingabe:");
			printTopValue();
			printStdout("\n");
			break;
		}

		case SWAP:
		{
			rc = swapTop();
			if (rc == OK) {
				printStdout(MESSAGE_SWAP_OK);
			}
		}
		break;

		case DOUBLE:
		{
			rc = dupeTop();
			if (rc == OK) {
				printStdout(MESSAGE_DOUBLE_OK);
			}
		}
		break;
			
		case OVERFLOW:
		printStdout("Overflow!\n" __FILE__ );
		printStdout("\n");
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
		if (rc!=OK) {
			errorHandler(rc);
			rc = OK;
			printStdout("\n");
		}
	}
}


