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
 * Module Name: test
 * Filename: debug.c
 * Creator: Yaokai Liu
 * Create Date: 2025-06-08
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include <stdio.h>
#include <stdint.h>

#include "ieee754.h"

int main () {
  float64_t a  = ieee754bin64(0, 0b1100100100001111110110101010001000100001011010001101010100101ULL, 1);
//  float64_t a  = ieee754bin64(0, 0b110010010000111111011ULL, 1);
//  float64_t a  = ieee754bin64(0, 0, 1024);
  uint64_t *pa = (uint64_t *)&a;
  printf("%.064lf\n", a);
  printf("%.064lb\n", *pa);
  return 0;
}
