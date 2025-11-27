


#include "LCD_general.h"
#include <stdint.h>
#include "eCodes.h"
#include "LCD_GUI.h"

uint32_t xCoords = 0;
uint32_t yCoords = 0;
uint8_t colorsSize = 0; //TODO rename
Coordinate coords;
uint32_t bildWidth = 0;
uint32_t bildHeight = 0;

void initCoords(int32_t width, int32_t height) {
    bildHeight = height;
    bildWidth = width;
    xCoords = LCD_X; 
    yCoords = LCD_HEIGHT - 1;
    (coords).x = xCoords;
    (coords).y = yCoords;
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
    if (xCoords == bildWidth && rcYBounds == OK) {
        xCoords = LCD_X;
        yCoords--;
    }
    (coords).x = xCoords;
    (coords).y = yCoords;
    return rcYBounds;
}

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

int updateDelta(uint32_t x, uint32_t y) {
    int rcBounds = checkBounds(xCoords+x, yCoords - y);
    if (rcBounds != OK) {
        return rcBounds;
    }
    xCoords += x;
    yCoords -= y;
    return 0;
}

int endline() {
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