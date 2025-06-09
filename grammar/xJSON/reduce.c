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

List * XJSON_List_1 (Token args[], XJSONContext *, ErrInfo *, const Allocator * ) {
  return args[0].value;
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

Texts * XJSON_Texts_0 (Token args[], XJSONContext *context, ErrInfo *, const Allocator *) {
  Texts *texts = args[0].value;
  Text *text = args[1].value;

  XJSONContent_add_text_content(context, text->content, text->length);
  texts->size += text->length;

  return texts;
}

Texts * XJSON_Texts_1 (Token args[], XJSONContext *context, ErrInfo *, const Allocator *allocator) {
  Text *text = args[0].value;

  Texts *texts = allocator->calloc(1, sizeof(Texts));

  texts->content = XJSONContent_new_text_content(context, text->content, text->length);
  texts->size = text->length;

  return texts;
}

Value * XJSON_Value_0 (Token args[], XJSONContext *, ErrInfo *, const Allocator *allocator) {
  Object *object = args->value;

  Value *value = allocator->calloc(1, sizeof(Value));
  value->type = XJSON_VAL_LIST;
  value->size = object->count;
  value->val.OBJECT = object;

  return value;
}

Value * XJSON_Value_1 (Token args[], XJSONContext *, ErrInfo *, const Allocator *allocator) {
  List *list = args->value;

  Value *value = allocator->calloc(1, sizeof(Value));
  value->type = XJSON_VAL_LIST;
  value->size = list->count;
  value->val.LIST = list;

  return value;
}

Value * XJSON_Value_2 (Token args[], XJSONContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

Value * XJSON_Value_3 (Token args[], XJSONContext *, ErrInfo *, const Allocator *allocator) {
  Texts *text = args->value;

  Value *value = allocator->calloc(1, sizeof(Value));
  value->type = XJSON_VAL_TEXT;
  value->size = text->size;
  value->val.TEXT = text;

  return value;
}

Value * XJSON_Value_4 (Token args[], XJSONContext *, ErrInfo *, const Allocator *allocator) {
  bool boolean = (uint64_t) args->value;

  Value *value = allocator->calloc(1, sizeof(Value));
  value->type = XJSON_VAL_BOOLEAN;
  value->size = 1;
  value->val.BOOLEAN = boolean;

  return value;
}

Value * XJSON_Value_5 (Token [], XJSONContext *, ErrInfo *, const Allocator *allocator) {

  Value *value = allocator->calloc(1, sizeof(Value));
  value->type = XJSON_VAL_NULL;
  value->size = 0;

  return value;
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

