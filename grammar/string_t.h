/* License
 *
 * xCONF - C Library to Parse xCONF to C
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
 * Project Name: xCONF
 * Module Name: grammar
 * Filename: string_t.h
 * Creator: Yaokai Liu
 * Create Date: 2025-06-07
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#ifndef XCONF_GRAMMAR_STRING_T_H
#define XCONF_GRAMMAR_STRING_T_H

#include <stdint.h>
#include "char_t.h"

uint32_t stridx_o(char_t chr, const char_t *str);
uint32_t strcmp_o(const char_t *str1, const char_t *str2);

#endif //XCONF_GRAMMAR_STRING_T_H
