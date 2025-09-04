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
#include "avl-tree.h"
#include "xCONF/xCONF.h"


typedef struct Text Text;
typedef struct Value Value;
typedef struct Array List; // Array<REFER(Value)>
typedef struct XCONFObject XCONFObject, Object;

typedef struct Text {
  uint32_t      size;
  REFER(char_t) content;
} Text;

typedef struct XCONFObject {
  Array *   keys;     // Array<REFER(char_t)>
  AVLTree * mapping;  // AVLTree<REFER(char_t), REFER(Value)>
} XCONFObject;

enum XCONF_VALUE_CATEGORY_ENUM: uint32_t {
  XCONF_VAL_CAT_NULL,
  XCONF_VAL_CAT_LIST,
  XCONF_VAL_CAT_TEXT,
  XCONF_VAL_CAT_OBJECT,
  XCONF_VAL_CAT_BOOLEAN,

  XCONF_VAL_CAT_INT,
  XCONF_VAL_CAT_UINT,
  XCONF_VAL_CAT_FLOAT,

  XCONF_VAL_CAT_UNINITIALIZED = UINT32_MAX,
};

typedef struct Value {
  enum XCONF_VALUE_TYPE_ENUM   type;
  uint32_t      size;
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

Object *Object_new(const Allocator *allocator);

void releaseValue(Value *, const Allocator *);
void releasePair(Pair *, const Allocator *);
void releaseTexts(Texts *, const Allocator *);
void releasePath(Path *, const Allocator *);
void releaseObject(Object *, const Allocator *);
void releaseList(List *, const Allocator *);
void releaseWrapperedText(WrapperedText *, const Allocator *);

#endif //XCONF_GRAMMAR_TARGET_H
