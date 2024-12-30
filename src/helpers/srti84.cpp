//
// Created by User on 29-12-2024.
//

#include "srti84.hpp"

SRTi84::SRTi84() = default;

int8_t SRTi84::init() {
    this->OSFontL = fontlib_GetFontByIndex("OSLFONT", 0);
    this->OSFontS = fontlib_GetFontByIndex("OSsfont", 0);

    /* This check is important! If fetching the font fails, trying to use the font will go . . . poorly. */
    if (!this->OSFontL || !this->OSFontS) {
        gfx_PrintStringXY("Missing OS Font", 0, 0);
        return 0;
    }
    return 1;
}

void SRTi84::drawUi() {
    // Header
    gfx_SetColor(0x4a);
    gfx_FillRectangle(0, 0, GFX_LCD_WIDTH, 30);

    constexpr char statBarTitle[] = "STOP RESISTING!   By the_real_mcarn";
    char statBarData[32] = {0};
    snprintf(statBarData, sizeof(statBarData), "SCORE: %d   HIGHSCORE: %d", this->score, this->highscore);

    fontlib_SetFont(this->OSFontS, FONTLIB_IGNORE_LINE_SPACING);
    fontlib_SetColors(0xff, 0x4a);

    fontlib_SetCursorPosition(32, 0);
    fontlib_DrawString(statBarTitle);

    fontlib_SetCursorPosition(32, 15);
    fontlib_DrawString(statBarData);

    gfx_SetTransparentColor(0x01);
    gfx_TransparentSprite_NoClip(headericon, 1, 1);

    // Instructions
    constexpr char controls[] = "Use DEL/CLEAR to remove inputs";
    fontlib_SetColors(0x00, 0xff);
    fontlib_SetCursorPosition(
        (GFX_LCD_WIDTH / 2) - (fontlib_GetStringWidth(controls)/2),
        (GFX_LCD_HEIGHT / 6) * 5);
    fontlib_DrawString(controls);

    // Menu Keys
    constexpr char btnQuit[] = "QUIT";
    constexpr char btnNew[] = "NEW";

    drawMenuKey(3, btnNew);
    drawMenuKey(4, btnQuit);
}

void SRTi84::drawMenuKey(uint8_t nr, const char* string) const {
    fontlib_SetFont(this->OSFontL, FONTLIB_IGNORE_LINE_SPACING);

    const uint8_t w = 64;
    const uint8_t h = 18;
    const uint8_t x = 2 + (nr * (w - 1));
    const uint8_t y = GFX_LCD_HEIGHT - h;

    // Inner box
    gfx_SetColor(0x00);
    gfx_Rectangle(x + 1, y + 1, w - 2, h - 1);

    // Outline
    gfx_VertLine(x, y + 2, h - 2);
    gfx_VertLine(x + w - 1, y + 2, h - 2);
    gfx_HorizLine(x + 2, y, w - 4);
    gfx_SetPixel(x+2,y+2);
    gfx_SetPixel(x + w - 3, y + 2);

    // Inside blank
    gfx_SetColor(0xff);;
    gfx_FillRectangle(x + 2, y + 2, w - 4, h - 1);

    // Text
    fontlib_SetColors(0x00, 0xff);
    fontlib_SetCursorPosition(
        (x + w/2) - (fontlib_GetStringWidth(string)/2),
        y+4);
    fontlib_DrawString(string);
}

void SRTi84::drawInputBox(int8_t *input, int8_t index, uint8_t validate) {
    constexpr uint8_t w = 32;
    constexpr uint8_t h = 32;
    constexpr uint8_t y = (GFX_LCD_HEIGHT / 3) * 2;
    constexpr uint8_t mx = 16;
    constexpr uint8_t a = 4; // Amount

    // Draw new inputs
    for (int i = 0; i < a; ++i) {
        uint8_t dw = (a * w) + ((a - 1) * mx);
        bool valid = validate >> ((a - 1) - i) & 1;

        uint8_t dx = (GFX_LCD_WIDTH / 2) - (dw / 2) + (i * w) + (i * mx);

        if (validate != 0xff) {
            gfx_SetColor((valid) ? 0x07 : 0xe0);
            fontlib_SetColors((valid) ? 0x00 : 0xe0, 0xff);
        } else {
            gfx_SetColor(0x00);
            fontlib_SetColors(0x00, 0xff);
        }

        gfx_Rectangle(dx, y, w, h);
        gfx_Rectangle(dx+1, y+1, w-2, h-2);

        fontlib_SetFont(this->OSFontL, FONTLIB_IGNORE_LINE_SPACING);
        fontlib_SetCursorPosition(dx + (w/2) - (10 / 2), y + (h/2) - (14/2)); // Fontheight
        if (index != -1 && i <= index) {
            fontlib_DrawInt(input[i], 1);
        } else {
            fontlib_DrawGlyph('?');
        }
    }
}

