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
 * Module Name: grammar
 * Filename: target.h
 * Creator: Yaokai Liu
 * Create Date: 2025-06-07
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#ifndef XJSON_GRAMMAR_TARGET_H
#define XJSON_GRAMMAR_TARGET_H

#include "array.h"
#include "xJSON/objects.h"
#include "xJSON/token.h"
#include "xJSON/enum.h"

typedef struct XJSONText {
  uint32_t      size;
  REFER(char_t) content;
} XJSONText;

typedef struct XJSONList {
  uint32_t           count;
  REFER(XJSONValue)  values;
} XJSONList;

typedef struct XJSONObject {
  uint32_t              count;
  REFER(REFER(char_t))  keys;
  REFER(XJSONValue)     values;
} XJSONObject;

typedef struct XJSONValue {
  xJSON_val_t   type;
  uint32_t      size;
  union {
    XJSONList *   LIST;
    XJSONText *   TEXT;
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

typedef XJSONList List;
typedef XJSONValue Value;
typedef XJSONObject Object;

typedef Array Values; // Array<Value>
typedef Array Pairs; // Array<Pair>

typedef struct Pair {
  void *key;
  Value *value;
} Pair;

typedef struct Text {
  uint16_t n_pred;
  uint16_t n_succ;
  uint32_t length;
  char_t * content;
} Text;

typedef XJSONText Texts;

typedef struct PathKey {

} PathKey;

void releaseValue(Value *, const Allocator *);
void releasePair(Pair *, const Allocator *);
void releaseText(Text *, const Allocator *);
void releaseTexts(Texts *, const Allocator *);
void releasePathKey(PathKey *, const Allocator *);
void releaseObject(Object *, const Allocator *);
void releaseList(List *, const Allocator *);

#endif //XJSON_GRAMMAR_TARGET_H
