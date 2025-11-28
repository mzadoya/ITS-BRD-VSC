/**
 ******************************************************************************
 * @file    main.c
 * @author  Maksym Zadoya
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
/*
#include "BMP_types.h"
#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "LCD_general.h"
#include "additionalFonts.h"
#include "buttons.h"
#include "converter.h"
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

bool nextFileReady = true;
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

void absoluteModeHanler() {
  COMread(nextByte, 1, 1);
  absoluteSize--;
  if (absoluteSize == 0) {
    if (!isAligned) {
      COMread(&trash, 1, 1);
    }
    isAbsoluteMode = false;
  }
  convertColor(palette[nextByte[0]], &lcdColor);
  // Ausgabe
  // displayDrawAbsolut(lcdColor);
  updateAbsolutLine(lcdColor);
}
void activateAbsolut(void) {
  absoluteSize = nextByte[1];
  if (absoluteSize % 2 != 0) {
    isAligned = false;
  } else {
    isAligned = true;
  }
  isAbsoluteMode = true;
}

void errorHandler(int rc) {

  const char *msg;
  switch (rc) {
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

  printError(rc, __FILE__, __LINE__, (char *)msg, false);
}

void statusCheck() {

  switch (status) {
  case STATUS_EOL:
    endline();
    break;

  case STATUS_END_OF_FILE:
    endline();
    // nextFileReady = true;
    break;

  case STATUS_ABSOLUTE:
    activateAbsolut();
    break;

  default:
    printError(status, __FILE__, __LINE__, MSG_ERR_UNKNOWN, false);
    break;
  }
}

void handleUncompressedImage() {
  int width = passport.biWidth;
  int align = (((width * 8) + 31) / 32) * 4;

  for (int y = 0; y < passport.biHeight; y++) {
    for (int x = 0; x < passport.biWidth; x++) {
      COMread(nextByte, 1, 1);
      convertColor(palette[nextByte[0]], &lcdColor);
      updateAbsolutLine(lcdColor);
    }
    int dif = align - width;
    if (dif > 0) {
      for (int i = 0; i < dif; i++) {
        COMread(&trash, 1, 1);
      }
    }
    endline();
  }
}

void handleCompressedImage() {
  COMread(nextByte, 1, 2);
  if (status == STATUS_DELTA) {
    updateDelta(nextByte[0], nextByte[1]);
    status = OK;
  } else {
    status = inputRecognizer(nextByte);

    if (status == STATUS_RLE) {
      convertColor(palette[nextByte[1]], &lcdColor);

      // Ausgabe
      // displayDrawRLE(lcdColor, nextByte[0]);
      updateRLELine(lcdColor, nextByte[0]);
    } else {
      statusCheck();
    }
  }
}
int main(void) {
  initITSboard();
  GUI_init(DEFAULT_BRIGHTNESS);
  TP_Init(false);
  initInput();
  while (1) {
    // Read Block
    buttons = GPIOF->IDR;

    if (nextFileReady) {
      openNextFile();
      int rcHeaders = readHeaders();
      if (rcHeaders != OK) {
        errorHandler(rcHeaders);
      } else {
        GUI_clear(0xFFFF);
        getFileHeader(&fileHeader);
        getInfoHeader(&passport);
        readPalette(palette, passport.biClrUsed);
        skipTrash(fileHeader.bfOffBits);
        initCoords(passport.biWidth, passport.biHeight);
        while (1) {

          if (passport.biCompression == BI_RGB) {
            handleUncompressedImage();
			nextFileReady = false;
			break;
          }

          else if (passport.biCompression == BI_RLE8) {
            if (isAbsoluteMode) {
              absoluteModeHanler();
            }
            // Absolute Handler END---------
            else {
              if (status == STATUS_END_OF_FILE) {
                endline();
                status = OK;
				nextFileReady = false;
                break;
              }
			  else {
				handleCompressedImage();
			  }
            }
          }
        }
      }
    }
    nextFileReady = false;

    buttonHandler(&nextFileReady, buttons);
  }
}





*/

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
        while (1) {

		  int rcImage = imageHandler( &passport, palette);
		  if (rcImage == STATUS_END_OF_FILE) {
			nextFileReady = false;
			break;
		  }
          }
        }
      }
    }
  }
