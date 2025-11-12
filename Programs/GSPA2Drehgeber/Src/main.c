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

bool fehler = false;
uint8_t direction = 0;
uint32_t timeDiff = 0;
double speed = 0.0;
double angle = 0.0;

int main(void) {
	initITSboard();    // Initialisierung des ITS Boards
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch

	initDecoder();
	initTimer();
	initDisplay();
	initLed();
	while(1) {
		 //or uint16 // TODO: rename 
		uint8_t eingabe = GPIOF->IDR; 

		int rcButton = readButtons(eingabe, &fehler);
		int rcRotary = ecoderUpdate(eingabe, &direction); //TODO rename
		
		if (rcRotary == ENCODER_TIME_UPDATED) {
		    updateSpeed(&speed);
			updateAngle(&angle);
		}

		else if (rcRotary == ENCODER_ERROR) {
			fehler = true;
		}

		if (fehler!=true) {
			ledSetDirection(direction);
			ledSetPhaseCount(transitionCount);
			if (rcRotary == ENCODER_TIME_UPDATED) {
				printAngle(angle);
				printAngularVelocity(speed);
			}
		}
		else {
			ledSetError();
		}
	}
}

// EOF
