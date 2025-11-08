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
 * Filename: context.c
 * Creator: Yaokai Liu
 * Create Date: 2025-06-07
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include "context.h"
#include "instance.h"
#include "meman-utils.h"
#include "generated/xCONF/action-table.gen.h"

XCONFContext *XCONFContext_new(XCONFInstance *instance, const Allocator *allocator) {
  XCONFContext *context = allocator->calloc(1, sizeof(XCONFContext));

  context->allocator = allocator;
  context->instance = instance;
  context->key_trie = instance->key_trie;
  context->key_array = instance->key_array;
  context->text_array = Array_new(sizeof(char_t), XCONF_TEXT_ARRAY, allocator);
  context->value_array = Array_new(sizeof(Value), XCONF_VALUE_ARRAY, allocator);
  context->obj_stack = Stack_new(allocator);
  context->object = nullptr;
  context->path_action = XCONF_PATH_ACTION_UNSET;
  context->parsing_refer = false;

  Array_append(context->text_array, "", 1);
  Array_append(context->key_array, "", 1);
  Array_append(context->value_array, &(Value){ .type = XCONF_VAL_UNINITIALIZED, .path = {.parent = nullptr, .key = nullptr}, .size = 0, .val.U256 = 0 }, 1);

  return context;
}

void XCONFContext_destroy(XCONFContext *context) {
  releasePrimeArray(context->text_array);
  Array_reset(context->value_array, (destruct_t *) releaseValue);
  Array_destroy(context->value_array);
  XCONFContext_clear(context);
  context->allocator->free(context->obj_stack);

  context->key_trie = nullptr;
  context->key_array = nullptr;
  context->text_array = nullptr;
  context->value_array = nullptr;
  context->obj_stack = nullptr;
  context->object = nullptr;
  context->parsing_refer = false;

  context->allocator->free(context);
}

void XCONFContext_state_action(XCONFContext *context, uint32_t state, Token *, const Allocator *) {
  if (context->path_action == XCONF_PATH_ACTION_ACCESS) { return; }
  switch (state) {
    case XCONF_state_LEFT_BRACKET:
    case XCONF_state_LEFT_BRACKET_Path_COLON_LEFT_BRACKET:
    case XCONF_state_LEFT_BRACKET_Path_ASSIGN_LEFT_SQUARE_BRACKET_LEFT_BRACKET:
    {
      XCONFContext_enter(context, Object_new(context->allocator));
      break;
    }
    case XCONF_state_LEFT_BRACKET_Path_ASSIGN_AT:
    case XCONF_state_LEFT_BRACKET_Path_ASSIGN_LEFT_SQUARE_BRACKET_AT: {
      context->parsing_refer = true;
      break;
    }
    default:{}
  }
}

inline REFER(char_t) XCONFContext_new_text_content(XCONFContext *context, const char_t *text_content, uint32_t size) {
  Array_append(context->text_array, "\0", 1);
  REFER(char_t) v_content = Array_last_virt(context->text_array) + 1;
  Array_append(context->text_array, text_content, size);
  return v_content;
}

inline void XCONFContext_enter(XCONFContext *context, Object *object) {
  Stack_push(context->obj_stack, &context->object, sizeof(Object *));
  context->object = object;
}

inline void XCONFContext_exit(XCONFContext *context) {
  Stack_pop(context->obj_stack, &context->object, sizeof(Object *));
}

void XCONFContext_reset(XCONFContext *context) {
  Array_reset(context->text_array, nullptr);
  Array_reset(context->value_array, (destruct_t *) releaseValue);
  context->path_action = XCONF_PATH_ACTION_UNSET;
}

void XCONFContext_clear(XCONFContext *context) {
  do {
    if (context->object) {
      Dict_destroy(context->object);
      context->object = nullptr;
    }
    XCONFContext_exit(context);
  } while (context->object || !Stack_empty(context->obj_stack));
  Stack_clear(context->obj_stack);
  context->object = nullptr;
}

