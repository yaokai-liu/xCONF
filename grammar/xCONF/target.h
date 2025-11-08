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
 * Filename: target.h
 * Creator: Yaokai Liu
 * Create Date: 2025-06-07
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#ifndef XCONF_GRAMMAR_TARGET_H
#define XCONF_GRAMMAR_TARGET_H

#include "array.h"
#include "dict.h"
#include "token.h"
#include "xCONF/xCONF.h"
#include "meman-utils.h"
#include "enum.h"


typedef Array List; // Array<REFER(Value)>
typedef Dict Object; // Dict<REFER(char), Value>
typedef Object Imported;
typedef struct Text Text;
typedef struct Path Path;
typedef struct Value Value;

#define Object_new(allocator) \
  Dict_new(sizeof(REFER(char)), sizeof(REFER(Value)), (key_t *) refer2u64, XCONF_OBJECT_ID, nullptr, nullptr, allocator)
#define List_new(allocator) \
  Array_new(sizeof(REFER(Value)), XCONF_REFER_VALUE_ARRAY, allocator)
#define ReferArray_new(allocator) \
  Array_new(sizeof(Refer), XCONF_REFER_ARRAY, allocator)

typedef struct Text {
  uint32_t      size;
  REFER(char_t) content;
} Text;

typedef struct Path {
  const REFER(void)  key;
  const REFER(Value) parent;
} Path;

typedef struct Refer {
  union {
    REFER(char_t) key;
    uint32_t      index;
  } refer;
  Location start;
  Location end;
} Refer;

typedef Array ReferArray; // Array<Refer>

typedef struct Value {
  enum XCONF_VALUE_TYPE_ENUM   type;
  uint32_t      size;
  Path          path;
  union {
    int32_t       I32;
    uint32_t      U32;
    int64_t       I64;
    uint64_t      U64;
    int128_t      I128;
    uint128_t     U128;
    int256_t      I256;
    uint256_t     U256;
    float32_t     F32;
    float64_t     F64;
    float128_t    F128;
    float256_t    F256;
    Text *        TEXT;
    bool          BOOLEAN;
    Object *      OBJECT;
    List *        LIST;
    void *        REFER;
  } val;
} Value;

typedef Array Values; // Array<REFER(Value)>
typedef Array Pairs; // Array<Pair>

typedef struct WrapperedText {
  uint16_t n_pred;
  uint16_t n_succ;
  uint32_t length;
  char_t * content;
} WrapperedText;

typedef Text Texts;

// Maybe reinterpretation of `Path` here
// makes a little bit confused, but there is no wrong.
// Because every `Path` is only mapping to a `Pair`.
typedef Path Pair;

void releaseValue(Value *, const Allocator *);
void releasePair(Pair *, const Allocator *);
void releaseTexts(Texts *, const Allocator *);
void releasePath(Path *, const Allocator *);
void releaseObject(Object *, const Allocator *);
void releaseList(List *, const Allocator *);
void releaseWrapperedText(WrapperedText *, const Allocator *);

#endif //XCONF_GRAMMAR_TARGET_H
