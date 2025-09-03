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
 * Module Name: grammar/xCONF
 * Filename: reduce.c
 * Creator: Yaokai Liu
 * Create Date: 2025-06-07
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include "target.h"
#include "context.h"
#include "generated/tokens.gen.h"
#include "error_info.h"

List * XCONF_List_0 (Token args[], XCONFContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

List * XCONF_List_1 (Token args[], XCONFContext *, ErrInfo *, const Allocator * ) {
  return args[0].value;
}

Object * XCONF_Object_0 (Token [], XCONFContext *context, ErrInfo *, const Allocator *) {
  return context->object;
}

Object * XCONF_Object_1 (Token [], XCONFContext *context, ErrInfo *, const Allocator *) {
  return context->object;
}

Object * XCONF_Object_EXT (Token args[], XCONFContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

Pair * XCONF_Pair_0 (Token args[], XCONFContext *context, ErrInfo *, const Allocator *allocator) {
  Path *path = args[1].value;
  Value *value = args[4].value;

  Value *val = Array_virt2real(context->value_array, path);
  if (val->type != XCONF_VAL_UNINITIALIZED) { return nullptr; }

  allocator->memcpy(val, value, sizeof(Value));

  return (Pair *) XCONF_TOKEN_Pair;
}

Pair * XCONF_Pair_1 (Token args[], XCONFContext *context, ErrInfo *, const Allocator *allocator) {
  Path *path = args[1].value;
  Value *value = args[3].value;

  Value *val = Array_virt2real(context->value_array, path);
  if (val->type != XCONF_VAL_UNINITIALIZED) { return nullptr; }

  allocator->memcpy(val, value, sizeof(Value));

  return (Pair *) XCONF_TOKEN_Pair;
}

Pair * XCONF_Pair_2 (Token args[], XCONFContext *context, ErrInfo *, const Allocator *allocator) {
  Path *path = args[0].value;
  Value *value = args[2].value;

  Value *val = Array_virt2real(context->value_array, path);
  if (val->type != XCONF_VAL_UNINITIALIZED) { return nullptr; }

  allocator->memcpy(val, value, sizeof(Value));
  return (Pair *) XCONF_TOKEN_Pair;
}

Pairs * XCONF_Pairs_0 (Token [], XCONFContext *, ErrInfo *, const Allocator *) {
  return (Pairs *) XCONF_TOKEN_Pairs;
}

Pairs * XCONF_Pairs_1 (Token [], XCONFContext *, ErrInfo *, const Allocator *) {
  return (Pairs *) XCONF_TOKEN_Pairs;
}

Path * XCONF_Path_0 (Token args[], XCONFContext *context, ErrInfo *, const Allocator *) {
  Path *path = args[0].value;
  REFER(char_t) key = args[2].value;

  Value *obj = Array_virt2real(context->value_array, path);
  if (obj->type == XCONF_VAL_UNINITIALIZED) {
    obj->val.OBJECT = Object_new();
    obj->type = XCONF_VAL_OBJECT;
    obj->size = 0;
  } else if (obj->type != XCONF_VAL_OBJECT) {
    return nullptr;
  }

  Object *object = obj->val.OBJECT;
  REFER(Value) v_val = AVLTree_get(object->mapping, (uint64_t) key);
  if (!v_val) {
    Value val = {.type = XCONF_VAL_UNINITIALIZED};
    Array_append(context->value_array, &val, 1);
    Array_append(object->keys, &key, 1);
    v_val = Array_last_virt(context->value_array);
    AVLTree_set(object->mapping, (uint64_t) key, v_val);
  }

  return v_val;
}

Path * XCONF_Path_1 (Token args[], XCONFContext *context, ErrInfo *, const Allocator *) {
  REFER(char_t) key = args[1].value;

  Object *object = context->object;
  REFER(Value) v_val = AVLTree_get(object->mapping, (uint64_t) key);
  if (!v_val) {
    Value val = {.type = XCONF_VAL_UNINITIALIZED};
    Array_append(context->value_array, &val, 1);
    Array_append(object->keys, &key, 1);
    v_val = Array_last_virt(context->value_array);
    AVLTree_set(object->mapping, (uint64_t) key, v_val);
  }

  return v_val;
}

Path * XCONF_Path_2 (Token args[], XCONFContext *context, ErrInfo *, const Allocator *) {
  REFER(char_t) key = args[0].value;

  Object *object = context->object;
  REFER(Value) v_val = AVLTree_get(object->mapping, (uint64_t) key);
  if (!v_val) {
    Value val = {.type = XCONF_VAL_UNINITIALIZED};
    Array_append(context->value_array, &val, 1);
    Array_append(object->keys, &key, 1);
    v_val = Array_last_virt(context->value_array);
    AVLTree_set(object->mapping, (uint64_t) key, v_val);
  }

  return v_val;
}

Texts * XCONF_Texts_0 (Token args[], XCONFContext *context, ErrInfo *, const Allocator *) {
  Texts *texts = args[0].value;
  WrapperedText *text = args[1].value;

  XCONFContent_add_text_content(context, text->content, text->length);
  texts->size += text->length;

  return texts;
}

Texts * XCONF_Texts_1 (Token args[], XCONFContext *context, ErrInfo *, const Allocator *allocator) {
  WrapperedText *text = args[0].value;

  Texts *texts = allocator->calloc(1, sizeof(Texts));

  texts->content = XCONFContent_new_text_content(context, text->content, text->length);
  texts->size = text->length;

  return texts;
}

Value * XCONF_Value_0 (Token args[], XCONFContext *context, ErrInfo *, const Allocator *) {
  Object *object = args->value;

  Value value = { .type = XCONF_VAL_OBJECT, .size = Array_length(object->keys), .val.OBJECT = object };
  Array_append(context->value_array, &value, 1);
  REFER(Value) v_val = Array_last_virt(context->value_array);

  return v_val;
}

Value * XCONF_Value_1 (Token args[], XCONFContext *context, ErrInfo *, const Allocator *) {
  List *list = args->value;

  Value value = { .type = XCONF_VAL_LIST, .size = Array_length(list), .val.LIST = list };

  Array_append(context->value_array, &value, 1);
  REFER(Value) v_val = Array_last_virt(context->value_array);

  return v_val;
}

Value * XCONF_Value_2 (Token args[], XCONFContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

Value * XCONF_Value_3 (Token args[], XCONFContext *context, ErrInfo *, const Allocator *) {
  Texts *text = args->value;

  Value value = { .type = XCONF_VAL_TEXT, .size = text->size, .val.TEXT = text };

  Array_append(context->value_array, &value, 1);
  REFER(Value) v_val = Array_last_virt(context->value_array);

  return v_val;
}

Value * XCONF_Value_4 (Token args[], XCONFContext *context, ErrInfo *, const Allocator *) {
  bool boolean = (uint64_t) args->value;

  Value value = { .type = XCONF_VAL_BOOLEAN, .size = 1, .val.BOOLEAN = boolean };

  Array_append(context->value_array, &value, 1);
  REFER(Value) v_val = Array_last_virt(context->value_array);

  return v_val;
}

Value * XCONF_Value_5 (Token [], XCONFContext *context, ErrInfo *, const Allocator *) {

  Value value = { .type = XCONF_VAL_NULL, .size = 0, .val.U128 = 0 };

  Array_append(context->value_array, &value, 1);
  REFER(Value) v_val = Array_last_virt(context->value_array);

  return v_val;
}

Values * XCONF_Values_0 (Token args[], XCONFContext *, ErrInfo *, const Allocator *allocator) {
  REFER(Value) value = args[0].value;

  Values *values = Array_new(sizeof(REFER(Value)), XCONF_TOKEN_Value, allocator);

  Array_append(values, &value, 1);

  return values;
}

Values * XCONF_Values_1 (Token args[], XCONFContext *, ErrInfo *, const Allocator *) {
  Values *values = args[0].value;
  REFER(Value) value = args[2].value;

  Array_append(values, &value, 1);

  return values;
}

