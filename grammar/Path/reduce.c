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
 * Module Name: grammar/Path
 * Filename: reduce.c
 * Creator: Yaokai Liu
 * Create Date: 2025-09-05
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include "xCONF/target.h"
#include "xCONF/context.h"

static void fill_error_info(ErrInfo *errInfo, const Token *start, const Token *end);


Path * XCONF_PATH_Path_0 (Token args[], XCONFContext *context, ErrInfo *errInfo, const Allocator *allocator) {
  Path *path = args[0].value;
  REFER(char_t) key = args[2].value;

  const Value *path_value = nullptr;
  if (!path->parent) {
    const REFER(Value) v_val = *(const REFER(Value) *)Dict_get(context->object, &path->key);
    if (v_val) { path_value = Array_virt2real(context->value_array, v_val); }
  } else {
    const Value *parent = Array_virt2real(context->value_array, path->parent);
    if (parent->type == XCONF_VAL_OBJECT) {
      const REFER(Value) v_val = *(const REFER(Value) *)Dict_get(parent->val.OBJECT, &path->key);
      if (v_val) { path_value = Array_virt2real(context->value_array, v_val); }
    } else if (parent->type == XCONF_VAL_LIST) {
      const REFER(Value) *p_v_val = Array_virt2real(parent->val.LIST, path->key);
      if (p_v_val) { path_value = Array_virt2real(context->value_array, *p_v_val); }
    }
  }

  if (path_value) {
    if (path_value->type != XCONF_VAL_OBJECT) {
      errInfo->code = XCONF_ERROR_CONFLICT_KEY_TYPE;
      fill_error_info(errInfo, &args[0], &args[2]);
      return nullptr;
    }
    REFER(Value) v_val = Array_last_virt(context->value_array);
    path->parent = v_val;
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
  REFER(Value) v_val = Array_last_virt(context->value_array);
  if (!path->parent) {
    Dict_set(context->object, &path->key, &v_val);
  } else {
    const Value *parent = Array_virt2real(context->value_array, path->parent);
    if (parent->type == XCONF_VAL_OBJECT) { Dict_set(parent->val.OBJECT, &path->key, &v_val); }
    else if (parent->type == XCONF_VAL_LIST) { Array_append(parent->val.LIST, &v_val, 1); }
  }
  path->parent = v_val;
  path->key = key;
  return path;
}

Path * XCONF_PATH_Path_1 (Token args[], XCONFContext *context, ErrInfo *errInfo, const Allocator *allocator) {
  Path *path = args[0].value;
  Value *number = args[2].value;

  if (number->type != XCONF_VAL_I32) {
    errInfo->code = XCONF_ERROR_INDEX_OUT_OF_RANGE;
    fill_error_info(errInfo, &args[2], &args[2]);
    return nullptr;
  }

  const Value *path_value = nullptr;
  if (!path->parent) {
    const REFER(Value) v_val = *(const REFER(Value) *)Dict_get(context->object, &path->key);
    if (v_val) { path_value = Array_virt2real(context->value_array, v_val); }
  } else {
    const Value *parent = Array_virt2real(context->value_array, path->parent);
    if (parent->type == XCONF_VAL_OBJECT) {
      const REFER(Value) v_val = *(const REFER(Value) *)Dict_get(parent->val.OBJECT, &path->key);
      if (v_val) { path_value = Array_virt2real(context->value_array, v_val); }
    } else if (parent->type == XCONF_VAL_LIST) {
      const REFER(Value) *p_v_val = Array_virt2real(parent->val.LIST, path->key);
      if (p_v_val) { path_value = Array_virt2real(context->value_array, *p_v_val); }
    }
  }

  if (path_value) {
    if (path_value->type != XCONF_VAL_LIST) {
      errInfo->code = XCONF_ERROR_CONFLICT_KEY_TYPE;
      fill_error_info(errInfo, &args[0], &args[3]);
      return nullptr;
    }
    const List *list = path_value->val.LIST;
    const uint32_t index = number->val.I32;
    allocator->free(number);
    if (index > Array_length(list)) {
      errInfo->code = XCONF_ERROR_INDEX_OUT_OF_RANGE;
      fill_error_info(errInfo, &args[2], &args[2]);
      return nullptr;
    }
    REFER(Value) v_val = Array_last_virt(context->value_array);
    path->parent = v_val;
    path->key = Array_virt_addr(list, index);
    return path;
  }

  if (context->path_action != XCONF_PATH_ACTION_BUILD) {
    errInfo->code = XCONF_ERROR_FIELD_UNDEFINED;
    fill_error_info(errInfo, &args[0], &args[0]);
    return nullptr;
  }

  const Value value = {.type = XCONF_VAL_LIST, .size = 0, .val.LIST = List_new(allocator)};
  Array_append(context->value_array, &value, 1);
  REFER(Value) v_val = Array_last_virt(context->value_array);
  if (!path->parent) {
    Dict_set(context->object, &path->key, &v_val);
  } else {
    const Value *parent = Array_virt2real(context->value_array, path->parent);
    if (parent->type == XCONF_VAL_OBJECT) { Dict_set(parent->val.OBJECT, &path->key, &v_val); }
    else if (parent->type == XCONF_VAL_LIST) { Array_append(parent->val.LIST, &v_val, 1); }
  }
  path = allocator->calloc(1, sizeof(Path));
  path->parent = v_val;
  path->key = Array_virt_addr(value.val.LIST, 0);
  return path;
}

Path * XCONF_PATH_Path_2 (Token args[], XCONFContext *, ErrInfo *, const Allocator *allocator) {
  REFER(char_t) key = args[1].value;

  Path *path = allocator->calloc(1, sizeof(Path));
  path->parent = nullptr;
  path->key = key;

  return path;
}

Path * XCONF_PATH_Path_3 (Token args[], XCONFContext *, ErrInfo *, const Allocator *allocator) {
  REFER(char_t) key = args[0].value;

  Path *path = allocator->calloc(1, sizeof(Path));
  path->parent = nullptr;
  path->key = key;
  return path;
}

Path * XCONF_PATH_Path_EXT (Token args[], XCONFContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

inline void fill_error_info(ErrInfo *errInfo, const Token *start, const Token *end) {
  errInfo->start.offset = start->start.offset;
  errInfo->start.lineno = start->start.lineno;
  errInfo->start.column = start->start.column;
  errInfo->end.offset   = end->end.offset;
  errInfo->end.lineno   = end->end.lineno;
  errInfo->end.column   = end->end.column;
}
