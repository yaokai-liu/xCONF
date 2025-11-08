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

#include <sys/ucontext.h>

#include "target.h"
#include "context.h"
#include "enum.h"
#include "generated/tokens.gen.h"
#include "error_info.h"

static void fill_error_info(ErrInfo *errInfo, const Token *start, const Token *end);

Imported * XCONF_Imported_0 (Token args[], XCONFContext *context, ErrInfo *errInfo, const Allocator *) {
  Texts *text = args[3].value;
  uint32_t update_method = (uint64_t) args[6].value;
  Object *object = args[6].value;
  const char_t *filepath = Array_virt2real(context->text_array, text->content);

  Object *imported = nullptr;
  const uint32_t result = XCONF_load(context->instance, filepath, &imported);
  if (result != XCONF_SUCCESS) {
    errInfo->code = XCONF_ERROR_FAILED_TO_IMPORT_CONFIG;
    errInfo->info = result;
    fill_error_info(errInfo, &args[3], &args[3]);
    return nullptr;
  }

  // TODO: this update is not correct, please try to resolve it.
  if (update_method == XCONF_UM_UPDATE) {
    Dict_update(imported, object, true);
  } else if (update_method == XCONF_UM_APPEND) {
    Dict_update(imported, object, false);
  } else if (update_method == XCONF_UM_REMOVE) {
    Dict_reduce(imported, object);
  }

  return imported;
}

Imported * XCONF_Imported_1 (Token args[], XCONFContext *context, ErrInfo *errInfo, const Allocator *) {
  Texts *text = args[3].value;
  const char_t *const filepath = Array_virt2real(context->text_array, text->content);

  Object *imported = nullptr;
  const uint32_t result = XCONF_load(context->instance, filepath, &imported);
  if (result != XCONF_SUCCESS) {
    errInfo->code = XCONF_ERROR_FAILED_TO_IMPORT_CONFIG;
    errInfo->info = result;
    fill_error_info(errInfo, &args[3], &args[3]);
    return nullptr;
  }

  return imported;
}

List * XCONF_List_0 (Token args[], XCONFContext *, ErrInfo *, const Allocator *) {
  return args[1].value;
}

List * XCONF_List_1 (Token args[], XCONFContext *, ErrInfo *, const Allocator * ) {
  return args[1].value;
}

Object * XCONF_Object_0 (Token [], XCONFContext *context, ErrInfo *, const Allocator *) {
  Object *object = context->object;
  XCONFContext_exit(context);
  return object;
}

Object * XCONF_Object_1 (Token [], XCONFContext *context, ErrInfo *, const Allocator *) {
  Object *object = context->object;
  XCONFContext_exit(context);
  return object;
}

