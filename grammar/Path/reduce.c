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

Path * XCONF_PATH_Path_0 (Token args[], XCONFContext *context, ErrInfo *errInfo, const Allocator * allocator) {
  Path *path = args[0].value;
  REFER(char_t) key = args[2].value;

  if (context->path_action == XCONF_PATH_ACTION_BUILD) {
    if (!path->value) {
      const Value val = { .type = XCONF_VAL_OBJECT, .size = 0, .val.OBJECT = Object_new(allocator) };
      Array_append(context->value_array, &val, 1);
      path->value = Array_last_virt(context->value_array);
    }
    if (!Array_virt2real(context->key_array, path->key)) {
      List *list = (List *) path->key;
      uint32_t n_vals = Array_length(list);
      REFER(Value) *vals = Array_first_real(list);
      for (uint32_t i = 0; i < n_vals; i++) {
        if (vals[i] == path->value) { goto __has_value; }
      }
      Array_append(list, &path->value, 1);
      __has_value:
    }
    Value *value = Array_virt2real(context->value_array, path->value);
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

    if (!Array_virt2real(context->key_array, path->key)) { allocator->free(path); }

    return Array_virt2real(object->pairs, v_pair);
  }
  if (context->path_action == XCONF_PATH_ACTION_ACCESS) {
    if (!path->value) {
      errInfo->code = XCONF_ERROR_FIELD_UNDEFINED;
      fill_error_info(errInfo, &args[0], &args[0]);
      return nullptr;
    }
    const Value *value = Array_virt2real(context->value_array, path->value);
    if (!value) {
      errInfo->code = XCONF_ERROR_FIELD_UNDEFINED;
      fill_error_info(errInfo, &args[0], &args[0]);
      return nullptr;
    }
    if (value->type != XCONF_VAL_OBJECT) {
      errInfo->code = XCONF_ERROR_CONFLICT_KEY;
      fill_error_info(errInfo, &args[0], &args[0]);
      return nullptr;
    }
    const Object *object = value->val.OBJECT;
    REFER(Pair) v_pair = AVLTree_get(object->mapping, (uint64_t) key);
    if (!v_pair) {
      errInfo->code = XCONF_ERROR_NO_SUCH_KEY;
      fill_error_info(errInfo, &args[2], &args[2]);
      return nullptr;
    }
    return Array_virt2real(object->pairs, v_pair);
  }

  errInfo->code = XCONF_ERROR_INVALID_PATH_ACTION;
  return nullptr;
}

Path * XCONF_PATH_Path_1 (Token args[], XCONFContext *context, ErrInfo *errInfo, const Allocator *allocator) {
  Path *path = args[0].value;
  Value *number = args[2].value;

  if (context->path_action == XCONF_PATH_ACTION_BUILD) {
    if (!path->value) {
      List *list = Array_new(sizeof(REFER(Value)), XCONF_REFER_VALUE_ARRAY, allocator);
      const Value val = { .type = XCONF_VAL_LIST, .size = 0, .val.LIST = list };
      Array_append(context->value_array, &val, 1);
      path->value = Array_last_virt(context->value_array);
    }
    Value *value = Array_virt2real(context->value_array, path->value);
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

    Pair *pair = allocator->calloc(1, sizeof(Pair));
    pair->value = v_value ? *v_value : nullptr;
    pair->key = (void *) list;
    return pair;
  }
  if (context->path_action == XCONF_PATH_ACTION_ACCESS) {
    if (!path->value) {
      errInfo->code = XCONF_ERROR_FIELD_UNDEFINED;
      fill_error_info(errInfo, &args[0], &args[0]);
      return nullptr;
    }
    Value *value = Array_virt2real(context->value_array, path->value);
    if (value->type != XCONF_VAL_LIST) {
      errInfo->code = XCONF_ERROR_CONFLICT_KEY;
      fill_error_info(errInfo, &args[0], &args[0]);
      return nullptr;
    }
    const List *list = value->val.LIST;
    if (number->type != XCONF_VAL_I32) {
      errInfo->code = XCONF_ERROR_INDEX_OUT_OF_RANGE;
      fill_error_info(errInfo, &args[2], &args[2]);
      return nullptr;
    }
    uint32_t index = number->val.I32;
    if (index >= Array_length(list)) {
      errInfo->code = XCONF_ERROR_INDEX_OUT_OF_RANGE;
      fill_error_info(errInfo, &args[2], &args[2]);
      return nullptr;
    }
    REFER(Value) *v_value = Array_real_addr(list, index);

    Pair *pair = allocator->calloc(1, sizeof(Pair));
    pair->value = *v_value;
    pair->key = nullptr;
    return pair;
  }

  errInfo->code = XCONF_ERROR_INVALID_PATH_ACTION;
  return nullptr;
}

Path * XCONF_PATH_Path_2 (Token args[], XCONFContext *context, ErrInfo *errInfo, const Allocator *) {
  REFER(char_t) key = args[1].value;

  Object *object = context->object;
  if (context->path_action == XCONF_PATH_ACTION_BUILD) {
    REFER(Pair) v_pair = AVLTree_get(object->mapping, (uint64_t) key);
    if (!v_pair) {
      Array_append(object->pairs, &(Pair){.key = key, .value = nullptr}, 1);
      v_pair = Array_last_virt(object->pairs);
      AVLTree_set(object->mapping, (uint64_t) key, v_pair);
    }
    return Array_virt2real(object->pairs, v_pair);
  }
  if (context->path_action == XCONF_PATH_ACTION_ACCESS) {
    REFER(Pair) v_pair = AVLTree_get(object->mapping, (uint64_t) key);
    if (!v_pair) {
      errInfo->code = XCONF_ERROR_NO_SUCH_KEY;
      fill_error_info(errInfo, &args[1], &args[1]);
      return nullptr;
    }
    return Array_virt2real(object->pairs, v_pair);
  }
  errInfo->code = XCONF_ERROR_INVALID_PATH_ACTION;
  return nullptr;
}

Path * XCONF_PATH_Path_3 (Token args[], XCONFContext *context, ErrInfo *errInfo, const Allocator *) {
  REFER(char_t) key = args[0].value;

  Object *object = context->object;
  if (context->path_action == XCONF_PATH_ACTION_BUILD) {
    REFER(Pair) v_pair = AVLTree_get(object->mapping, (uint64_t) key);
    if (!v_pair) {
      Array_append(object->pairs, &(Pair){.key = key, .value = nullptr}, 1);
      v_pair = Array_last_virt(object->pairs);
      AVLTree_set(object->mapping, (uint64_t) key, v_pair);
    }
    return Array_virt2real(object->pairs, v_pair);
  }
  if (context->path_action == XCONF_PATH_ACTION_ACCESS) {
    REFER(Pair) v_pair = AVLTree_get(object->mapping, (uint64_t) key);
    if (!v_pair) {
      errInfo->code = XCONF_ERROR_NO_SUCH_KEY;
      fill_error_info(errInfo, &args[0], &args[1]);
      return nullptr;
    }
    return Array_virt2real(object->pairs, v_pair);
  }
  errInfo->code = XCONF_ERROR_INVALID_PATH_ACTION;
  return nullptr;
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
