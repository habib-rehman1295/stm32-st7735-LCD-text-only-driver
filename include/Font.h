/*
 * FreeSans10pt.h
 *
 *  Created on: Oct 23, 2025
 *      Author: HU
 */

#ifndef INC_FONT_H_
#define INC_FONT_H_

#include <stdint.h>

#ifdef __cplusplus		//wrapper
extern "C" {
#endif

typedef struct {
  uint16_t bitmapOffset; // starting index inside the bitmap array
  uint8_t  width;        // width in pixels
  uint8_t  height;       // height in pixels
  uint8_t  xAdvance;     // how much to move cursor after drawing
  int8_t   xOffset;      // x offset from cursor to start of glyph
  int8_t   yOffset;      // y offset from cursor to top of glyph
} GFXglyph;

extern const uint8_t font_Bitmaps[];
extern const GFXglyph font_Glyphs[];

#ifdef __cplusplus
}
#endif

#endif /* INC_FREESANS10PT_H_ */
