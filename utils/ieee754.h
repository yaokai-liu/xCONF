/* License
 *
 * xJSON - C Library to Parse xJSON to C
 * Copyright (C) 2025 Yaokai Liu
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *
 * Project Name: xJSON
 * Module Name: utils
 * Filename: ieee754.h
 * Creator: Yaokai Liu
 * Create Date: 2025-06-08
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#ifndef XJSON_UTILS_IEEE754_H
#define XJSON_UTILS_IEEE754_H

#include <stdint.h>
#include "xJSON/extint.h"
#include "xJSON/extfloat.h"

float32_t ieee754bin32(bool sign, uint32_t integer, uint8_t exponent);
float64_t ieee754bin64(bool sign, uint64_t integer, uint16_t exponent);
float128_t ieee754bin128(bool sign, uint128_t integer, uint16_t exponent);

#endif //XJSON_UTILS_IEEE754_H
