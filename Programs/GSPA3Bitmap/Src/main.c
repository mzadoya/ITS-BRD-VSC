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
uint16_t secondLcdColor = 0;
int absoluteSize = 0;
bool isAligned = false;
int status = OK;
unsigned char trash;


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
		absoluteSize = nextByte[1];
		if (absoluteSize %2 !=0) {
			isAligned = false;
		}
		else {
			isAligned = true;
		}
		isAbsoluteMode = true;
		break;
	
		default:


		break;
	}
}

int main(void) {
    initITSboard(); 
    GUI_init(DEFAULT_BRIGHTNESS);
    TP_Init(false); 
    initInput();

    while(1) {
        // --- БЛОК 1: ЗАГРУЗКА ---
        buttons = GPIOF->IDR;

        if (nextFileReady) {
            openNextFile();
            GUI_clear(0xFFFF);
            int rcHeaders = readHeaders();

            if (rcHeaders != OK) {
                errorHandler(rcHeaders);
                isFileCompleted = true; // Останавливаемся при ошибке
            }
            else {
                getFileHeader(&fileHeader);
                getInfoHeader(&passport);
                readPalette(palette);
                // Пропускаем мусор (используем bfOffBits аккуратно)
                skipTrash(fileHeader.bfOffBits); 
                initCoords(passport.biWidth, passport.biHeight);
                isFileCompleted = false;
            }
            nextFileReady = false;
        }

        buttonHandler(&nextFileReady, buttons);

        // --- БЛОК 2: ДЕКОДИРОВАНИЕ ---
        if (!isFileCompleted) {
            
            // ВЕТКА А: АБСОЛЮТНЫЙ РЕЖИМ
            if (isAbsoluteMode) {
                // 1. Читаем один пиксель
                COMread(nextByte, 1, 1); 
                
                // 2. Рисуем его
                convertColor(palette[nextByte[0]], &lcdColor);
                displayDrawAbsolut(lcdColor);

                // 3. Уменьшаем счетчик
                absoluteSize--;
                
                // 4. Проверяем конец блока
                if (absoluteSize == 0) {
                    isAbsoluteMode = false; // ВЫХОДИМ ИЗ РЕЖИМА!
                    
                    // Если было нечетное кол-во байт, читаем паддинг
                    if (!isAligned) {
                        COMread(&trash, 1, 1);
                    }
                }
            }
            // ВЕТКА Б: КОМАНДЫ / RLE
            else {
                // 1. Читаем команду (2 байта)
                COMread(nextByte, 1, 2);

                status = inputRecognizer(nextByte);

                if (status == STATUS_DELTA) {
                    // ИСПРАВЛЕНИЕ DELTA: Читаем аргументы!
                    unsigned char deltaArgs[2];
                    COMread(deltaArgs, 1, 2);
                    updateDelta(deltaArgs[0], deltaArgs[1]);
                    status = OK;
                }
                else if (status == STATUS_RLE) {
                    convertColor(palette[nextByte[1]], &lcdColor);
                    displayDrawRLE(lcdColor, nextByte[0]);
                }
                
                // Проверяем остальные статусы (EOL, EOF, START_ABSOLUTE)
                statusCheck();
            }
        }
    }
}