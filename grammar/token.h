/* License
 *
 * xCONF - A Configuration Language and Its Parser
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
 * Filename: token.h
 * Creator: Yaokai Liu
 * Create Date: 2025-06-07
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#ifndef TOKEN_H
#define TOKEN_H
#include "char_t.h"
#include "allocator.h"
#include "xCONF/xCONF.h"
#include <stdint.h>

/**
 * @description location of a Token in a file
 */
typedef struct Location {
  /**
   * @description offset in src string
   */
  uint32_t offset;
  /**
   * @description line number in src file
   */
  uint32_t lineno;
  /**
   * @description column offset in the line in src file
   */
  uint32_t column;
} Location;

typedef struct Terminal {
  Location location;
  /// length of the terminal (size in bytes)
  uint32_t length;
  /// Token Type
  uint32_t type;
  /// value of the terminal
  void *value;
} Terminal;

typedef struct Token {
  /// start of the terminal
  Location start;
  /// end of the terminal
  Location end;
  /// length of the terminal (size in bytes)
  uint32_t length;
  /// Token Type
  uint32_t type;
  /// value of the terminal
  void *value;
} Token;

const char_t *get_token_type_name(uint16_t type);

void releaseToken(Token *token, const Allocator *allocator);

#endif //TOKEN_H
