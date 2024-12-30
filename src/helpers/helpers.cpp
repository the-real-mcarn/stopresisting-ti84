//
// Created by User on 28-12-2024.
//
#include "helpers.h"

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
    return -1; // Should never reach this point
}

uint8_t getSingleKeyPress(void) {
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