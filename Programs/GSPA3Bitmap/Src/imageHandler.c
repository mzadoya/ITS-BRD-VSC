#include "imageHandler.h"
#include "converter.h"
#include "display.h"
#include "eCodes.h"
#include "input.h"
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "errorHandler.h"
#include "LCD_GUI.h"

#define MAX_SCALED_WIDTH (480*5)
#define MAX_SCALED_HEIGHT (320*5) 
#define MAX_LINES 5
#define BLACK 0x00U
bool isAligned = false;
int status = OK;
unsigned char trash;
unsigned char nextByte[2] = {00, 00};
uint16_t lcdColor = 0;
int absoluteSize = 0;
bool isAbsoluteMode = false;
int mode = 0;
uint16_t lineBuffer[MAX_LINES][MAX_SCALED_WIDTH];
int currentRecordY = 0;
int currentRecordX = 0;

void resetDecoderState(void) {
    status = OK;
    isAbsoluteMode = false;
    absoluteSize = 0; 
    isAligned = false;
}

void reset() {
  for (int y = 0; y < MAX_LINES; y++) {
    for (int x = 0; x < MAX_SCALED_WIDTH; x++) {
      lineBuffer[y][x] = BLACK;
    }
  }
}

int recordLine() {
  if (currentRecordX < MAX_SCALED_WIDTH) {
    lineBuffer[currentRecordY][currentRecordX] = lcdColor;
    currentRecordX++;
  }
  return OK;
}

void handleUncompressedImage(RGBQUAD* palette, BITMAPINFOHEADER* passport) {
  int width = (*passport).biWidth;
  int align = (((width * 8) + 31) / 32) * 4;

  for (int x = 0; x < (*passport).biWidth; x++) {
      COMread(nextByte, 1, 1);
      convertColor(palette[nextByte[0]], &lcdColor);
      if (mode == MODE_DRAW) {
        updateAbsolutLineToDraw(lcdColor);
      }
      else if (mode == MODE_RECORDING) {
        recordLine();
      }
    }
    int dif = align - width;
    if (dif > 0) {
      for (int i = 0; i < dif; i++) {
        COMread(&trash, 1, 1);
    }
  }
}

