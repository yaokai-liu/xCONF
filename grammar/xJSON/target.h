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
#include "xJSON/extint.h"
#include "xJSON/token.h"
#include "xJSON/enum.h"
#include "avl-tree.h"
#include "dict.h"


typedef Array List; // Array<REFER(Value)>
typedef struct Text Text;
typedef struct Value Value;
typedef struct Object Object;

typedef struct Text {
  uint32_t      size;
  REFER(char_t) content;
} Text;

typedef struct Object {
  Array *   keys;     // Array<REFER(char_t)>
  AVLTree * mapping;  // AVLTree<REFER(char_t), REFER(Value>>
} Object;

typedef struct Value {
  xJSON_val_t   type;
  uint32_t      size;
  union {
    List *        LIST;
    Text *        TEXT;
    uint32_t      UINT;
    float         FLOAT;
    double        DOUBLE;
    Object *      OBJECT;
    bool          BOOLEAN;
    uint64_t      LONG_UINT;
    long double   LONG_DOUBLE;
    uint128_t     LONG_LONG_UINT;
  } val;
} Value;

typedef Array Values; // Array<REFER(Value)>
typedef Array Pairs; // Array<Pair>

typedef struct Pair {
  void *key;
  Value *value;
} Pair;

typedef struct WrapperedText {
  uint16_t n_pred;
  uint16_t n_succ;
  uint32_t length;
  char_t * content;
} WrapperedText;

typedef Text Texts;

// Maybe reinterpretation of `Path` here
// makes a little bit confused, but there is no wrong.
// Because every `Path` is only mapping to a `Value`.
typedef Value Path;

Object *Object_new();

void releaseValue(Value *, const Allocator *);
void releasePair(Pair *, const Allocator *);
void releaseTexts(Texts *, const Allocator *);
void releasePath(Path *, const Allocator *);
void releaseObject(Object *, const Allocator *);
void releaseList(List *, const Allocator *);
void releaseWrapperedText(WrapperedText *, const Allocator *);

#endif //XJSON_GRAMMAR_TARGET_H
