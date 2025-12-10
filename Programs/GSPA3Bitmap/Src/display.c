#include "display.h"
#include "LCD_general.h"
#include <stdint.h>
#include "eCodes.h"
#include "LCD_GUI.h"

#define MAX_LCD_WIDTH LCD_WIDTH 

uint32_t xCoords = 0;
uint32_t yCoords = 0;
uint8_t colorsSize = 0; //TODO rename
Coordinate coords;
uint32_t bildWidth = 0;
uint32_t bildHeight = 0;

//NEW for Part B 
int currentPos;
uint16_t lcdColorMap[LCD_WIDTH];
int bufferSize = 0;



void initCoords(int32_t width, int32_t height) {
    bildHeight = height;
    bildWidth = width;
    xCoords = LCD_X; 
    yCoords = LCD_HEIGHT - 1;
    (coords).x = xCoords;
    (coords).y = yCoords;
    
    //NEW for Part B 
    currentPos = 0;
    if (width > MAX_LCD_WIDTH) {
        bufferSize = MAX_LCD_WIDTH;
    }
    else {
        bufferSize = width;
    }
}

int checkYBounds(uint32_t yCoords) {
    if (yCoords< LCD_HEIGHT) { //Moglieche Fehler bei =
        return OK;
    }
    else return ERR_OUT_OF_BOUNDS;
}
int checkXBounds(uint32_t xCoords) {
    if (xCoords< LCD_WIDTH) { //Moglieche Fehler bei =
        return OK;
    }
    else return ERR_OUT_OF_BOUNDS;
}

int checkBounds(uint32_t xCoords, uint32_t yCoords) {
    int rcX = checkXBounds(xCoords);
    if (rcX != OK) {
        return rcX;
    }
    int rcY = checkYBounds(yCoords);
    if (rcY != OK) {
        return rcY;
    }
    return OK;
}

int updateCoords() {
    int rcXBounds = checkXBounds(xCoords);
    int rcYBounds = checkYBounds(yCoords);
    if (rcXBounds == OK) {
        xCoords++;  
    }
    
    (coords).x = xCoords;
    (coords).y = yCoords;
    return rcYBounds;
}

int updateDelta(uint32_t x, uint32_t y) {
    int rcBounds = checkBounds(xCoords+x, yCoords - y);
    if (rcBounds != OK) {
        return rcBounds;
    }
    xCoords += x;
    yCoords -= y;

    (coords).x = xCoords;
    (coords).y = yCoords;
    
    return OK;
}

//PART B ----
int updateRLELine(uint16_t lcdColor, int size){
    for (int i = 0; i < size; i++) {
        if (currentPos < bufferSize) {
            lcdColorMap[currentPos] = lcdColor;
            currentPos++;
        }
    }
 
    return OK;
}

int updateAbsolutLineToDraw(uint16_t lcdColor) {
     if (currentPos < bufferSize) {
        lcdColorMap[currentPos] = lcdColor;
        currentPos++;
     }
     return OK;
}

int lcdPrintLine() {
    GUI_WriteLine(
    coords, bufferSize, lcdColorMap);
    currentPos = 0;
    return OK;
}

int endline() {
    //PART B
    lcdPrintLine();
    int rcY = checkYBounds(yCoords - 1);
    if (rcY != OK) {
        return rcY;
    }
    yCoords--;
    xCoords = LCD_X;
    coords.x = xCoords;
    coords.y = yCoords;
    return rcY;
}
//END PART B----








/*
//PART A ----
int displayDrawRLE(uint16_t lcdColor, int size){
    for (int i = 0; i < size; i++) {
        int rcBounds = checkBounds(coords.x, coords.y);
        if (rcBounds != OK) {
            return rcBounds;
        }
        GUI_drawPoint(coords, lcdColor, DOT_PIXEL_1X1, DOT_FILL_AROUND);
        updateCoords();
    }
    return OK;
}

int displayDrawAbsolut(uint16_t lcdColor) {
     int rcBounds = checkBounds(coords.x, coords.y);
     if (rcBounds != OK) {
        return rcBounds;
     }
     GUI_drawPoint(coords, lcdColor, DOT_PIXEL_1X1, DOT_FILL_AROUND);
     updateCoords();
     return OK;
}
//END PART A ----

*/