//
// Created by User on 29-12-2024.
//

#ifndef SRUI_H
#define SRUI_H

#include <cstdint>
#include "keypadc.h"
#include "graphx.h"
#include "fontlibc.h"
#include "gfx/gfx.h"
#include <cstdio>
#include <fontlibc.h>

class SRTi84 {
    private:
        /**
         * Draws TI style keys for buttons below the display
         * @param nr
         * @param string
         */
        void drawMenuKey(uint8_t nr, const char* string) const;

    public:
        fontlib_font_t *OSFontL{};
        fontlib_font_t *OSFontS{};

        uint8_t score = 0;
        uint8_t highscore = 0;

        /**
         * Constructor that does nothing
         */
        SRTi84();

        /**
         * Loads fonts
         * @return 1 on succes, 0 on fail
         */
        int8_t init();

        /**
         * Draw static UI elements
         */
        void drawUi();

        /**
         *
         * @param input
         * @param index
         * @param validate
         */
        void drawInputBox(int8_t *input, int8_t index, uint8_t validate = 0xff);

        void drawResult(char *value, uint8_t validate);

        /**
         * Draw big resistor.png
         * @param y Y offset
         * @param colors Color values from StopResisting
         * @param bands Amount of bands
         */
        void drawResistor(int16_t y, const int8_t *colors, uint8_t bands);
};

#endif //SRUI_H
