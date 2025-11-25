


#include "LCD_general.h"
#include <stdint.h>
#include "eCodes.h"
#include "LCD_GUI.h"

uint32_t xCoords = 0;
uint32_t yCoords = 320;
uint8_t colorsSize = 0; //TODO rename
Coordinate coords;
uint32_t bildWidth = 0;
uint32_t bildHeight = 0;

void initCoords(int32_t width, int32_t height) {
    bildHeight = height;
    bildWidth = width;
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
        (coords).x = xCoords;
         xCoords++;
    }
    if (xCoords == bildWidth && rcYBounds == OK) {
        xCoords = 0;
        (coords).x = xCoords;
        (coords).y = --yCoords;
    }
    
    return rcYBounds;
}

int inputRecognizer(char* info) { //TODO rename 

    if (info[0] == 0x00) {
    switch (info[1])
    {
        case 0x00:
            yCoords--;
            xCoords = 0;
            break;
        case 0x01:
            return STATUS_END_OF_FILE;
            break;
        /*case 0x02:
      
            break;

        case 0x03:
      
            break;*/
        default:
            break;
        }   
    }
    else {
        colorsSize = info[0];
        return STATUS_COLOR;
    }
    return OK;
}

int displayDraw(uint16_t lcdColor){
    for (int i = 0; i < colorsSize; i++) {
        GUI_drawPoint(coords, lcdColor, DOT_PIXEL_1X1, DOT_FILL_AROUND);
        updateCoords();
    }
    colorsSize = 0;
    return OK;
}