


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
    xCoords = 0; 
    yCoords = 319;
    (coords).x = xCoords;
    (coords).y = yCoords;
}

int checkYBounds(uint32_t yCoords) {
    if (yCoords< LCD_HEIGHT) {
        return OK;
    }
    else return ERR_OUT_OF_BOUNDS;
}
int checkXBounds(uint32_t xCoords) {
    if (xCoords< LCD_WIDTH) {
        return OK;
    }
    else return ERR_OUT_OF_BOUNDS;
}
int updateCoords() {
    int rcXBounds = checkXBounds(xCoords);
    int rcYBounds = checkYBounds(yCoords);
    if (rcXBounds == OK) {
        xCoords++;  
    }
    if (xCoords == bildWidth && rcYBounds == OK) {
        xCoords = 0;
        yCoords--;
    }
    (coords).x = xCoords;
    (coords).y = yCoords;
    return rcYBounds;
}



int displayDrawRLE(uint16_t lcdColor, int size){
    for (int i = 0; i < size; i++) {
        GUI_drawPoint(coords, lcdColor, DOT_PIXEL_1X1, DOT_FILL_AROUND);
        updateCoords();
    }
    return OK;
}

int displayDrawAbsolut(uint16_t lcdColor) {
     GUI_drawPoint(coords, lcdColor, DOT_PIXEL_1X1, DOT_FILL_AROUND);
     updateCoords();
     return OK;
}

int updateDelta(uint32_t x, uint32_t y) {
    int rcX = checkXBounds(xCoords + x);
    if (rcX != OK) {
        return rcX;
    }
    xCoords += x;
    int rcY = checkYBounds(yCoords - y);
    if (rcY != OK) {
        return rcY;
    }
    yCoords -= y;
    return 0;
}

int endline() {
    int rcY = checkYBounds(yCoords - 1);
    if (rcY != OK) {
        return rcY;
    }
    yCoords--;
    xCoords = 0;
    coords.x = xCoords;
    coords.y = yCoords;
    return rcY;
}