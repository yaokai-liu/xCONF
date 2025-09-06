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

static void fill_error_info(ErrInfo *errInfo, const Token *start, const Token *end);

List * XCONF_List_0 (Token args[], XCONFContext *, ErrInfo *, const Allocator *) {
  return args[1].value;
}

List * XCONF_List_1 (Token args[], XCONFContext *, ErrInfo *, const Allocator * ) {
  return args[1].value;
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

Pair * XCONF_Pair_0 (Token args[], XCONFContext *context, ErrInfo *errInfo, const Allocator *) {
  Path *path = args[1].value;
  Value *value = args[4].value;

  if (path->value) {
    errInfo->code = XCONF_ERROR_CONFLICT_KEY;
    fill_error_info(errInfo, &args[1], &args[4]);
    return nullptr;
  }

  if (!path->key) { return nullptr; }

  if (!Array_virt2real(context->key_array, path->key)) {
    Array_append((List *) path->key, &value, 1);
  } else {
    path->value = value;
  }

  return (Pair *) XCONF_TOKEN_Pair;
}

Pair * XCONF_Pair_1 (Token args[], XCONFContext *context, ErrInfo *errInfo, const Allocator *) {
  Path *path = args[1].value;
  Value *value = args[4].value;

  if (path->value) {
    errInfo->code = XCONF_ERROR_CONFLICT_KEY;
    fill_error_info(errInfo, &args[1], &args[4]);
  }

  if (!path->key) { return nullptr; }

  if (!Array_virt2real(context->key_array, path->key)) {
    Array_append((List *) path->key, &value, 1);
  } else {
    path->value = value;
  }

  return (Pair *) XCONF_TOKEN_Pair;
}

Pair * XCONF_Pair_2 (Token args[], XCONFContext *context, ErrInfo *errInfo, const Allocator *) {
  Path *path = args[1].value;
  Value *value = args[3].value;

  if (path->value) {
    errInfo->code = XCONF_ERROR_CONFLICT_KEY;
    fill_error_info(errInfo, &args[1], &args[3]);
    return nullptr;
  }

  if (!path->key) { return nullptr; }

  if (!Array_virt2real(context->key_array, path->key)) {
    Array_append((List *) path->key, &value, 1);
  } else {
    path->value = value;
  }

  return (Pair *) XCONF_TOKEN_Pair;
}

Pair * XCONF_Pair_3 (Token args[], XCONFContext *context, ErrInfo *errInfo, const Allocator *) {
  Path *path = args[0].value;
  Value *value = args[2].value;

  if (path->value) {
    errInfo->code = XCONF_ERROR_CONFLICT_KEY;
    fill_error_info(errInfo, &args[0], &args[2]);
    return nullptr;
  }

  if (!path->key) { return nullptr; }

  if (!Array_virt2real(context->key_array, path->key)) {
    Array_append((List *) path->key, &value, 1);
  } else {
    path->value = value;
  }

  return (Pair *) XCONF_TOKEN_Pair;
}

Pair * XCONF_Pair_4 (Token args[], XCONFContext *context, ErrInfo *errInfo, const Allocator *) {
  Path *path = args[0].value;
  Value *value = args[2].value;

  if (path->value) {
    errInfo->code = XCONF_ERROR_CONFLICT_KEY;
    fill_error_info(errInfo, &args[0], &args[2]);
    return nullptr;
  }

  if (!path->key) { return nullptr; }

  if (!Array_virt2real(context->key_array, path->key)) {
    Array_append((List *) path->key, &value, 1);
  } else {
    path->value = value;
  }

  return (Pair *) XCONF_TOKEN_Pair;
}

Pairs * XCONF_Pairs_0 (Token [], XCONFContext *, ErrInfo *, const Allocator *) {
  return (Pairs *) XCONF_TOKEN_Pairs;
}

Pairs * XCONF_Pairs_1 (Token [], XCONFContext *, ErrInfo *, const Allocator *) {
  return (Pairs *) XCONF_TOKEN_Pairs;
}

Path * XCONF_Path_0 (Token args[], XCONFContext *context, ErrInfo *errInfo, const Allocator *allocator) {
  Path *path = args[0].value;
  REFER(char_t) key = args[2].value;

  Pair *pair = path;
  if (!pair->value) {
    const Value val = { .type = XCONF_VAL_OBJECT, .size = 0, .val.OBJECT = Object_new(allocator) };
    Array_append(context->value_array, &val, 1);
    pair->value = Array_last_virt(context->value_array);
  }
  if (!Array_virt2real(context->key_array, pair->key)) {
    Array_append((List *) pair->key, &pair->value, 1);
  }
  Value *value = Array_virt2real(context->value_array, pair->value);
  if (value->type != XCONF_VAL_OBJECT) {
    errInfo->code = XCONF_ERROR_CONFLICT_KEY;
    fill_error_info(errInfo, &args[0], &args[0]);
    return nullptr;
  }
  const Object *object = value->val.OBJECT;
  REFER(Pair) v_pair = AVLTree_get(object->mapping, (uint64_t) key);
  if (!v_pair) {
    Array_append(object->pairs, &(Pair){.key = key, .value = nullptr}, 1);
    v_pair = Array_last_virt(object->pairs);
    AVLTree_set(object->mapping, (uint64_t) key, v_pair);
  }

  if (!pair->key || !Array_virt2real(context->key_array, pair->key)) { allocator->free(pair); }

  return Array_virt2real(object->pairs, v_pair);
}

Path * XCONF_Path_1 (Token args[], XCONFContext *context, ErrInfo *errInfo, const Allocator *allocator) {
  Path *path = args[0].value;
  Value *number = args[2].value;

  Pair *pair = path;
  if (!pair->value) {
    List *list = Array_new(sizeof(REFER(Value)), XCONF_REFER_VALUE_ARRAY, allocator);
    const Value val = { .type = XCONF_VAL_LIST, .size = 0, .val.LIST = list };
    Array_append(context->value_array, &val, 1);
    pair->value = Array_last_virt(context->value_array);
  }
  Value *value = Array_virt2real(context->value_array, pair->value);
  if (value->type != XCONF_VAL_LIST) {
    errInfo->code = XCONF_ERROR_CONFLICT_KEY;
    fill_error_info(errInfo, &args[0], &args[0]);
    return nullptr;
  }
  List *list = value->val.LIST;
  if (number->type != XCONF_VAL_I32) {
    errInfo->code = XCONF_ERROR_INDEX_OUT_OF_RANGE;
    fill_error_info(errInfo, &args[2], &args[2]);
    return nullptr;
  }
  uint32_t index = number->val.I32;
  if (index > Array_length(list)) {
    errInfo->code = XCONF_ERROR_INDEX_OUT_OF_RANGE;
    fill_error_info(errInfo, &args[2], &args[2]);
    return nullptr;
  }
  REFER(Value) *v_value = Array_real_addr(list, index);

  pair = allocator->calloc(1, sizeof(Pair));
  pair->value = v_value ? *v_value : nullptr;
  pair->key = (void *) list;
  return pair;
}

Path * XCONF_Path_2 (Token args[], XCONFContext *context, ErrInfo *, const Allocator *) {
  REFER(char_t) key = args[1].value;

  Object *object = context->object;
  REFER(Pair) v_pair = AVLTree_get(object->mapping, (uint64_t) key);
  if (!v_pair) {
    Array_append(object->pairs, &(Pair){.key = key, .value = nullptr}, 1);
    v_pair = Array_last_virt(object->pairs);
    AVLTree_set(object->mapping, (uint64_t) key, v_pair);
  }

  return Array_virt2real(object->pairs, v_pair);
}

Path * XCONF_Path_3 (Token args[], XCONFContext *context, ErrInfo *, const Allocator *) {
  REFER(char_t) key = args[0].value;

  Object *object = context->object;
  REFER(Pair) v_pair = AVLTree_get(object->mapping, (uint64_t) key);
  if (!v_pair) {
    Array_append(object->pairs, &(Pair){.key = key, .value = nullptr}, 1);
    v_pair = Array_last_virt(object->pairs);
    AVLTree_set(object->mapping, (uint64_t) key, v_pair);
  }

  return Array_virt2real(object->pairs, v_pair);
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

  texts->content = XCONFContext_new_text_content(context, text->content, text->length);
  texts->size = text->length;

  return texts;
}

Value * XCONF_Value_0 (Token args[], XCONFContext *context, ErrInfo *, const Allocator *) {
  Object *object = args->value;

  Value value = { .type = XCONF_VAL_OBJECT, .size = 0, .val.OBJECT = object };
  Array_append(context->value_array, &value, 1);
  REFER(Value) v_val = Array_last_virt(context->value_array);

  return v_val;
}

Value * XCONF_Value_1 (Token args[], XCONFContext *context, ErrInfo *, const Allocator *) {
  List *list = args->value;

  Value value = { .type = XCONF_VAL_LIST, .size = 0, .val.LIST = list };

  Array_append(context->value_array, &value, 1);
  REFER(Value) v_val = Array_last_virt(context->value_array);

  return v_val;
}

Value * XCONF_Value_2 (Token args[], XCONFContext *context, ErrInfo *, const Allocator *) {
  Value *value = args[0].value;

  Array_append(context->value_array, value, 1);
  REFER(Value) v_val = Array_last_virt(context->value_array);

  return v_val;
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

inline void fill_error_info(ErrInfo *errInfo, const Token *start, const Token *end) {
  errInfo->start.offset = start->start.offset;
  errInfo->start.lineno = start->start.lineno;
  errInfo->start.column = start->start.column;
  errInfo->end.offset   = end->end.offset;
  errInfo->end.lineno   = end->end.lineno;
  errInfo->end.column   = end->end.column;
}
