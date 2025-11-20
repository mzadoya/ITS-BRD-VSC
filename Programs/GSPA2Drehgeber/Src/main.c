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
#include "buttons.h"
#include "display.h"
#include <limits.h>

#define PUNKT 0x01U
int tst = 0;
bool fehler = false;
uint16_t direction = 0;
uint32_t timeDiff = 0;
bool isHigh = false;


int main(void) {
	initITSboard();    // Initialisierung des ITS Boards
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch

	
	initTimer();
	initDisplay();
	initLed();
	initDecoder();
	
	GPIOE->BSRR = PUNKT << 16; 
	while(1) {
		if (isHigh) {
			GPIOE->BSRR = PUNKT; 
		}
		else {
			GPIOE->BSRR = PUNKT << 16; 
		}
		isHigh = !isHigh;
		
		 //or uint16 // TODO: rename 
		 
		uint32_t timer2 = TIM2->CNT;
		uint16_t eingabe = GPIOF->IDR;

		

		int rcButton = readButtons(eingabe, &fehler);
		

	
	
		if (fehler!=true) {
			int rcRotary = ecoderUpdate(eingabe, &direction, timer2); //TODO rename
		

			if (rcRotary == ENCODER_ERROR) {
				fehler = true;
			}
		
			ledSetDirection(direction);
			ledSetPhaseCount(transitionCount);
			printAngle(angle);
			printAngularVelocity(speed);
		}
		else {

			ledSetError();
		}
		
		
	}
}

// EOF

 