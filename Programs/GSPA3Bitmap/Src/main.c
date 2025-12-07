/**
 ******************************************************************************
 * @file    main.c
 * @author  Maksym Zadoya
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "BMP_types.h"
#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "LCD_general.h"
#include "additionalFonts.h"
#include "buttons.h"
#include "display.h"
#include "eCodes.h"
#include "errorhandler.h"
#include "fontsFLASH.h"
#include "handshake.h"
#include "headers.h"
#include "init.h"
#include "input.h"
#include "lcd.h"
#include "stm32f4xx_hal.h"
#include "statusHandler.h"
#include "imageHandler.h"

bool nextFileReady = true;
uint16_t buttons = 0;
BITMAPFILEHEADER fileHeader;
BITMAPINFOHEADER passport;
RGBQUAD palette[MAX_COLOR_TABLE_SIZE];

int main(void) {
  initITSboard();
  GUI_init(DEFAULT_BRIGHTNESS);
  TP_Init(false);
  initInput();
  while (1) {
    
    buttons = GPIOF->IDR;
	buttonHandler(&nextFileReady, buttons);

    if (nextFileReady) {
      openNextFile();
      int rcHeaders = readHeaders();
      if (rcHeaders != OK) {
        errorHandler(rcHeaders);
      } else {
        GUI_clear(0xFFFF);
		    resetDecoderState();
        getFileHeader(&fileHeader);
        getInfoHeader(&passport);
        readPalette(palette, passport.biClrUsed);
        skipTrash(fileHeader.bfOffBits);
        initCoords(passport.biWidth, passport.biHeight);
        while (imageHandler( &passport, palette) != STATUS_END_OF_FILE) {	   }
        }
		nextFileReady = false;
      }
    }
  }
