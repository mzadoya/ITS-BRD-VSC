/**
  ******************************************************************************
  * @file    main.c
  * @author  Franz Korf
  * @brief   Kleines Testprogramm fuer neu erstelle Fonts.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/

#include "BMP_types.h"
#include "LCD_general.h"
#include "errorhandler.h"
#include "input.h"
#include "stm32f4xx_hal.h"
#include "init.h"
#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "lcd.h"
#include "fontsFLASH.h"
#include "additionalFonts.h"
#include "eCodes.h"
#include "handshake.h"
#include "buttons.h"
#include "converter.h"
#include "display.h"

bool nextFileReady = true;
bool isFileCompleted = false;
uint16_t buttons = 0;
BITMAPFILEHEADER fileHeader;
BITMAPINFOHEADER passport;
RGBQUAD palette[MAX_COLOR_TABLE_SIZE];
char nextByte[2] = {00, 00};
uint16_t lcdColor = 0;

uint32_t start = 0;
uint32_t end = 0;

void errorHandler(int rc) {

	const char* msg;
	switch (rc)
	{
	case ERR_NOT_BMP:
		msg = "Invalid Header Signature (Expected 'BM')";
		break;
	case ERR_WRONG_FORMAT:
		msg = "Unsupported Format (Compression or BitDepth mismatch)";
		break;
	case ERR_READ_FAIL:
		msg = "Unexpected End of File or Read Fail";
		break;
	case ERR_OUT_OF_BOUNDS:
		msg = "Image dimensions exceed Display Resolution";
		break;
	case ERR_SIGNATURE:
		msg = "Corrupted Data Stream";
		break;
	default:
		msg = "Unknown Sys Error";
		break;
	}

	printError(rc, __FILE__, __LINE__, (char*)msg, false);

}
int main(void) {
	initITSboard(); 
	GUI_init(DEFAULT_BRIGHTNESS);
	TP_Init(false); 
	initInput();


	while(1) {
		//Read Block
		buttons = GPIOF->IDR;
		
		if (nextFileReady) {
			openNextFile();
			GUI_clear(0xFFFF);
			int rcShake = shakeHandsWithFile(&fileHeader, &passport, palette);
			if (rcShake != OK) {
				errorHandler(rcShake);
			}
			initCoords(passport.biWidth, passport.biHeight);
			isFileCompleted = false;
			nextFileReady = false;
			start = fileHeader.bfOffBits;
			end = fileHeader.bfSize;
		}
		COMread(nextByte, 1, 2);

	
		//Compute Block
		int rcInput = inputRecognizer(nextByte);
		if (rcInput == STATUS_END_OF_FILE) {
			isFileCompleted = true;
		}
		buttonHandler(&nextFileReady, buttons);



		if (!isFileCompleted /*&& rcInput == STATUS_COLOR*/) {
		convertColor(palette[nextByte[1]], &lcdColor);		
		//Back Block


		displayDraw(lcdColor);
		}
	}
}

// EOF
