//------------------------------------------------------------------------------
// File:          tft.c
// Written By:    E.G. Walters
// Date Created:  5 Jan 15
// Description:   C file for TFT display
// Compiler:	  XC32 v1.34
// Target:        Adafruit TFT Capacitive Touch Shield
// Schematic(s):  Adafruit schematic for TFT shield
//
// NOTE: tft.h and tft.c were originally ported from Adafruit_ILI9341-master
//   library downloaded from Adafruit.com
//
// Revision History (date, initials, description)
//   27 Feb 18, egw100, Updated for SP18
//------------------------------------------------------------------------------
#include "tft.h"


uint8_t i, j; // index variables


//------------------------------------------------------------------------------
// Function: TFT_DrawMonochromeSprite()
// Params:   x;           x coordinate of sprite
//           y;           y coordinate of sprite
//           width;       width of sprite
//           height;      height of sprite
//           sprite;      pointer to sprite
//           spriteColor; color of sprite
//           backColor;   color of background
// Returns:  void
// (x, y) is top left corner.  Width and height are in pixels.  This function
// sends stream of pixels to a window so it is fast, but must send spritecolor
// or backcolor so it can not do sprite with transparency.
//------------------------------------------------------------------------------
void TFT_DrawMonochromeSprite(int16_t x, int16_t y, int16_t width, int16_t height,
        uint8_t *sprite, uint16_t spriteColor, uint16_t backColor) {

    uint8_t bytemask;
    uint8_t spritehi = spriteColor >> 8;
    uint8_t spritelo = spriteColor;
    uint8_t backhi = backColor >> 8;
    uint8_t backlo = backColor;
    
    TFT_SetAddrWindow(x, y, x + width - 1, y + height - 1);
    
    TFT_DC = 1;
    TFT_CS = 0;
    for (j = 0; j < height; j++) {
        bytemask = 0x80;
        for (i = 0; i < width; i++) {
            if (*sprite & bytemask) {
                // bit is on
                SPI2_TransmitByte(spritehi);
                SPI2_TransmitByte(spritelo);
            } else {
                // bit is off
                SPI2_TransmitByte(backhi);
                SPI2_TransmitByte(backlo);
            }
            bytemask >>= 1;
            if (bytemask == 0) {
                bytemask = 0x80;
                sprite++;
            }
        } // end for i
        if (bytemask != 0x80) {
            sprite++;
        }
    } // end for j
    TFT_CS = 1;

} // end TFT_DrawMonochromeSprite()



//------------------------------------------------------------------------------
// Function: TFT_DrawString()
// Params:   x;         x coordinate of string
//           y;         y coordinate of string
//           str;       string of character to draw
//           charColor; color of string
//           backColor; color of background
//           size;      size of characters (1 = base size, 2 = 2x size, 3 = 3x size, etc.)
// Returns:  void
// Draws a string of ASCII characters
//------------------------------------------------------------------------------
void TFT_DrawString(int16_t x, int16_t y, const uint8_t *str, uint16_t charColor,
        uint16_t backColor, uint8_t size) {

    while (*str != 0x00) {
        TFT_DrawChar(x, y, *str, charColor, backColor, size);
        *str++;
        x += size * 6;
    }

} // end TFT_DrawString()



//------------------------------------------------------------------------------
// Function: TFT_DrawChar()
// Params:   x;         x coordinate of character
//           y;         y coordinate of character
//           character; ASCII value of character to draw
//           charColor; color of character
//           backColor; color of background
//           size;      size of character (1 = base size, 2 = 2x size, 3 = 3x size, etc.)
// Returns:  void
// Draws an ASCII character
//------------------------------------------------------------------------------
void TFT_DrawChar(int16_t x, int16_t y, uint8_t character, uint16_t charColor,
        uint16_t backColor, uint8_t size) {

    if(   (x >= TFT_WIDTH)            // Clip right
       || (y >= TFT_HEIGHT)           // Clip bottom
       || ((x + 6 * size - 1) < 0)    // Clip left
       || ((y + 8 * size - 1) < 0)) { // Clip top
        return;
    }

    uint8_t line;
    for (i = 0; i < 6; i++ ) {
        if (i == 5) {
            line = 0x0;
        } else{
            line = font[(character * 5) + i];
        }
        for (j = 0; j < 8; j++) {
            if (line & 0x1) {
                if (size == 1) { // default size
                    TFT_DrawPixel(x + i, y + j, charColor);
                } else {  // big size
                    TFT_FillRectangle(x + (i*size), y + (j*size), size, size, charColor);
                }
            } else if (backColor != charColor) {
                if (size == 1) { // default size
                    TFT_DrawPixel(x + i, y + j, backColor);
                } else {  // big size
                    TFT_FillRectangle(x + i*size, y + j*size, size, size, backColor);
                }
            }
            line >>= 1;
        }
    }

} // end TFT_DrawChar()



