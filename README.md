# 📺 ST7735 Text-Only Display Driver (FreeSans Font) for STM32

A lightweight and minimal ST7735 TFT library for STM32 designed purely for **fast, clean text rendering** — no graphics, no UI framework, no heavy overhead.

Perfect for embedded applications where you only need readable text output on a small TFT.

---

## 🚀 Why This Library?

| Standard GFX Libraries | This Library |
|---|---|
| Large + feature heavy | 🔥 Small + lightweight |
| Includes UI & vector support | 🟢 Text only (fast) |
| Requires more Flash + RAM | 🧠 Tiny memory footprint |
| Best for GUI apps | ⚡ Best for data display & debug text |

If all you want is to **print text clearly and efficiently**, this is the right tool.

---

## ✨ Features

- 📌 TrueType-style FreeSans font rendering (smooth & readable)
- ⚡ No dynamic memory allocation (`malloc`-free)
- 🔥 Lightweight — ideal for small MCUs
- 🎯 Designed specifically for text output
- 🔄 Multi-line display supported
- 🧩 Works on STM32 HAL (F0 / F1 / F3 / F4 / G4 / H7 etc.)

---

## 📸 Output Preview (Real Hardware Demo)

YouTube Short (live output test):  
🔗 https://youtube.com/shorts/BZm0dHGuec4?si=T_2y-ZV62Of7I5wn

---

## 🏁 Getting Started

```c
#include "ST7735.h"
#define		RED		0xF800   // Red
#define		GREEN	0x07E0   // Green
#define		BLUE	0x001F   // Blue
#define		YELLOW	0xFFE0 // Yellow
#define		MAGENTA	0xF81F // Magenta
#define		CYAN	0x07FF   // Cyan
#define		WHITE	0xFFFF   // White
#define		BLACK	0x0000   // Black

ST7735_Init();
ST7735_FillScreen(BLACK);
HAL_Delay(100);

ST7735_DrawString(0, 20, "Hello STM32!", WHITE, BLACK);
ST7735_DrawString(0, 40, "ST7735", YELLOW, BLACK);

```
## 🔧 SPI Configuration

Default LCD interface → hspi1
To change SPI instance: Modify inside ST7735.c

```c
extern SPI_HandleTypeDef hspi1;   // if needed replace with hspi2 or hspi3
```

## 🔤 Using Custom Fonts

You can replace the bundled FreeSans font with any Adafruit-GFX compatible TrueType bitmap font.

Font collection:
🔗 https://gitlab.com/Matthias_C/Adafruit-GFX-Library/-/tree/master/Fonts

Replace Font Steps:
1. Open your desired font .h file
2. Copy the glyph + bitmap arrays
3. Paste into Font.c (replace both existing arrays)
4. Rebuild → new font displayed immediately



