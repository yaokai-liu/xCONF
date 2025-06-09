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
 * Filename: objects.h
 * Creator: Yaokai Liu
 * Create Date: 2025-06-07
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#ifndef XJSON_OBJECTS_H
#define XJSON_OBJECTS_H

#include "array.h"
#include "char_t.h"
#include "enum.h"
#include "extint.h"

typedef struct XJSONList XJSONList;
typedef struct XJSONObject XJSONObject;

typedef struct XJSONValue {
  xJSON_val_t   type;
  uint32_t      size;
  union {
    XJSONList *   LIST;
    char_t *      TEXT;
    uint32_t      UINT;
    float         FLOAT;
    double        DOUBLE;
    XJSONObject * OBJECT;
    bool          BOOLEAN;
    uint64_t      LONG_UINT;
    long double   LONG_DOUBLE;
    uint128_t     LONG_LONG_UINT;
  } val;
} XJSONValue;


#endif //XJSON_OBJECTS_H