//------------------------------------------------------------------------------
// Function: TFT_DrawVLine()
// Params:   x;      x coordinate of left endpoint of line
//           y;      y coordinate of left endpoint of line
//           height; height of line
//           color;  color of line
// Returns:  void
// Draws a vertical line
//------------------------------------------------------------------------------
void TFT_DrawVLine(int16_t x, int16_t y, int16_t height, uint16_t color) {

    // Rudimentary clipping
    if((x >= TFT_WIDTH) || (y >= TFT_HEIGHT)) {
        return;
    }
    if((y + height - 1) >= TFT_HEIGHT) {
        height = TFT_HEIGHT - y;
    }

    TFT_SetAddrWindow(x, y, x, y + height - 1);

    uint8_t hi = color >> 8;
    uint8_t lo = color;

    TFT_DC = 1;
    TFT_CS = 0;
    while (height--) {
        SPI2_TransmitByte(hi);
        SPI2_TransmitByte(lo);
    }
    TFT_CS = 1;

} // end TFT_DrawVLine()



//------------------------------------------------------------------------------
// Function: TFT_DrawHLine()
// Params:   x;     x coordinate of left endpoint of line
//           y;     y coordinate of left endpoint of line
//           width; width of line
//           color; color of line
// Returns:  void
// Draws a horizontal line
//------------------------------------------------------------------------------
void TFT_DrawHLine(int16_t x, int16_t y, int16_t width, uint16_t color) {

    // Rudimentary clipping
    if((x >= TFT_WIDTH) || (y >= TFT_HEIGHT)) {
        return;
    }
    if((x + width - 1) >= TFT_WIDTH) {
        width = TFT_WIDTH - x;
    }

    TFT_SetAddrWindow(x, y, x + width - 1, y);

    uint8_t hi = color >> 8;
    uint8_t lo = color;

    TFT_DC = 1;
    TFT_CS = 0;
    while (width--) {
        SPI2_TransmitByte(hi);
        SPI2_TransmitByte(lo);
    }
    TFT_CS = 1;

} // TFT_DrawHLine()



