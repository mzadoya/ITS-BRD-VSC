/**
 ******************************************************************************
 * @file    main.c
 * @author  Maksym Zadoya
 * @brief   Bestimmt mit einem inkrementellen Drehgeber die Drehrichtung,
 *          den Drehwinkel und die Winkelgeschwindigkeit eines rotierenden
 *			    System mittels Interrupt.
 * @date    2025/12/30
 * @version 1.0 vom 2025/12/30
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
#include "error_handler.h"
#include "display.h"
#include "fehler.h"
#include "fontsFLASH.h"
#include "global.h"
#include "init.h"
#include "lcd.h"
#include "led.h"
#include "timer.h"
#include "encoder_interrupt.h"

static bool fehler = false;
static uint8_t direction = 0;
static bool isReadyToPrint = false;
static double lastPrintedAngle = 0.0;
static double angleToPrint = 0.0;
static double speed = 0.0;
static double angle = 0.0;
static int32_t currentTransCount; // schlecht? 

int main(void) {
  initITSboard();
  GUI_init(DEFAULT_BRIGHTNESS);
  TP_Init(false);
  initTimer();
  initDisplay();
  initLed();
  initInterrupt();
  while (1) {

    int rc = encoderUpdater(&angle, &speed, &direction);
    if (rc == ENCODER_TIME_INVALID || rc == ENCODER_DATA_INCOSISTENT) {
      errorState(rc);
    }

    if ((lastPrintedAngle != angle && !isReadyToPrint)) {
        isReadyToPrint = true;
		    angleToPrint = angle;
      }

      ledSetDirection(direction);
      getEncoderLedTransitionCounter(&currentTransCount); // schlecht? 
      ledPhaseValue(currentTransCount);

      if (isReadyToPrint) {
        if (displayPrintHandler(angleToPrint, speed) == COMPLETED) {
			lastPrintedAngle = angleToPrint;
			isReadyToPrint = false;
		}
      }
  }
}

// EOF
