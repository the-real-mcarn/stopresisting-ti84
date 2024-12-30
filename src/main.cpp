/*
 *--------------------------------------
 * Program Name:
 * Author:
 * License:
 * Description:
 *--------------------------------------
*/
#include <cstdint>
#include <cstdio>
#include <ti/screen.h>
#include <ti/getcsc.h>
#include <graphx.h>
#include <sys/rtc.h>
#include "srti84.hpp"

#include "stopresisting/StopResisting.hpp"
#include "helpers/helpers.h"

StopResisting res(rtc_Time());
SRTi84 ui;

// Input globals
int8_t colors[4] = {0}; // Actual colors
int8_t inputs[4] = {0}; // User guess
int8_t inputIndex = 0;

// Restart flag
bool restart = false; // Restart flag set by input validator
uint16_t restartCount = 0; // Timer
#define RESTART_DELAY 1200 // Time to runout

/**
 * Redraw everything
 */
void redraw();

/**
 * Check and submit value if input is complete
 * @param value Integer input
 */
void input(int8_t value);

/* Main function, called first */
int main(void)
{
    // Setup gfx and set palette
    gfx_Begin();
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);

    // Load fonts, stop if failed
    if (!ui.init()) {
        while (!getSingleKeyPress()) {
        }
        gfx_End();
        return 0;
    }

    // Draw first
    redraw();

    // Key handling
    const uint8_t keys[10] = {sk_0, sk_1, sk_2, sk_3, sk_4, sk_5, sk_6, sk_7, sk_8, sk_9};
    bool stop = false; // Quit loop

    while (!stop) {
        if (restart) {
            if (restartCount++ > RESTART_DELAY) {
                restart = false;
                restartCount = 0;
                redraw();
            }
        }

        // Progress keypresses
        const uint8_t key = getSingleKeyPress();
        switch(key) {
            case sk_Graph:
                // Exit program
                stop = true;
                break;
            case sk_Trace:
                // New resistor and redraw
                ui.score = 0;
                redraw();
                break;
            case sk_Del:
                // Delete one input
                inputIndex--;
                ui.drawInputBox(inputs, inputIndex);
                break;
            case sk_Clear:
                // Delete all input
                inputIndex = -1;
                ui.drawInputBox(inputs, inputIndex);
                break;
            case sk_Enter:
                // Skip delay
                restartCount = RESTART_DELAY;
            default:
                // Try to interpret number keys and ignore the rest without making 700 switchcases
                const int8_t val = indexOf(key, keys, sizeof(keys));
                if (val >= 0) {
                    input(val);
                }
                break;
        }
    }

    gfx_End();
    return 0;
}

void redraw() {
    gfx_FillScreen(0xff);

    inputIndex = -1;
    res.newResistor();
    const uint8_t bands = res.getColors(colors);

    ui.drawResistor(50, colors, bands);
    ui.drawUi();
    ui.drawInputBox(inputs, inputIndex);
}

void input(int8_t value) {
    // -1 prevents any number from being printed but it also breaks the array index
    // Skip to index 0 when first input is given
    if (inputIndex < 0)
        inputIndex++;

    inputs[inputIndex] = value;

    if (inputIndex == 3) {
        const uint8_t result = res.guess(inputs);
        ui.drawInputBox(colors, inputIndex, result);

        char valStr[16] = {0};
        res.getValueStr(valStr);
        ui.drawResult(valStr, result);

        if (result == 15) {
            // Answer correct
            ui.score++;
            if (ui.score>ui.highscore)
                ui.highscore = ui.score;

            // gfx_FillScreen(0x07);
        } else {
            // Answer incorrect
            ui.score = 0;
            // gfx_FillScreen(0xE0);
        }
        restart = true;
    } else {
        ui.drawInputBox(inputs, inputIndex);
        inputIndex++;
    }
}