REFER(Value) XCONFContext_getReferValue(const XCONFContext *context, const ReferArray *refer_array, ErrInfo *errInfo) {
  const uint32_t n_refers = Array_length(refer_array);
  const Refer *refers = Array_first_real(refer_array);
  const REFER(Value) *p_v_refer_val = Dict_get(context->object, &(refers[0].refer.key));
  if (!p_v_refer_val) {
    errInfo->code = XCONF_ERROR_NO_SUCH_KEY;
    errInfo->start = refers[0].start;
    errInfo->end = refers[0].end;
    return nullptr;
  }
  for (uint32_t j = 1; j < n_refers; j ++) {
    const Value *refer_val = Array_virt2real(context->value_array, *p_v_refer_val);
    if (refer_val->type == XCONF_VAL_OBJECT) {
      p_v_refer_val = Dict_get(refer_val->val.OBJECT, &(refers[j].refer.key));
    } else if (refer_val->type == XCONF_VAL_LIST) {
      p_v_refer_val = Array_real_addr(refer_val->val.LIST, refers[j].refer.index);
    } else {
      if (j != n_refers - 1) {
        errInfo->code = XCONF_ERROR_NON_INDEXABLE_VALUE;
        errInfo->start = refers[0].start;
        errInfo->end = refers[j].end;
        return nullptr;
      }
    }
    if (!p_v_refer_val) {
      errInfo->code = XCONF_ERROR_NO_SUCH_KEY;
      errInfo->start = refers[j].start;
      errInfo->end = refers[j].end;
      return nullptr;
    }
  }
  const Value *refer_val = Array_virt2real(context->value_array, *p_v_refer_val);

  if (refer_val->type != XCONF_VAL_LINKAGE) {
    return Array_real2virt(context->value_array, refer_val);
  }

  refer_array = refer_val->val.REFER;
  REFER(Value) v_val = XCONFContext_getReferValue(context, refer_array, errInfo);
  if (Array_virt2real(context->value_array, v_val) == refer_val) {
    errInfo->code = XCONF_ERROR_SELF_REFER;
    errInfo->start = refers[0].start;
    errInfo->end = refers[n_refers - 1].end;
    return nullptr;
  }
  return v_val;
}

uint32_t XCONFContext_buildObjectRefer(const XCONFContext *context, Object *object, ErrInfo *errInfo) {
  const uint32_t count = Dict_count(object);
  REFER(Value) *values = Dict_elements(object);
  for (uint32_t i = 0; i < count; i++) {
    Value *const value = Array_virt2real(context->value_array, values[i]);
    if (value->type == XCONF_VAL_LINKAGE) {
      ReferArray *refer_array = value->val.REFER;
      REFER(Value) v_val = XCONFContext_getReferValue(context, refer_array, errInfo);
      if (!v_val) { return errInfo->code; }
      releasePrimeArray(refer_array);
      value->val.REFER = v_val;
      value->type = XCONF_VAL_LINKED;
    } else if (value->type == XCONF_VAL_OBJECT) {
      XCONFContext_buildObjectRefer(context, value->val.OBJECT, errInfo);
    } else if (value->type == XCONF_VAL_LIST) {
      XCONFContext_buildListRefer(context, value->val.LIST, errInfo);
    }
  }
  return XCONF_SUCCESS;
}

uint32_t XCONFContext_buildListRefer(const XCONFContext *context, const List *list, ErrInfo *errInfo) {
  const uint32_t count = Array_length(list);
  REFER(Value) *values = Array_first_real(list);
  for (uint32_t i = 0; i < count; i++) {
    Value *const value = Array_virt2real(context->value_array, values[i]);
    if (value->type == XCONF_VAL_LINKAGE) {
      ReferArray *refer_array = value->val.REFER;
      REFER(Value) v_val = XCONFContext_getReferValue(context, refer_array, errInfo);
      if (!v_val) { return errInfo->code; }
      releasePrimeArray(refer_array);
      value->val.REFER = v_val;
      value->type = XCONF_VAL_LINKED;
    } else if (value->type == XCONF_VAL_OBJECT) {
      XCONFContext_buildObjectRefer(context, value->val.OBJECT, errInfo);
    } else if (value->type == XCONF_VAL_LIST) {
      XCONFContext_buildListRefer(context, value->val.LIST, errInfo);
    }
  }
  return XCONF_SUCCESS;
}