//------------------------------------------------------------------------------
// Function: TFT_DrawLine()
// Params:   x0;    x coordinate of first endpoint of line
//           y0;    y coordinate of first endpoint of line
//           x1;    x coordinate of second endpoint of line
//           y1;    y coordinate of fisecondrst endpoint of line
//           color; color of line
// Returns:  void
// Draws a line
//------------------------------------------------------------------------------
// Bresenham's algorithm - thx wikpedia
void TFT_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {

    int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    int16_t temp;

    if (steep) {
        temp = x0; x0 = y0, y0 = temp;
        temp = x1; x1 = y1, y1 = temp;
    }

    if (x0 > x1) {
        temp = x0; x0 = x1; x1 = temp;
        temp = y0; y0 = y1; y1 = temp;
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int16_t err = dx >> 1;
    int16_t ystep;

    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for (; x0 <= x1; x0++) {
        if (steep) {
            TFT_DrawPixel(y0, x0, color);
        } else {
            TFT_DrawPixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }

} // end TFT_DrawLine()



//------------------------------------------------------------------------------
// Function: TFT_DrawTriangle()
// Params:   x0;    x coordinate of first virtex of triangle
//           y0;    y coordinate of first virtex of triangle
//           x1;    x coordinate of second virtex of triangle
//           y1;    y coordinate of second virtex of triangle
//           x2;    x coordinate of third virtex of triangle
//           y2;    y coordinate of third virtex of triangle
//           color; color of triangle
// Returns:  void
// Draws a rectangle
//------------------------------------------------------------------------------
void TFT_DrawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
        int16_t x2, int16_t y2, uint16_t color) {

    TFT_DrawLine(x0, y0, x1, y1, color);
    TFT_DrawLine(x1, y1, x2, y2, color);
    TFT_DrawLine(x2, y2, x0, y0, color);

} // end TFT_DrawTriangle()



//------------------------------------------------------------------------------
// Function: TFT_FillTriangle()
// Params:   x0;    x coordinate of first virtex of triangle
//           y0;    y coordinate of first virtex of triangle
//           x1;    x coordinate of second virtex of triangle
//           y1;    y coordinate of second virtex of triangle
//           x2;    x coordinate of third virtex of triangle
//           y2;    y coordinate of third virtex of triangle
//           color; color of triangle
// Returns:  void
// Draws a rectangle
//------------------------------------------------------------------------------
void TFT_FillTriangle ( int16_t x0, int16_t y0, int16_t x1, int16_t y1,
        int16_t x2, int16_t y2, uint16_t color) {

    int16_t a, b, y, last;
    int16_t temp;

    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1) {
        temp = y0; y0 = y1; y1 = temp;
        temp = x0; x0 = x1; x1 = temp;
    }
    if (y1 > y2) {
        temp = y2; y2 = y1; y1 = temp;
        temp = x2; x2 = x1; x1 = temp;
    }
    if (y0 > y1) {
        temp = y0; y0 = y1; y1 = temp;
        temp = x0; x0 = x1; x1 = temp;
    }

    if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
        a = b = x0;
        if (x1 < a) {
            a = x1;
        }  else if(x1 > b) {
            b = x1;
        }
        if (x2 < a) {
            a = x2;
        } else if (x2 > b) {
            b = x2;
        }
        TFT_DrawHLine(a, y0, b - a + 1, color);
        return;
    }

    int16_t dx01 = x1 - x0;
    int16_t dy01 = y1 - y0;
    int16_t dx02 = x2 - x0;
    int16_t dy02 = y2 - y0;
    int16_t dx12 = x2 - x1;
    int16_t dy12 = y2 - y1;
    int32_t sa = 0;
    int32_t sb = 0;

    // For upper part of triangle, find scanline crossings for segments
    // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
    // is included here (and second loop will be skipped, avoiding a /0
    // error there), otherwise scanline y1 is skipped here and handled
    // in the second loop...which also avoids a /0 error here if y0=y1
    // (flat-topped triangle).
    if (y1 == y2) {
        last = y1;   // Include y1 scanline
    } else {
        last = y1-1; // Skip it
    }

    for (y = y0; y <= last; y++) {
        a = x0 + sa / dy01;
        b = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        /* longhand:
        a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if(a > b) {
            temp = a; a = b; b = temp;
        }
        TFT_DrawHLine(a, y, b - a + 1, color);
    }

    // For lower part of triangle, find scanline crossings for segments
    // 0-2 and 1-2.  This loop is skipped if y1=y2.
    sa = dx12 * (y - y1);
    sb = dx02 * (y - y0);
    for (; y<=y2; y++) {
        a = x1 + sa / dy12;
        b = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        /* longhand:
        a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if (a > b) {
            temp = a; a = b; b = temp;
        }
        TFT_DrawHLine(a, y, b - a + 1, color);
    }

} // end TFT_FillTriangle()





//------------------------------------------------------------------------------
// Function: TFT_DrawCircle()
// Params:   x0;     x coordinate of center of circle
//           y0;     y coordinate of center of circle
//           radius; radius of circle
//           color;  color of circle
// Returns:  void
// Draws a rectangle
//------------------------------------------------------------------------------
void TFT_DrawCircle(int16_t x0, int16_t y0, int16_t radius, uint16_t color) {

    int16_t f = 1 - radius;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * radius;
    int16_t x = 0;
    int16_t y = radius;

    TFT_DrawPixel(x0,          y0 + radius, color);
    TFT_DrawPixel(x0,          y0 - radius, color);
    TFT_DrawPixel(x0 + radius, y0,          color);
    TFT_DrawPixel(x0 - radius, y0,          color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        TFT_DrawPixel(x0 + x, y0 + y, color);
        TFT_DrawPixel(x0 - x, y0 + y, color);
        TFT_DrawPixel(x0 + x, y0 - y, color);
        TFT_DrawPixel(x0 - x, y0 - y, color);
        TFT_DrawPixel(x0 + y, y0 + x, color);
        TFT_DrawPixel(x0 - y, y0 + x, color);
        TFT_DrawPixel(x0 + y, y0 - x, color);
        TFT_DrawPixel(x0 - y, y0 - x, color);
    }

} // end TFT_DrawCircle()



//------------------------------------------------------------------------------
// Function: TFT_DrawCircleHelper()
// Params:   x0;         x coordinate of center of circle
//           y0;         y coordinate of center of circle
//           radius;     radius of circle
//           cornername;
//           color;      color of circle
// Returns:  void
// Used to draw rounded rectangles
//------------------------------------------------------------------------------
void TFT_DrawCircleHelper( int16_t x0, int16_t y0, int16_t radius,
        uint8_t cornername, uint16_t color) {

    int16_t f = 1 - radius;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * radius;
    int16_t x = 0;
    int16_t y = radius;

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        if (cornername & 0x4) {
            TFT_DrawPixel(x0 + x, y0 + y, color);
            TFT_DrawPixel(x0 + y, y0 + x, color);
        }
        if (cornername & 0x2) {
            TFT_DrawPixel(x0 + x, y0 - y, color);
            TFT_DrawPixel(x0 + y, y0 - x, color);
        }
        if (cornername & 0x8) {
            TFT_DrawPixel(x0 - y, y0 + x, color);
            TFT_DrawPixel(x0 - x, y0 + y, color);
        }
        if (cornername & 0x1) {
            TFT_DrawPixel(x0 - y, y0 - x, color);
            TFT_DrawPixel(x0 - x, y0 - y, color);
        }
    }

} // end TFT_DrawCircleHelper()



//------------------------------------------------------------------------------
// Function: TFT_DrawRoundRectangle()
// Params:   x;      x coordinate of top left corner of rectangle
//           y;      y coordinate of top left corner of rectangle
//           w;      width of rectangle
//           h;      height of rectangle
//           radius; radius of corners
//           color;  color of rectangle
// Returns:  void
// Draws a rounded rectangle
//------------------------------------------------------------------------------
void TFT_DrawRoundRectangle(int16_t x, int16_t y, int16_t w, int16_t h,
        int16_t radius, uint16_t color) {

  TFT_DrawHLine(x + radius, y, w - 2 * radius, color); // Top
  TFT_DrawHLine(x + radius, y + h - 1, w - 2 * radius, color); // Bottom
  TFT_DrawVLine(x, y + radius, h - 2 * radius, color); // Left
  TFT_DrawVLine(x + w - 1, y + radius, h - 2 * radius, color); // Right

  TFT_DrawCircleHelper(x + radius, y + radius, radius, 1, color);
  TFT_DrawCircleHelper(x + w - radius - 1, y + radius, radius, 2, color);
  TFT_DrawCircleHelper(x + w - radius - 1, y + h - radius - 1, radius, 4, color);
  TFT_DrawCircleHelper(x + radius, y + h - radius - 1, radius, 8, color);

} // end TFT_DrawRoundRectangle()



//------------------------------------------------------------------------------
// Function: TFT_FillRoundRectangle()
// Params:   x;      x coordinate of top left corner of rectangle
//           y;      y coordinate of top left corner of rectangle
//           w;      width of rectangle
//           h;      height of rectangle
//           radius; radius of corners
//           color;  color of rectangle
// Returns:  void
// Draws a rounded rectangle
//------------------------------------------------------------------------------
void TFT_FillRoundRectangle(int16_t x, int16_t y, int16_t w, int16_t h,
        int16_t radius, uint16_t color) {

  TFT_FillRectangle(x + radius, y, w - 2 * radius, h, color);

  TFT_FillCircleHelper(x + w - radius - 1, y + radius, radius, 1, h - 2 * radius - 1, color);
  TFT_FillCircleHelper(x + radius, y + radius, radius, 2, h - 2 * radius - 1, color);

} // end TFT_FillRoundRectangle()



//------------------------------------------------------------------------------
// Function: TFT_FillCircleHelper()
// Params:   x0;         x coordinate of center of circle
//           y0;         y coordinate of center of circle
//           radius;     radius of circle
//           cornername;
//           delta;
//           color;      color of circle
// Returns:  void
// Used to fill circles and rounded rectangles
//------------------------------------------------------------------------------
void TFT_FillCircleHelper(int16_t x0, int16_t y0, int16_t radius,
        uint8_t cornername, int16_t delta, uint16_t color) {

    int16_t f = 1 - radius;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * radius;
    int16_t x = 0;
    int16_t y = radius;

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        if (cornername & 0x1) {
            TFT_DrawVLine(x0 + x, y0 - y, 2 * y + 1 + delta, color);
            TFT_DrawVLine(x0 + y, y0 - x, 2 * x + 1 + delta, color);
        }
        if (cornername & 0x2) {
            TFT_DrawVLine(x0 - x, y0 - y, 2 * y + 1 + delta, color);
            TFT_DrawVLine(x0 - y, y0 - x, 2 * x + 1 + delta, color);
        }
    }

} // end TFT_FillCircleHelper()



//------------------------------------------------------------------------------
// Function: TFT_FillCircle()
// Params:   x0;     x coordinate of center of circle
//           y0;     y coordinate of center of circle
//           radius; radius of circle
//           color;  color of circle
// Returns:  void
// Draws a solid circle
//------------------------------------------------------------------------------
void TFT_FillCircle(int16_t x0, int16_t y0, int16_t radius, uint16_t color) {

  TFT_DrawVLine(x0, y0 - radius, 2 * radius + 1, color);
  TFT_FillCircleHelper(x0, y0, radius, 3, 0, color);

} // end TFT_FillCircle()




//------------------------------------------------------------------------------
// Function: TFT_DrawRectangle()
// Params:   x;     x coordinate of top left corner of rectangle
//           y;     y coordinate of top left corner of rectangle
//           w;     width of rectangle
//           h;     height of rectangle
//           color; color of rectangle
// Returns:  void
// Draws a rectangle
//------------------------------------------------------------------------------
void TFT_DrawRectangle(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {

    TFT_DrawHLine(x, y, w, color);
    TFT_DrawHLine(x, y + h - 1, w, color);
    TFT_DrawVLine(x, y, h, color);
    TFT_DrawVLine(x + w - 1, y, h, color);

} // end TFT_DrawRectangle()



//------------------------------------------------------------------------------
// Function: TFT_FillRectangle()
// Params:   x;     x coordinate of top left corner of rectangle
//           y;     y coordinate of top left corner of rectangle
//           w;     width of rectangle
//           h;     height of rectangle
//           color; color of rectangle
// Returns:  void
// Draws a solid rectangle
//------------------------------------------------------------------------------
void TFT_FillRectangle(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {

    // rudimentary clipping (drawChar w/big text requires this)
    if((x >= TFT_WIDTH) || (y >= TFT_HEIGHT)) {
        return;
    }
    if((x + w - 1) >= TFT_WIDTH)  {
        w = TFT_WIDTH  - x;
    }
    if((y + h - 1) >= TFT_HEIGHT) {
        h = TFT_HEIGHT - y;
    }

    TFT_SetAddrWindow(x, y, x + w - 1, y + h - 1);

    uint8_t hi = color >> 8;
    uint8_t lo = color;

    TFT_DC = 1;
    TFT_CS = 0;

    for(y = h; y > 0; y--) {
        for(x = w; x > 0; x--) {
            SPI2_TransmitByte(hi);
            SPI2_TransmitByte(lo);
        }
    }

    TFT_CS = 1;

} // end TFT_FillRectangle()



//------------------------------------------------------------------------------
// Function: TFT_DrawPixel()
// Params:   x;     x coordinate of pixel
//           y;     y coordinate of pixel
//           color; color of pixel
// Returns:  void
// Draws a single pixel
//------------------------------------------------------------------------------
void TFT_DrawPixel(int16_t x, int16_t y, uint16_t color) {

  if((x < 0) ||(x >= TFT_WIDTH) || (y < 0) || (y >= TFT_HEIGHT)) {
      return;
  }

  TFT_SetAddrWindow(x, y, x + 1, y + 1);

  TFT_DC = 1;
  TFT_CS = 0;
  SPI2_TransmitByte(color >> 8);
  SPI2_TransmitByte(color);
  TFT_CS = 1;

}  // end TFT_DrawPixel()



//------------------------------------------------------------------------------
// Function: TFT_SetAddrWindow()
// Params:   x0; x coordinate of top left corner of window
//           y0; y coordinate of top left corner of window
//           x1; x coordinate of bottom right corner of window
//           y1; y coordinate of bottom right corner of window
// Returns:  void
// Sets the address window using the given coordinates
//------------------------------------------------------------------------------
void TFT_SetAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {

  TFT_WriteCommandByte(TFT_CASET); // Column addr set
  TFT_WriteDataByte(x0 >> 8);
  TFT_WriteDataByte(x0 & 0xFF);    // XSTART
  TFT_WriteDataByte(x1 >> 8);
  TFT_WriteDataByte(x1 & 0xFF);    // XEND

  TFT_WriteCommandByte(TFT_PASET); // Row addr set
  TFT_WriteDataByte(y0 >> 8);
  TFT_WriteDataByte(y0);           // YSTART
  TFT_WriteDataByte(y1 >> 8);
  TFT_WriteDataByte(y1);           // YEND

  TFT_WriteCommandByte(TFT_RAMWR); // write to RAM

}  // end TFT_SetAddrWindow()



//------------------------------------------------------------------------------
// Function: TFT_InvertDisplay()
// Params:   invert; selects inversion or no inversion
// Returns:  void
// 0 = no inversion, 1 = invert display
//------------------------------------------------------------------------------
void TFT_InvertDisplay(uint8_t invert) {
    
    if (invert) {
        TFT_WriteCommandByte(TFT_INVON);
    } else {
        TFT_WriteCommandByte(TFT_INVOFF);
    }
} // end TFT_InvertDisplay()



//------------------------------------------------------------------------------
// Function: TFT_SetRotation()
// Params:   rotation; rotation to select
// Returns:  void
// Sets screen rotation.  0 = no rotation, 1 = 90 deg, 2 = 180 deg, 3 = 270 deg
// NOTE: This does not rotate touch coordinates
//------------------------------------------------------------------------------
void TFT_SetRotation(uint8_t rotation) {

    TFT_WriteCommandByte(TFT_MADCTL);
    rotation = rotation % 4; // can't be higher than 3
    switch (rotation) {
        case 0:
            TFT_WriteDataByte(0x48);
            break;
        case 1:
            TFT_WriteDataByte(0x28);
            break;
        case 2:
            TFT_WriteDataByte(0x88);
            break;
        case 3:
            TFT_WriteDataByte(0xef);
            break;
    }

} // end TFT_SetRotation()



//------------------------------------------------------------------------------
// Function: TFT_Init()
// Params:   void
// Returns:  void
// Initialization code for TFT shield
// Notes:
//   - SPI must be initialized and ready to use before calling this function
//------------------------------------------------------------------------------
void TFT_Init(void) {

  // Set tristates for TFT
  TFT_CS_TRIS = 0;
  TFT_DC_TRIS = 0;

  // Initialize chip select pin
  TFT_CS = 1;

  msDelay(500);

  TFT_WriteCommandByte(0xEF); // Unknown command
  TFT_WriteDataByte(0x03);
  TFT_WriteDataByte(0x80);
  TFT_WriteDataByte(0x02);

  TFT_WriteCommandByte(0xCF); // Unknown command
  TFT_WriteDataByte(0x00);
  TFT_WriteDataByte(0XC1);
  TFT_WriteDataByte(0X30);

  TFT_WriteCommandByte(0xED); // Unknown command
  TFT_WriteDataByte(0x64);
  TFT_WriteDataByte(0x03);
  TFT_WriteDataByte(0X12);
  TFT_WriteDataByte(0X81);

  TFT_WriteCommandByte(0xE8); // Unknown command
  TFT_WriteDataByte(0x85);
  TFT_WriteDataByte(0x00);
  TFT_WriteDataByte(0x78);

  TFT_WriteCommandByte(0xCB); // Unknown command
  TFT_WriteDataByte(0x39);
  TFT_WriteDataByte(0x2C);
  TFT_WriteDataByte(0x00);
  TFT_WriteDataByte(0x34);
  TFT_WriteDataByte(0x02);

  TFT_WriteCommandByte(0xF7); // Unknown command
  TFT_WriteDataByte(0x20);

  TFT_WriteCommandByte(0xEA); // Unknown command
  TFT_WriteDataByte(0x00);
  TFT_WriteDataByte(0x00);

  TFT_WriteCommandByte(TFT_PWCTR1);    //Power control
  TFT_WriteDataByte(0x23);   //VRH[5:0]

  TFT_WriteCommandByte(TFT_PWCTR2);    //Power control
  TFT_WriteDataByte(0x10);   //SAP[2:0];BT[3:0]

  TFT_WriteCommandByte(TFT_VMCTR1);    //VCM control
  TFT_WriteDataByte(0x3e); //对比度调节
  TFT_WriteDataByte(0x28);

  TFT_WriteCommandByte(TFT_VMCTR2);    //VCM control2
  TFT_WriteDataByte(0x86);  //--

  TFT_WriteCommandByte(TFT_MADCTL);    // Memory Access Control
  TFT_WriteDataByte(0x48);

  TFT_WriteCommandByte(TFT_PIXFMT);
  TFT_WriteDataByte(0x55);

  TFT_WriteCommandByte(TFT_FRMCTR1);
  TFT_WriteDataByte(0x00);
  TFT_WriteDataByte(0x18);

  TFT_WriteCommandByte(TFT_DFUNCTR);    // Display Function Control
  TFT_WriteDataByte(0x08);
  TFT_WriteDataByte(0x82);
  TFT_WriteDataByte(0x27);

  TFT_WriteCommandByte(0xF2);    // 3Gamma Function Disable
  TFT_WriteDataByte(0x00);

  TFT_WriteCommandByte(TFT_GAMMASET);    //Gamma curve selected
  TFT_WriteDataByte(0x01);

  TFT_WriteCommandByte(TFT_GMCTRP1);    //Set Gamma
  TFT_WriteDataByte(0x0F);
  TFT_WriteDataByte(0x31);
  TFT_WriteDataByte(0x2B);
  TFT_WriteDataByte(0x0C);
  TFT_WriteDataByte(0x0E);
  TFT_WriteDataByte(0x08);
  TFT_WriteDataByte(0x4E);
  TFT_WriteDataByte(0xF1);
  TFT_WriteDataByte(0x37);
  TFT_WriteDataByte(0x07);
  TFT_WriteDataByte(0x10);
  TFT_WriteDataByte(0x03);
  TFT_WriteDataByte(0x0E);
  TFT_WriteDataByte(0x09);
  TFT_WriteDataByte(0x00);

  TFT_WriteCommandByte(TFT_GMCTRN1);    //Set Gamma
  TFT_WriteDataByte(0x00);
  TFT_WriteDataByte(0x0E);
  TFT_WriteDataByte(0x14);
  TFT_WriteDataByte(0x03);
  TFT_WriteDataByte(0x11);
  TFT_WriteDataByte(0x07);
  TFT_WriteDataByte(0x31);
  TFT_WriteDataByte(0xC1);
  TFT_WriteDataByte(0x48);
  TFT_WriteDataByte(0x08);
  TFT_WriteDataByte(0x0F);
  TFT_WriteDataByte(0x0C);
  TFT_WriteDataByte(0x31);
  TFT_WriteDataByte(0x36);
  TFT_WriteDataByte(0x0F);

  TFT_WriteCommandByte(TFT_SLPOUT);    //Exit Sleep
  msDelay(120);
  TFT_WriteCommandByte(TFT_DISPON);    //Display on

  // Clear TFT
  TFT_FillRectangle(0, 0, TFT_WIDTH, TFT_HEIGHT, TFT_BLACK);

} // end TFT_Init()



//------------------------------------------------------------------------------
// Function: TFT_WriteCommandByte()
// Params:   cmd; command number to send
// Returns:  void
// Writes a command to the TFT using SPI
//------------------------------------------------------------------------------
void TFT_WriteCommandByte(uint8_t cmd) {

    TFT_DC = 0;
    TFT_CS = 0;
    SPI2_TransmitByte(cmd);
    TFT_CS = 1;

} // end TFT_WriteCommandByte()



//------------------------------------------------------------------------------
// Function: TFT_WriteDataByte()
// Params:   data; data value to send
// Returns:  void
// Writes data to the TFT using SPI
//------------------------------------------------------------------------------
void TFT_WriteDataByte(uint8_t data) {

    TFT_DC = 1;
    TFT_CS = 0;
    SPI2_TransmitByte(data);
    TFT_CS = 1;

} // end TFT_WriteDataByte()


