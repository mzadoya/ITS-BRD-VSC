/**
 ******************************************************************************
 * @file    main.c
 * @author  Maksym Zadoya
 * @brief   Bestimmt mit einem inkrementellen Drehgeber die Drehrichtung,
 *          den Drehwinkel und die Winkelgeschwindigkeit eines rotierenden
 *			System.
 * @date    2025/11/15
 * @version 1.2 vom 2025/12/29
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/

// Standard C Libs
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

// Board Libs
#include "stm32f4xx_hal.h"

// App modules
#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "additionalFonts.h"
#include "buttons.h"
#include "display.h"
#include "fehler.h"
#include "fontsFLASH.h"
#include "global.h"
#include "init.h"
#include "lcd.h"
#include "led.h"
#include "rotary_decoder.h"
#include "timer.h"

static int tst = 0;
static bool fehler = false;
static uint16_t direction = 0;
static uint32_t timeDiff = 0;
static bool isReadyToPrint = false;
static double lastPrintedAngle = 0.0;
static double angleToPrint = 0.0;

int main(void) {
  initITSboard();
  GUI_init(DEFAULT_BRIGHTNESS);
  TP_Init(false);
  initTimer();
  initDisplay();
  initLed();
  initDecoder();

  while (1) {

    uint32_t timer2 = TIM2->CNT;
    uint16_t eingabe = GPIOF->IDR;

    int rcButton = readButtons(eingabe, &fehler);

    if (fehler != true) {
      int rcRotary = encoderUpdate(eingabe, &direction, timer2);
      if (rcRotary == ENCODER_ERROR) {
        fehler = true;

      } else if ((lastPrintedAngle != angle && !isReadyToPrint) || rcRotary == ENCODER_TIME_UPDATED) {
        isReadyToPrint = true;
		angleToPrint = angle;
      }

      ledSetDirection(direction);
      ledPhaseValue(transitionCount);

      if (isReadyToPrint) {
        if (displayPrintHandler(angleToPrint, speed) == COMPLETED) {
			lastPrintedAngle = angleToPrint;
			isReadyToPrint = false;
		}
      }
    } else {

      ledSetError();
    }
  }
}

// EOF