void SRTi84::drawResult(char *value, uint8_t validate) {
    constexpr uint8_t y = (GFX_LCD_HEIGHT / 2) + 6;
    const uint8_t w = fontlib_GetStringWidth(value);
    uint8_t x = (GFX_LCD_WIDTH / 2) - (w / 2) + (24 / 2);

    fontlib_SetFont(this->OSFontL, FONTLIB_IGNORE_LINE_SPACING);
    fontlib_SetColors(0x00, 0xff);
    fontlib_SetCursorPosition(x, y);
    fontlib_DrawString(value);

    gfx_SetTransparentColor(0x01);
    if (validate == 15) {
        gfx_TransparentSprite_NoClip(ok, x - 24, y - 2);
    } else {
        gfx_TransparentSprite_NoClip(nok, x - 24, y - 2);
    }
}

/**
 * Draw the resistor window
 * @param y Top left Y
 */
void SRTi84::drawResistor(int16_t y, const int8_t *colors, uint8_t bands ) {
    constexpr uint8_t h = 50; // Body height
    constexpr uint8_t w = 100; // Body width
    constexpr uint8_t th = 60; // Thicker bit height
    constexpr uint8_t tw = 20; // Thicker bit width
    constexpr uint8_t ts = 6; // Spacing between thicker bit and body
    uint8_t ow = 2; // Outline width
    constexpr uint8_t lw = 8; // Width of legs
    constexpr uint8_t lm = 20; // Space between screen and legs

    // Legs outline
    gfx_SetColor(0x00);
    gfx_FillRectangle(lm - ow, (y + (h/2) - (lw/2)) - ow,GFX_LCD_WIDTH - (2*lm) + ow * 2, lw+ ow * 2); // Top
    gfx_FillRectangle(lm - ow - 2 , (y + (h/2) - (lw/2)) + 2 - ow, lw+ ow * 2, lw+ ow * 2); // Left Connecting
    gfx_FillRectangle(lm - ow - 4, (y + (h/2) - (lw/2)) + 4 - ow,lw + ow * 2, GFX_LCD_HEIGHT - h + ow * 2); // Left
    gfx_FillRectangle(GFX_LCD_WIDTH - lm - ow - lw + 2, (y + (h/2) - (lw/2)) + 2 - ow,lw+ ow * 2, lw+ ow * 2); // Right Connecting
    gfx_FillRectangle(GFX_LCD_WIDTH - lm - ow - lw + 4, (y + (h/2) - (lw/2)) + 4 - ow,lw+ ow * 2, GFX_LCD_HEIGHT - h + ow * 2); // Right

    // Legs
    gfx_SetColor(0x4a);
    gfx_FillRectangle(lm, (y + (h/2) - (lw/2)),GFX_LCD_WIDTH - (2*lm), lw); // Top
    gfx_FillRectangle(lm - 2 , (y + (h/2) - (lw/2)) + 2, lw, lw); // Left Connecting
    gfx_FillRectangle(lm - 4, (y + (h/2) - (lw/2)) + 4,lw, GFX_LCD_HEIGHT - h); // Left
    gfx_FillRectangle(GFX_LCD_WIDTH - lm - lw + 2, (y + (h/2) - (lw/2)) + 2,lw, lw); // Right Connecting
    gfx_FillRectangle(GFX_LCD_WIDTH - lm - lw + 4, (y + (h/2) - (lw/2)) + 4,lw, GFX_LCD_HEIGHT - h); // Right

    ow = 4; // Outline width

    // Body outline
    gfx_SetColor(0x00);
    gfx_FillRectangle((GFX_LCD_WIDTH / 2 - w / 2) - ow, y - ow,w + ow * 2,h + ow * 2); // Main body
    gfx_FillRectangle((GFX_LCD_WIDTH / 2 - w / 2) - ts - tw - ow,y-((th-h)/2) - ow, tw + ow * 2, th+1 + ow * 2); // Thicker part left 80
    gfx_FillRectangle((GFX_LCD_WIDTH / 2 + w / 2) + ts - ow,y-((th-h)/2) - ow, tw + ow * 2, th+1 + ow * 2); // Thicker part right 218

    gfx_FillEllipse((GFX_LCD_WIDTH / 2 - w / 2) - ts - tw, y-((th-h)/2)+(th/2), tw/2+ ow, th/2+ ow); // Outside left 50
    gfx_FillEllipse((GFX_LCD_WIDTH / 2 - w / 2) - ts, y-((th-h)/2)+(th/2), tw/2+ ow, th/2+ ow); // Connecting thicker part left and body
    gfx_FillEllipse((GFX_LCD_WIDTH / 2 + w / 2) + ts, y-((th-h)/2)+(th/2), tw/2+ ow, th/2+ ow); // Connecting thicker part right and body
    gfx_FillEllipse((GFX_LCD_WIDTH / 2 + w / 2) + ts + tw, y-((th-h)/2)+(th/2), tw/2+ ow, th/2+ ow); // Outside right

    // Body
    gfx_SetColor(0x5f); // Kinda blue
    gfx_FillRectangle(GFX_LCD_WIDTH / 2 - w / 2,y,w,h); // Main body
    gfx_FillRectangle((GFX_LCD_WIDTH / 2 - w / 2) - ts - tw, y-((th-h)/2), tw, th+1); // Thicker part left 80
    gfx_FillRectangle((GFX_LCD_WIDTH / 2 + w / 2) + ts, y-((th-h)/2), tw, th+1); // Thicker part right 218

    // Elises require a centre coordinate
    gfx_FillEllipse((GFX_LCD_WIDTH / 2 - w / 2) - ts - tw, y-((th-h)/2)+(th/2), tw/2, th/2); // Outside left 50
    gfx_FillEllipse((GFX_LCD_WIDTH / 2 - w / 2) - ts, y-((th-h)/2)+(th/2), tw/2, th/2); // Connecting thicker part left and body
    gfx_FillEllipse((GFX_LCD_WIDTH / 2 + w / 2) + ts, y-((th-h)/2)+(th/2), tw/2, th/2); // Connecting thicker part right and body
    gfx_FillEllipse((GFX_LCD_WIDTH / 2 + w / 2) + ts + tw, y-((th-h)/2)+(th/2), tw/2, th/2); // Outside right

    // Lines
    const uint8_t lx[] = {
        (GFX_LCD_WIDTH / 2 - w / 2) - ts - tw + 2,
        GFX_LCD_WIDTH / 2 - w / 2 + 12,
        GFX_LCD_WIDTH / 2 - w / 2 + 42,
        GFX_LCD_WIDTH / 2 - w / 2 + 72
    };

    for (uint8_t i = 0; i < bands; ++i) {
        const uint8_t bw = 16;
        const uint8_t displaycolor[] = {
            0x00, // Black
            0x41, // Brown
            0xe0, // Red
            0xe2, // Orange
            0xe7, // Yellow
            0x07, // Green
            0x19, // Blue
            0x90, // Violet
            0x4a, // Grey
            0xff, // White
            0x83, // Gold (kinda)
            0x73, // Silver???????
            0xf0  // Pink
        };

        gfx_SetColor(displaycolor[colors[i]]);
        if (i == 0) { // y-((th-h)/2)
            gfx_FillRectangle(lx[i], y-((th-h)/2),bw,th+1); // Don't ask
        } else {
            gfx_FillRectangle(lx[i], y,bw,h);
        }
    }

    // gfx_FillRectangle();
}