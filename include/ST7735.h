/*
 * ST7735.h
 *
 *  Created on: Oct 21, 2025
 *      Author: HU
 */
/* Colour codes must be defined in main.cpp
 * #define		RED		0xF800	// Red
 * #define		GREEN	0x07E0	// Green
 * #define		BLUE	0x001F	// Blue
 * #define		YELLOW	0xFFE0	// Yellow
 * #define		MAGENTA	0xF81F	// Magenta
 * #define		CYAN	0x07FF	// Cyan
 * #define		WHITE	0xFFFF	// White
 * #define		BLACK	0x0000	// Black
 */
#ifndef INC_ST7735_H_
#define INC_ST7735_H_

#include "main.h"

#include "Font.h"	//Font Bitmap file

#ifdef __cplusplus		//wrapper
extern "C" {
#endif

void ST7735_Init(void);
void ST7735_DrawChar(uint8_t x, uint8_t y, const char c, uint16_t textColor, uint16_t bgColor);
void ST7735_DrawString(uint8_t x, uint8_t y, const char *str, uint16_t textColor, uint16_t bgColor);
void ST7735_FillScreen(uint16_t color);
void ST7735_FillRect(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint16_t color);

#ifdef __cplusplus
}
#endif

#endif /* INC_ST7735_H_ */
