/**
  ******************************************************************************
  * @file    main.c
  * @author  Franz Korf
  * @brief   Kleines Testprogramm fuer neu erstelle Fonts.
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
#include "fehler.h"
#include "global.h"
#include "rotary_decoder.h"
#include "led.h"
#include "timer.h"
#include "converter.h"
#include "buttons.h"

#define VALUE_SIZE 50
bool fehler = false;
uint8_t direction = 0;
uint32_t timeDiff = 0;
double speed = 0;

int main(void) {
	initITSboard();    // Initialisierung des ITS Boards
    initLed();
	
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch

	initTimer();
	timerStart();

	while(1) {
		uint16_t eingabe = GPIOF->IDR; //or uint16 // TODO: rename 
		uint8_t buttons = GPIOF->IDR;

		int rcButton = readButtons(buttons, &fehler);
		int rcRotary = ecoderUpdate(eingabe, &direction); //TODO rename
		
		if (rcRotary == ENCODER_CHANGED) {
		    updateSpeed(&speed);
		}
		else if (rcRotary == ENCODER_ERROR) {
			fehler = true;
		}

		if (fehler!=true) {
			ledSetDirection(direction);
			char str[VALUE_SIZE];
			convertDoubleValue(str, speed);
			lcdPrintS(str);
		}
		else {
			ledSetError();
		}
	}
}

// EOF
