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
 * Filename: enum.h
 * Creator: Yaokai Liu
 * Create Date: 2025-06-07
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#ifndef XJSON_ENUM_H
#define XJSON_ENUM_H

typedef enum XJSON_VALUE_TYPE_ENUM: uint32_t {
  XJSON_VAL_NULL,
  XJSON_VAL_LIST,
  XJSON_VAL_TEXT,
  XJSON_VAL_OBJECT,
  XJSON_VAL_BOOLEAN,

  XJSON_VAL_INT,
  XJSON_VAL_UINT,
  XJSON_VAL_FLOAT,

  XJSON_VAL_UNINITIALIZED,
} XJSON_val_type;

#endif //XJSON_ENUM_H
