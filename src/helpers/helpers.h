//
// Created by User on 28-12-2024.
//

#ifndef HELPERS_H
#define HELPERS_H

#include <cstdint>
#include "keypadc.h"

/**
 * Find element in array because this toolchain lacks half of cpp
 * @param elm Element
 * @param ar Array
 * @param ar_cnt Length of array
 * @return -1 if not present, index if it is
 */
int8_t indexOf(uint8_t elm, const uint8_t *ar, int ar_cnt);

/**
 * Library function to get keys
 * @return sk_key value
 */
uint8_t getSingleKeyPress();

#endif //HELPERS_H