void absoluteModeHanler(RGBQUAD* palette) {
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
  if (mode == MODE_DRAW) {
    updateAbsolutLineToDraw(lcdColor);
  }
  else if (mode == MODE_RECORDING) {
    recordLine();
  }
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

void statusCheck() {
  switch (status) {
  case STATUS_EOL:
    if(mode == MODE_DRAW)
    {
     endline();
    }
    break;

  case STATUS_END_OF_FILE:
    if(mode == MODE_DRAW)
    {
     endline();
    }
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

void handleCompressedImage(RGBQUAD* palette) {
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
      if (mode == MODE_DRAW) {
        updateRLELine(lcdColor, nextByte[0]);
      }
      else if (mode == MODE_RECORDING) {
        for (int i = 0; i<nextByte[0]; i++) {
          recordLine();
        }
      }
    } else {
      statusCheck();
    }
  }
}

int handleStandardImage(BITMAPINFOHEADER* passport, RGBQUAD* palette) {
  if ((*passport).biCompression == BI_RGB) {
    for (int y = 0; y < (*passport).biHeight; y++) {
       handleUncompressedImage(palette, passport);
       endline();
    }
    status = STATUS_END_OF_FILE;

  } else if ((*passport).biCompression == BI_RLE8) {
    if (isAbsoluteMode) {
      absoluteModeHanler(palette);
    } else {
      if (status == STATUS_END_OF_FILE) {
        endline();
        return status;
      }

      else {
        handleCompressedImage(palette);
      }
    }
  }
  return status;
}

int calculateBoxAverage(int boxScale, uint16_t *finalColor, int startX, int endX) {
  uint64_t sumR = 0;
  uint64_t sumG = 0;
  uint64_t sumB = 0;
  int count = 0;
  for (int y = 0; y < boxScale; y++) {
    for (int x = startX; x < endX; x++) {
    
      if (x>=MAX_SCALED_WIDTH) break;
      
      uint8_t colorRed = (lineBuffer[y][x] >> 11) & 0x1F;
      uint8_t colorGreen = (lineBuffer[y][x] >> 5) & 0x3F;
      uint8_t colorBlue =  (lineBuffer[y][x]) & 0x1F;
     
      sumR += colorRed;
      sumG += colorGreen;
      sumB += colorBlue;
      count++;
    }
  }
  uint16_t finalR = sumR / count;
  uint16_t finalG = sumG / count;
  uint16_t finalB = sumB / count;

  (*finalColor) = (finalB | finalG << 5 | finalR << 11);

  return OK;
}

int handleScaledImage(BITMAPINFOHEADER* passport, RGBQUAD* palette) {
  if (passport->biWidth > MAX_SCALED_WIDTH || passport->biHeight > MAX_SCALED_HEIGHT) {
    return ERR_IMAGE_NOT_SUPPORTED;
  }
  float scaleX = (float)passport->biWidth / LCD_X_MAXPIXEL;
 float scaleY = (float)abs(passport->biHeight) / LCD_Y_MAXPIXEL;
  float scaleRAW = (scaleX < scaleY) ? scaleY : scaleX;

  /*
  if ((*passport).biCompression == BI_RGB) {
    reset();
    for (int y = 0; y < LCD_Y_MAXPIXEL; y++) {
      currentRecordY = 0;
      for (int row = 0; row < boxScale; row++) {
        currentRecordX = 0;
        handleUncompressedImage(palette, passport);
        currentRecordY++;
      }
      for (int x = 0; x < LCD_X_MAXPIXEL; x++) {
        int startX = (int) (x*scaleRAW);
        int endX = (int) ((x+1)*(scaleRAW));
        uint16_t finalColor = 0;
        calculateBoxAverage(boxScale, &finalColor, startX, endX);
        updateAbsolutLineToDraw(finalColor);
      }
       endline();
    }
    status = STATUS_END_OF_FILE;
  }
 */

  if ((*passport).biCompression == BI_RLE8) {
    reset();
    int linesProcessed = 0;
    for (int y = 0; y < LCD_Y_MAXPIXEL; y++) {
      
      int targetFileRow = (int) ((y+1) * scaleRAW);
      int linesToRead = targetFileRow - linesProcessed;

      currentRecordY = 0; 
      for (int i = 0; i < linesToRead; i++) {
        currentRecordX = 0; 
        status = OK;       
        while (status != STATUS_EOL && status != STATUS_END_OF_FILE) {
          if (isAbsoluteMode) {
            absoluteModeHanler(palette);
          } 
          else {
            handleCompressedImage(palette);
          }
        }
        if (status == STATUS_EOL) {
          status = OK;
          linesProcessed++;
        }
        if (status != STATUS_END_OF_FILE) {
          currentRecordY++;
        } else {
          break; 
        }
      }
      
      for (int x = 0; x < LCD_X_MAXPIXEL; x++) {
        int startX = (int)(x*scaleRAW);
        int endX = (int)((x+1)*(scaleRAW));

        uint16_t finalColor = 0;
        calculateBoxAverage(linesToRead, &finalColor, startX, endX);
        updateAbsolutLineToDraw(finalColor);
      }
      
      endline(); 
      
      if (status == STATUS_END_OF_FILE) {
        return status;
      }
    }
  }
}

int imageHandler(BITMAPINFOHEADER* passport, RGBQUAD* palette) {
  bool isBig = (passport->biWidth >480) || (passport->biHeight >320);
  if(isBig) {
    mode = MODE_RECORDING;
    status = handleScaledImage(passport, palette);
  }
  else {
    mode = MODE_DRAW;
    
    while ((handleStandardImage(passport, palette))!=STATUS_END_OF_FILE) {

    }
  }
  return status;
}
