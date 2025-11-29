/*
 * ST7735.c
 *
 *  Created on: Oct 21, 2025
 *      Author: HU
 */

#include "ST7735.h"

/*
 * RESET_PIN -> ACTIVE_LOW
 * CS_PIN -> ACTIVE_LOW
 * AO_PIN ->(LOW=command, HIGH=data)
 */

extern SPI_HandleTypeDef hspi1; // SP1 in this case

#define RESET_LOW()   HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_RESET)
#define RESET_HIGH()  HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_SET)

#define CS_LOW()   HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET)
#define CS_HIGH()  HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET)

#define AO_LOW()   HAL_GPIO_WritePin(AO_GPIO_Port, AO_Pin, GPIO_PIN_RESET)
#define AO_HIGH()  HAL_GPIO_WritePin(AO_GPIO_Port, AO_Pin, GPIO_PIN_SET)

static void ST7735_WriteCommand(uint8_t cmd);
static void ST7735_WriteData(uint8_t data);
static void ST7735_SetAddressWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);

void ST7735_Init(void)
{
    // --- Hardware Reset ---
    RESET_LOW();
    HAL_Delay(100);
    RESET_HIGH();
    HAL_Delay(100);

    // --- Software Reset ---
    ST7735_WriteCommand(0x01); // Software reset
    HAL_Delay(100);

    // --- Sleep Out ---
    ST7735_WriteCommand(0x11);
    HAL_Delay(100);

    /*
    // --- Frame Rate Control ---
    ST7735_WriteCommand(0xB1);
    ST7735_WriteData(0x01);
    ST7735_WriteData(0x2C);
    ST7735_WriteData(0x2D);

    // --- Power Control 1 ---
    ST7735_WriteCommand(0xC0);
    ST7735_WriteData(0xA2);
    ST7735_WriteData(0x02);
    ST7735_WriteData(0x84);

    // --- Power Control 2 ---
    ST7735_WriteCommand(0xC1);
    ST7735_WriteData(0xC5);

    // --- VCOM Control 1 ---
    ST7735_WriteCommand(0xC5);
    ST7735_WriteData(0x0A);
    ST7735_WriteData(0x00);

    */

    // --- Memory Access Control ---
    ST7735_WriteCommand(0x36);
    ST7735_WriteData(0xC0);  // rotation, RGB/BGR order, row/column exchange
    //ST7735_WriteData(0xC8); // switch RGB ↔ BGR


    // --- Color Mode ---
    ST7735_WriteCommand(0x3A);
    ST7735_WriteData(0x05);  // 16-bit color

    // --- Display ON ---
    ST7735_WriteCommand(0x29);
    HAL_Delay(100);
}
static void ST7735_WriteCommand(uint8_t cmd)
{
    AO_LOW();     // Command mode
    CS_LOW();     // Select LCD
    HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);
    CS_HIGH();    // Deselect LCD
}
static void ST7735_WriteData(uint8_t data)
{
    AO_HIGH();    // Data mode
    CS_LOW();
    HAL_SPI_Transmit(&hspi1, &data, 1, HAL_MAX_DELAY);
    CS_HIGH();
}
void ST7735_SetAddressWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
    ST7735_WriteCommand(0x2A); // Column address set
    ST7735_WriteData(0x00);
    ST7735_WriteData(x0);
    ST7735_WriteData(0x00);
    ST7735_WriteData(x1);
    ST7735_WriteCommand(0x2B); // Row address set
    ST7735_WriteData(0x00);
    ST7735_WriteData(y0);
    ST7735_WriteData(0x00);
    ST7735_WriteData(y1);
    ST7735_WriteCommand(0x2C); // Memory write for writing Pixels
}
void ST7735_FillScreen(uint16_t color) {
    ST7735_SetAddressWindow(0, 0, 128, 160); // for 128x160 display
    uint8_t data[] = { color >> 8, color & 0xFF }; // high byte first
    AO_HIGH();
    CS_LOW();
    for (uint16_t y = 0; y < 160; y++) {
        for (uint16_t x = 0; x < 128; x++) {
            HAL_SPI_Transmit(&hspi1, data, 2, HAL_MAX_DELAY);
        }
    }
    CS_HIGH();
}
void ST7735_FillRect(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint16_t color) {
    ST7735_SetAddressWindow(x1, y1, x2, y2); // for 128x160 display
    uint8_t data[] = { color >> 8, color & 0xFF }; // high byte first
    AO_HIGH();
    CS_LOW();
    for (uint16_t y = 0; y < y2-y1; y++) {
        for (uint16_t x = 0; x < x2-x1; x++) {
            HAL_SPI_Transmit(&hspi1, data, 2, HAL_MAX_DELAY);
        }
    }
    CS_HIGH();
}
void ST7735_DrawChar(uint8_t x, uint8_t y, const char c, uint16_t textColor, uint16_t bgColor)
{
    if (c < 0x20 || c > 0x7E) return;
    const GFXglyph *glyph = &font_Glyphs[c - 0x20];
    const uint8_t *bitmap = &font_Bitmaps[glyph->bitmapOffset];
    uint8_t w = glyph->width;
    uint8_t h = glyph->height;
    int8_t xo = glyph->xOffset;
    int8_t yo = glyph->yOffset;
    ST7735_SetAddressWindow(x + xo , y + yo , x + xo + w - 1 , y + yo + h - 1);
    AO_HIGH();
    CS_LOW();
    uint8_t bit = 0, bits = 0;
    for (uint8_t yy = 0; yy < h; yy++) {
        for (uint8_t xx = 0; xx < w; xx++) {
            if (!(bit++ & 7)) bits = *bitmap++;
            uint16_t color = (bits & 0x80) ? textColor : bgColor;
            uint8_t data[] = { color >> 8, color & 0xFF };
            HAL_SPI_Transmit(&hspi1, data, 2, HAL_MAX_DELAY);
            bits <<= 1;
        }
    }
    CS_HIGH();
}
void ST7735_DrawString(uint8_t x, uint8_t y, const char *str, uint16_t textColor, uint16_t bgColor)
{
    while (*str)
    {
        char c = *str++;
        if (c < 0x20 || c > 0x7E) continue;  // skip unsupported chars
        const GFXglyph *glyph = &font_Glyphs[c - 0x20];
        const uint8_t *bitmap = &font_Bitmaps[glyph->bitmapOffset];
        uint8_t w = glyph->width;
        uint8_t h = glyph->height;
        int8_t xo = glyph->xOffset;
        int8_t yo = glyph->yOffset;
        ST7735_SetAddressWindow(x + xo , y + yo , x + xo + w - 1 , y + yo + h - 1);
        AO_HIGH();
        CS_LOW();
        uint8_t bit = 0, bits = 0;
        for (uint8_t yy = 0; yy < h; yy++) {
            for (uint8_t xx = 0; xx < w; xx++) {
                if (!(bit++ & 7)) bits = *bitmap++;
                uint16_t color = (bits & 0x80) ? textColor : bgColor;
                uint8_t data[2] = { color >> 8, color & 0xFF };
                HAL_SPI_Transmit(&hspi1, data, 2, HAL_MAX_DELAY);
                bits <<= 1;
            }
        }
        CS_HIGH();
        // Move cursor for next char
        x += glyph->xAdvance;
    }
}
