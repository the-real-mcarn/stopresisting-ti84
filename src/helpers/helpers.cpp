//
// Created by User on 28-12-2024.
//
#include "helpers.h"

#include <stopresisting/StopResisting.hpp>

int8_t indexOf(const uint8_t elm, const uint8_t *ar, int ar_cnt)
{
    // decreasing array count till it reaches negative
    // arr_cnt - 1 to 0
    while (ar_cnt--)
    {
        // Return array index if current element equals provided element
        if (ar[ar_cnt] == elm)
            return ar_cnt;
    }

    // Element not present
    return -1; // Should never reaches this point
}

uint8_t get_single_key_pressed(void) {
    static uint8_t last_key;
    uint8_t only_key = 0;
    kb_Scan();
    for (uint8_t key = 1, group = 7; group; --group) {
        for (uint8_t mask = 1; mask; mask <<= 1, ++key) {
            if (kb_Data[group] & mask) {
                if (only_key) {
                    last_key = 0;
                    return 0;
                } else {
                    only_key = key;
                }
            }
        }
    }
    if (only_key == last_key) {
        return 0;
    }
    last_key = only_key;
    return only_key;
}



/**
 * Draw the resistor window
 * @param y Top left Y
 */
void drawResistor(int16_t y, const int8_t *colors, uint8_t bands ) {
    constexpr uint8_t h = 60; // Body height
    constexpr uint8_t w = 120; // Body width
    constexpr uint8_t th = 70; // Thicker bit height
    constexpr uint8_t tw = 25; // Thicker bit width
    constexpr uint8_t ts = 6; // Spacing between thicker bit and body
    uint8_t ow = 2; // Outline width
    constexpr uint8_t lw = 10; // Width of legs
    constexpr uint8_t lm = 20; // Space between screen and legs

    // Legs outline
    gfx_SetColor(0x00);
    gfx_FillRectangle(lm - ow, (y + (h/2) - (lw/2)) - ow,GFX_LCD_WIDTH - (2*lm) + ow * 2, lw+ ow * 2); // Top
    gfx_FillRectangle(lm - ow - 2 , (y + (h/2) - (lw/2)) + 2 - ow, lw+ ow * 2, lw+ ow * 2); // Left Connecting
    gfx_FillRectangle(lm - ow - 4, (y + (h/2) - (lw/2)) + 4 - ow,lw + ow * 2, GFX_LCD_HEIGHT - h + ow * 2); // Left
    gfx_FillRectangle(GFX_LCD_WIDTH - lm - ow - lw + 2, (y + (h/2) - (lw/2)) + 2 - ow,lw+ ow * 2, lw+ ow * 2); // Right Connecting
    gfx_FillRectangle(GFX_LCD_WIDTH - lm - ow - lw + 4, (y + (h/2) - (lw/2)) + 4 - ow,lw+ ow * 2, GFX_LCD_HEIGHT - h+ ow * 2); // Right

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
        GFX_LCD_WIDTH / 2 - w / 2 + 10,
        GFX_LCD_WIDTH / 2 - w / 2 + 50,
        GFX_LCD_WIDTH / 2 - w / 2 + 90
    };

    for (uint8_t i = 0; i < bands; ++i) {
        const uint8_t bw = 20;
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
            gfx_FillRectangle(lx[i], y,bw,h+1);
        }
    }

    // gfx_FillRectangle();
}