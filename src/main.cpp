/*
 *--------------------------------------
 * Program Name:
 * Author:
 * License:
 * Description:
 *--------------------------------------
*/
#include <cstdint>
#include <ti/screen.h>
#include <ti/getcsc.h>
#include <graphx.h>
#include <time.h>

#include "stopresisting/StopResisting.hpp"
#include "helpers/helpers.h"

StopResisting res(time(nullptr));
int8_t inputs[4] = {0};
uint8_t inputIndex = 0;

/**
 * Redraw everything
 */
void redraw();

/**
 * Check and submit value if input is complete
 * @param value Integer input
 */
void input(int8_t value);

uint8_t bgMap[8] = {
    0b00001000,
    0b00000100,
    0b00000010,
    0b00000001,
    0b00010000,
    0b00100000,
    0b01000000,
    0b10000000
};

/* Main function, called first */
int main(void)
{
    gfx_Begin();
    gfx_ZeroScreen();

    redraw();

    const uint8_t keys[10] = {sk_0, sk_1, sk_2, sk_3, sk_4, sk_5, sk_6, sk_7, sk_8, sk_9};
    bool stop = false;
    while (!stop) {
        const uint8_t key = get_single_key_pressed();
        switch(key) {
            case sk_Graph:
                // Exit program
                stop = true;
                break;
            case sk_Trace:
                // New resistor and redraw
                redraw();
                break;
            case sk_Del:
                // Delete one input
                inputIndex--;
                break;
            case sk_Clear:
                // Delete all input
                inputIndex = 0;
                break;

            default:
                // Try to interpret number keys and ignore the rest without making 700 switchcases
                const int8_t val = indexOf(key, keys, sizeof(keys));
                if (val >= 0) {
                    gfx_PrintInt(val, 2);
                    input(val);
                }
                break;
        }
    }

    gfx_End();
    return 0;
}

void redraw() {
    inputIndex = 0;
    res.newResistor();

    int8_t colors[4] = {0};
    uint8_t bands = res.getColors(colors);

    gfx_FillScreen(0x5a);

    // Center line
    gfx_SetColor(0xe0);
    gfx_Line(GFX_LCD_WIDTH/2,0,GFX_LCD_WIDTH/2,GFX_LCD_HEIGHT);

    drawResistor(20, colors, bands);
}

void input(int8_t value) {
    inputs[inputIndex] = value;

    if (inputIndex == 3) {
        const uint8_t result = res.guess(inputs);
        if (result == 15) {
            gfx_FillScreen(0x07);
        } else {
            gfx_FillScreen(0xE0);
        }
    } else {
        inputIndex++;
    }
}