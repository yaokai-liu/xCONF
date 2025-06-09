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
 * Module Name: include/xJSON
 * Filename: error.h
 * Creator: Yaokai Liu
 * Create Date: 2025-06-07
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#ifndef XJSON_ERROR_H
#define XJSON_ERROR_H

#include "token.h"

enum XLR_ERROR_CODE_ENUM {
  XJSON_SUCCESS,
  XJSON_ERROR_UNRECOGNIZED_SYMBOL,
  XJSON_ERROR_UNEXPECTED_EOF,

  XJSON_UNEXPECTED_TOKEN,
};


typedef struct ErrInfo {
  Location pos;
  uint32_t code;
  int32_t  state;
  uint32_t token;
} ErrInfo;

#endif //XJSON_ERROR_H