Object * XCONF_Object_2 (Token args[], XCONFContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

Object * XCONF_Object_EXT (Token args[], XCONFContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

Pair * XCONF_Pair_0 (Token args[], XCONFContext *context, ErrInfo *, const Allocator *allocator) {
  Path *path = args[1].value;
  REFER(Value) v_value = args[4].value;

  Value *value = Array_virt2real(context->value_array, v_value);
  value->path = *path;

  if (!path->parent) {
    Dict_set(context->object, &path->key, &v_value);
  } else {
    const Value *parent = Array_virt2real(context->value_array, path->parent);
    if (parent->type == XCONF_VAL_OBJECT) { Dict_set(parent->val.OBJECT, &path->key, &v_value); }
    else if (parent->type == XCONF_VAL_LIST) { Array_append(parent->val.LIST, &v_value, 1); }
  }
  allocator->free(path);

  return (Pair *) XCONF_TOKEN_Pair;
}

Pair * XCONF_Pair_1 (Token args[], XCONFContext *context, ErrInfo *, const Allocator *allocator) {
  Path *path = args[1].value;
  REFER(Value) v_value = args[4].value;

  Value *value = Array_virt2real(context->value_array, v_value);
  value->path = *path;

  if (!path->parent) {
    Dict_set(context->object, &path->key, &v_value);
  } else {
    const Value *parent = Array_virt2real(context->value_array, path->parent);
    if (parent->type == XCONF_VAL_OBJECT) { Dict_set(parent->val.OBJECT, &path->key, &v_value); }
    else if (parent->type == XCONF_VAL_LIST) { Array_append(parent->val.LIST, &v_value, 1); }
  }
  allocator->free(path);

  return (Pair *) XCONF_TOKEN_Pair;
}

Pair * XCONF_Pair_2 (Token args[], XCONFContext *context, ErrInfo *, const Allocator *allocator) {
  Path *path = args[1].value;
  REFER(Value) v_value = args[3].value;

  Value *value = Array_virt2real(context->value_array, v_value);
  value->path = *path;

  if (!path->parent) {
    Dict_set(context->object, &path->key, &v_value);
  } else {
    const Value *parent = Array_virt2real(context->value_array, path->parent);
    if (parent->type == XCONF_VAL_OBJECT) { Dict_set(parent->val.OBJECT, &path->key, &v_value); }
    else if (parent->type == XCONF_VAL_LIST) { Array_append(parent->val.LIST, &v_value, 1); }
  }
  allocator->free(path);

  return (Pair *) XCONF_TOKEN_Pair;
}

Pair * XCONF_Pair_3 (Token args[], XCONFContext *context, ErrInfo *, const Allocator *allocator) {
  Path *path = args[0].value;
  REFER(Value) v_value = args[2].value;

  Value *value = Array_virt2real(context->value_array, v_value);
  value->path = *path;

  if (!path->parent) {
    Dict_set(context->object, &path->key, &v_value);
  } else {
    const Value *parent = Array_virt2real(context->value_array, path->parent);
    if (parent->type == XCONF_VAL_OBJECT) { Dict_set(parent->val.OBJECT, &path->key, &v_value); }
    else if (parent->type == XCONF_VAL_LIST) { Array_append(parent->val.LIST, &v_value, 1); }
  }
  allocator->free(path);

  return (Pair *) XCONF_TOKEN_Pair;
}

Pair * XCONF_Pair_4 (Token args[], XCONFContext *context, ErrInfo *, const Allocator *allocator) {
  Path *path = args[0].value;
  REFER(Value) v_value = args[2].value;

  Value *value = Array_virt2real(context->value_array, v_value);
  value->path = *path;

  if (!path->parent) {
    Dict_set(context->object, &path->key, &v_value);
  } else {
    const Value *parent = Array_virt2real(context->value_array, path->parent);
    if (parent->type == XCONF_VAL_OBJECT) { Dict_set(parent->val.OBJECT, &path->key, &v_value); }
    else if (parent->type == XCONF_VAL_LIST) { Array_append(parent->val.LIST, &v_value, 1); }
  }
  allocator->free(path);

  return (Pair *) XCONF_TOKEN_Pair;
}

Pairs * XCONF_Pairs_0 (Token [], XCONFContext *, ErrInfo *, const Allocator *) {
  return (Pairs *) XCONF_TOKEN_Pairs;
}

Pairs * XCONF_Pairs_1 (Token [], XCONFContext *, ErrInfo *, const Allocator *) {
  return (Pairs *) XCONF_TOKEN_Pairs;
}

Pairs * XCONF_Pairs_2 (Token [], XCONFContext *, ErrInfo *, const Allocator *) {
  return (Pairs *) XCONF_TOKEN_Pairs;
}

Path * XCONF_Path_0 (Token args[], XCONFContext *context, ErrInfo *errInfo, const Allocator *allocator) {
  Path *path = args[0].value;
  REFER(char_t) key = args[2].value;

  if (context->parsing_refer) {
    ReferArray *refer_array = (ReferArray *)path;
    const Refer refer = { .refer.key = key, .start = args[0].start, .end = args[2].end };
    Array_append(refer_array, &refer, 1);
    return (Path *) refer_array;
  }

  const REFER(Value) v_path_value = nullptr;
  if (!path->parent) {
    const REFER(Value) * p_v_path_value = Dict_get(context->object, &path->key);
    v_path_value = p_v_path_value ? *p_v_path_value : nullptr;
  } else {
    const Value *parent = Array_virt2real(context->value_array, path->parent);
    if (parent->type == XCONF_VAL_OBJECT) {
      const REFER(Value) * p_v_path_value = Dict_get(parent->val.OBJECT, &path->key);
      v_path_value = p_v_path_value ? *p_v_path_value : nullptr;
    } else if (parent->type == XCONF_VAL_LIST) {
      const REFER(Value) *p_v_path_value = Array_real_addr(parent->val.LIST, (uint64_t) path->key);
      v_path_value = p_v_path_value ? *p_v_path_value : nullptr;
    }
  }
  const Value *path_value = Array_virt2real(context->value_array, v_path_value);
  if (path_value) {
    if (path_value->type != XCONF_VAL_OBJECT) {
      errInfo->code = XCONF_ERROR_CONFLICT_KEY_TYPE;
      fill_error_info(errInfo, &args[0], &args[2]);
      return nullptr;
    }
    path->parent = v_path_value;
    path->key = key;
    return path;
  }

  if (context->path_action != XCONF_PATH_ACTION_BUILD) {
    errInfo->code = XCONF_ERROR_FIELD_UNDEFINED;
    fill_error_info(errInfo, &args[0], &args[0]);
    return nullptr;
  }
  const Value value = {.type = XCONF_VAL_OBJECT, .size = 0, .path = *path, .val.OBJECT = Object_new(allocator)};
  Array_append(context->value_array, &value, 1);
  v_path_value = Array_last_virt(context->value_array);
  if (!path->parent) {
    Dict_set(context->object, &path->key, &v_path_value);
  } else {
    const Value *parent = Array_virt2real(context->value_array, path->parent);
    if (parent->type == XCONF_VAL_OBJECT) { Dict_set(parent->val.OBJECT, &path->key, &v_path_value); }
    else if (parent->type == XCONF_VAL_LIST) { Array_append(parent->val.LIST, &v_path_value, 1); }
  }
  path->parent = v_path_value;
  path->key = key;
  return path;
}

Path * XCONF_Path_1 (Token args[], XCONFContext *context, ErrInfo *errInfo, const Allocator *allocator) {
  Path *path = args[0].value;
  Value *number = args[2].value;

  if (number->type != XCONF_VAL_I32) {
    errInfo->code = XCONF_ERROR_INDEX_OUT_OF_RANGE;
    fill_error_info(errInfo, &args[2], &args[2]);
    return nullptr;
  }
  const uint32_t index = number->val.I32;
  allocator->free(number);

  if (context->parsing_refer) {
    ReferArray *refer_array = (ReferArray *)path;
    const Refer refer = { .refer.index = index, .start = args[0].start, .end = args[2].end };
    Array_append(refer_array, &refer, 1);
    return (Path *) refer_array;
  }

  const REFER(Value) v_path_value = nullptr;
  if (!path->parent) {
    const REFER(Value) * p_v_path_value = Dict_get(context->object, &path->key);
    v_path_value = p_v_path_value ? *p_v_path_value : nullptr;
  } else {
    const Value *parent = Array_virt2real(context->value_array, path->parent);
    if (parent->type == XCONF_VAL_OBJECT) {
      const REFER(Value) * p_v_path_value = Dict_get(context->object, &path->key);
      v_path_value = p_v_path_value ? *p_v_path_value : nullptr;
    } else if (parent->type == XCONF_VAL_LIST) {
      const REFER(Value) *p_v_path_value = Array_real_addr(parent->val.LIST, (uint64_t) path->key);
      v_path_value = p_v_path_value ? *p_v_path_value : nullptr;
    }
  }
  const Value *path_value = Array_virt2real(context->value_array, v_path_value);
  if (path_value) {
    if (path_value->type != XCONF_VAL_LIST) {
      errInfo->code = XCONF_ERROR_CONFLICT_KEY_TYPE;
      fill_error_info(errInfo, &args[0], &args[3]);
      return nullptr;
    }
    const List *list = path_value->val.LIST;
    if (index > Array_length(list)) {
      errInfo->code = XCONF_ERROR_INDEX_OUT_OF_RANGE;
      fill_error_info(errInfo, &args[2], &args[2]);
      return nullptr;
    }
    path->parent = v_path_value;
    path->key = (void *) (uint64_t) index;
    return path;
  }

  if (context->path_action != XCONF_PATH_ACTION_BUILD) {
    errInfo->code = XCONF_ERROR_FIELD_UNDEFINED;
    fill_error_info(errInfo, &args[0], &args[0]);
    return nullptr;
  }

  const Value value = {.type = XCONF_VAL_LIST, .path = *path, .size = 0, .val.LIST = List_new(allocator)};
  Array_append(context->value_array, &value, 1);
  v_path_value = Array_last_virt(context->value_array);
  if (!path->parent) {
    Dict_set(context->object, &path->key, &v_path_value);
  } else {
    const Value *parent = Array_virt2real(context->value_array, path->parent);
    if (parent->type == XCONF_VAL_OBJECT) { Dict_set(parent->val.OBJECT, &path->key, &v_path_value); }
    else if (parent->type == XCONF_VAL_LIST) { Array_append(parent->val.LIST, &v_path_value, 1); }
  }
  path->parent = v_path_value;
  path->key = nullptr;
  return path;
}

Path * XCONF_Path_2 (Token args[], XCONFContext *context, ErrInfo *, const Allocator *allocator) {
  REFER(char_t) key = args[1].value;

  if (context->parsing_refer) {
    ReferArray *refer_array = ReferArray_new(allocator);
    const Refer refer = { .refer.key = key, .start = args[1].start, .end = args[1].end };
    Array_append(refer_array, &refer, 1);
    return (Path *) refer_array;
  }

  Path *path = allocator->calloc(1, sizeof(Path));
  path->parent = nullptr;
  path->key = key;

  return path;
}

Path * XCONF_Path_3 (Token args[], XCONFContext *context, ErrInfo *, const Allocator *allocator) {
  REFER(char_t) key = args[0].value;

  if (context->parsing_refer) {
    ReferArray *refer_array = ReferArray_new(allocator);
    const Refer refer = { .refer.key = key, .start = args[0].start, .end = args[0].end };
    Array_append(refer_array, &refer, 1);
    return (Path *) refer_array;
  }

  Path *path = allocator->calloc(1, sizeof(Path));
  path->parent = nullptr;
  path->key = key;

  return path;
}

Texts * XCONF_Texts_0 (Token args[], XCONFContext *context, ErrInfo *, const Allocator *allocator) {
  Texts *texts = args[0].value;
  WrapperedText *text = args[1].value;

  XCONFContent_add_text_content(context, text->content, text->length);
  texts->size += text->length;

  allocator->free(text->content);
  allocator->free(text);

  return texts;
}

Texts * XCONF_Texts_1 (Token args[], XCONFContext *context, ErrInfo *, const Allocator *allocator) {
  WrapperedText *text = args[0].value;

  Texts *texts = allocator->calloc(1, sizeof(Texts));

  texts->content = XCONFContext_new_text_content(context, text->content, text->length);
  texts->size = text->length;

  allocator->free(text->content);
  allocator->free(text);

  return texts;
}

Value * XCONF_Value_0 (Token args[], XCONFContext *context, ErrInfo *, const Allocator *) {
  Object *object = args->value;

  const Value value = { .type = XCONF_VAL_OBJECT, .size = 0, .val.OBJECT = object };
  Array_append(context->value_array, &value, 1);
  REFER(Value) v_value = Array_last_virt(context->value_array);

  const uint32_t count = Dict_count(object);
  const REFER(Value) *eles = Dict_elements(object);
  for (uint32_t i = 0; i < count; i ++) {
    Value *child_val = Array_virt2real(context->value_array, eles[i]);
    child_val->path.parent = v_value;
  }

  return v_value;
}

Value * XCONF_Value_1 (Token args[], XCONFContext *context, ErrInfo *, const Allocator *) {
  List *list = args->value;

  const Value value = { .type = XCONF_VAL_LIST, .size = 0, .val.LIST = list };
  Array_append(context->value_array, &value, 1);
  REFER(Value) v_value = Array_last_virt(context->value_array);

  const uint32_t count = Array_length(list);
  const REFER(Value) *eles = Array_first_real(list);
  for (uint32_t i = 0; i < count; i ++) {
    Value *child_val = Array_virt2real(context->value_array, eles[i]);
    child_val->path.parent = v_value;
  }

  return v_value;
}

Value * XCONF_Value_2 (Token args[], XCONFContext *context, ErrInfo *, const Allocator *allocator) {
  Value *value = args[0].value;

  Array_append(context->value_array, value, 1);
  REFER(Value) v_val = Array_last_virt(context->value_array);

  allocator->free(value);

  return v_val;
}

Value * XCONF_Value_3 (Token args[], XCONFContext *context, ErrInfo *, const Allocator *) {
  Texts *text = args->value;

  const Value value = { .type = XCONF_VAL_TEXT, .size = text->size, .val.TEXT = text };

  Array_append(context->value_array, &value, 1);
  REFER(Value) v_val = Array_last_virt(context->value_array);

  return v_val;
}

Value * XCONF_Value_4 (Token args[], XCONFContext *context, ErrInfo *, const Allocator *) {
  const bool boolean = (uint64_t) args->value;

  const Value value = { .type = XCONF_VAL_BOOLEAN, .size = 1, .val.BOOLEAN = boolean };

  Array_append(context->value_array, &value, 1);
  REFER(Value) v_val = Array_last_virt(context->value_array);

  return v_val;
}

Value * XCONF_Value_5 (Token [], XCONFContext *context, ErrInfo *, const Allocator *) {

  const Value value = { .type = XCONF_VAL_NULL, .size = 0, .val.U128 = 0 };

  Array_append(context->value_array, &value, 1);
  REFER(Value) v_val = Array_last_virt(context->value_array);

  return v_val;
}

Value * XCONF_Value_6 (Token args[], XCONFContext *context, ErrInfo *, const Allocator *) {
  ReferArray *refer_array = args[1].value;

  context->parsing_refer = false;

  const Value value = { .type = XCONF_VAL_LINKAGE, .size = 0, .val.REFER = refer_array };
  Array_append(context->value_array, &value, 1);
  REFER(Value) v_value = Array_last_virt(context->value_array);
  return v_value;
}

Values * XCONF_Values_0 (Token args[], XCONFContext *context, ErrInfo *, const Allocator *allocator) {
  REFER(Value) v_value = args[0].value;

  Values *values = Array_new(sizeof(REFER(Value)), XCONF_TOKEN_Value, allocator);

  Value *value = Array_virt2real(context->value_array, v_value);
  value->path.key = (void *) (uint64_t) Array_length(values);

  Array_append(values, &v_value, 1);

  return values;
}

Values * XCONF_Values_1 (Token args[], XCONFContext *context, ErrInfo *, const Allocator *) {
  Values *values = args[0].value;
  REFER(Value) v_value = args[2].value;

  Value *value = Array_virt2real(context->value_array, v_value);
  value->path.key = (void *) (uint64_t) Array_length(values);

  Array_append(values, &v_value, 1);

  return values;
}

Values * XCONF_Values_2 (Token [], XCONFContext *, ErrInfo *, const Allocator *allocator) {
  return Array_new(sizeof(REFER(Value)), XCONF_TOKEN_Value, allocator);
}

inline void fill_error_info(ErrInfo *errInfo, const Token *start, const Token *end) {
  errInfo->start.offset = start->start.offset;
  errInfo->start.lineno = start->start.lineno;
  errInfo->start.column = start->start.column;
  errInfo->end.offset   = end->end.offset;
  errInfo->end.lineno   = end->end.lineno;
  errInfo->end.column   = end->end.column;
}
