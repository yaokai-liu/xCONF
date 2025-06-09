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
 * Module Name: grammar/xJSON
 * Filename: reduce.c
 * Creator: Yaokai Liu
 * Create Date: 2025-06-07
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include "target.h"
#include "context.h"
#include "generated/tokens.gen.h"

List * XJSON_List_0 (Token args[], XJSONContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

List * XJSON_List_1 (Token [], XJSONContext *, ErrInfo *, const Allocator * ) {
  return nullptr;
}

Object * XJSON_Object_0 (Token args[], XJSONContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

Object * XJSON_Object_1 (Token args[], XJSONContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

Object * XJSON_Object_EXT (Token args[], XJSONContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

Pair * XJSON_Pair_0 (Token [], XJSONContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Pair * XJSON_Pair_1 (Token [], XJSONContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Pair * XJSON_Pair_2 (Token [], XJSONContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Pair * XJSON_Pair_3 (Token [], XJSONContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Pairs * XJSON_Pairs_0 (Token args[], XJSONContext *, ErrInfo *, const Allocator *allocator) {
  Pair *pair = args[0].value;

  Pairs *pairs = Array_new(sizeof(Pair), XJSON_TOKEN_Pair, allocator);

  Array_append(pairs, pair, 1);

  return pairs;
}

Pairs * XJSON_Pairs_1 (Token args[], XJSONContext *, ErrInfo *, const Allocator *) {
  Pairs *pairs = args[0].value;
  Pair *pair = args[2].value;

  Array_append(pairs, pair, 1);

  return pairs;
}

PathKey * XJSON_PathKey_0 (Token [], XJSONContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

PathKey * XJSON_PathKey_1 (Token [], XJSONContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

PathKey * XJSON_PathKey_2 (Token [], XJSONContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Texts * XJSON_Texts_0 (Token [], XJSONContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Texts * XJSON_Texts_1 (Token [], XJSONContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Value * XJSON_Value_0 (Token [], XJSONContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Value * XJSON_Value_1 (Token [], XJSONContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Value * XJSON_Value_2 (Token [], XJSONContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Value * XJSON_Value_3 (Token [], XJSONContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Value * XJSON_Value_4 (Token [], XJSONContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Value * XJSON_Value_5 (Token [], XJSONContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Values * XJSON_Values_0 (Token args[], XJSONContext *, ErrInfo *, const Allocator *allocator) {
  Value *value = args[0].value;

  Values *values = Array_new(sizeof(Value), XJSON_TOKEN_Value, allocator);

  Array_append(values, value, 1);

  return values;
}

Values * XJSON_Values_1 (Token args[], XJSONContext *, ErrInfo *, const Allocator *) {
  Values *values = args[0].value;
  Value *value = args[2].value;

  Array_append(values, value, 1);

  return values;
}

