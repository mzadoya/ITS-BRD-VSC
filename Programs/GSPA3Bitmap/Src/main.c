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
#include "headers.h"
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
bool isAbsoluteMode = false;
uint16_t buttons = 0;
BITMAPFILEHEADER fileHeader;
BITMAPINFOHEADER passport;
RGBQUAD palette[MAX_COLOR_TABLE_SIZE];
unsigned char nextByte[2] = {00, 00};
uint16_t lcdColor = 0;
int absoluteSize = 0;
bool isAligned = false;
int status = OK;
unsigned char trash;

void activateAbsolut(void) {
	absoluteSize = nextByte[1];
	if (absoluteSize %2 !=0) {
		isAligned = false;
	}
	else {
		isAligned = true;
	}
	isAbsoluteMode = true;
}

void errorHandler(int rc) {

	const char* msg;
	switch (rc)
	{
	case ERR_NOT_BMP:
		msg = MSG_ERR_NOT_BMP;
		break;
	case ERR_WRONG_FORMAT:
		msg = MSG_ERR_WRONG_FORMAT;
		break;
	case ERR_READ_FAIL:
		msg = MSG_ERR_READ_FAIL;
		break;
	case ERR_OUT_OF_BOUNDS:
		msg = MSG_ERR_OUT_OF_BOUNDS;
		break;
	case ERR_SIGNATURE:
		msg = MSG_ERR_SIGNATURE;
		break;
	default:
		msg = MSG_ERR_UNKNOWN;
		break;
	}

	printError(rc, __FILE__, __LINE__, (char*)msg, false);
}

void statusCheck() {

	switch (status)
	{
	case STATUS_EOL:
		endline();
		break;

	case STATUS_END_OF_FILE:
		isFileCompleted = true;
		nextFileReady = true;
		break;
		
	case STATUS_ABSOLUTE:
		activateAbsolut();
		break;
	
	default:

		printError(status, __FILE__, __LINE__,MSG_ERR_UNKNOWN, false);

		break;
	}
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
			
			int rcHeaders = readHeaders();

			if (rcHeaders != OK) {
				errorHandler(rcHeaders);
				isFileCompleted = true;
			}
			else {
				GUI_clear(0xFFFF);
				getFileHeader (&fileHeader);
				getInfoHeader (&passport);
				readPalette(palette);
				skipTrash(fileHeader.bfOffBits);
				initCoords(passport.biWidth, passport.biHeight);
				isFileCompleted = false;
			}
			nextFileReady = false;
		}

		buttonHandler(&nextFileReady, buttons);

		if (!isFileCompleted) {

			//Absolute Handler ---------
			if (isAbsoluteMode) {
				COMread(nextByte, 1, 1);
				absoluteSize--;
				if (absoluteSize == 0) {
					if (!isAligned) {
						COMread(&trash, 1, 1);
					}
					isAbsoluteMode = false;
				}
				convertColor(palette[nextByte[0]], &lcdColor);
					//Ausgabe
				displayDrawAbsolut(lcdColor);

			}
			//Absolute Handler END---------
			else {
				COMread(nextByte, 1, 2);

				if(status == STATUS_DELTA) {
					updateDelta(nextByte[0], nextByte[1]);
					status = OK;
				}
				else {

				status = inputRecognizer(nextByte);
				
				if (status == STATUS_RLE) {
					convertColor(palette[nextByte[1]], &lcdColor);

					//Ausgabe
					displayDrawRLE(lcdColor, nextByte[0]);
				}
				else {
					statusCheck();
					}
				}
			}
		}
	}
}