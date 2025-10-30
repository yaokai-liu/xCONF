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
#include "meman-utils.h"
#include "generated/xCONF/action-table.gen.h"

XCONFContext *XCONFContext_new(const Allocator *allocator) {
  XCONFContext *context = allocator->calloc(1, sizeof(XCONFContext));

  context->allocator = allocator;
  context->key_trie = Trie_new(sizeof(char_t), (key_t *) char2u64, allocator);
  context->key_array = Array_new(sizeof(char_t),XCONF_KEY_ARRAY, allocator);
  context->text_array = Array_new(sizeof(char_t), XCONF_TEXT_ARRAY, allocator);
  context->value_array = Array_new(sizeof(Value), XCONF_VALUE_ARRAY, allocator);
  context->obj_stack = Stack_new(allocator);
  context->object = nullptr;
  context->path_action = XCONF_PATH_ACTION_UNSET;

  Array_append(context->text_array, "", 1);
  Array_append(context->key_array, "", 1);
  Array_append(context->value_array, &(Value){ .type = XCONF_VAL_UNINITIALIZED, .size = 0, .val.U256 = 0 }, 1);

  return context;
}

void XCONFContext_destroy(XCONFContext *context) {
  Trie_destroy(context->key_trie);
  releasePrimeArray(context->key_array);
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
    case XCONF_state_Object:
    case XCONF_state_LEFT_BRACKET_Path_COLON_Object:
    case XCONF_state_LEFT_BRACKET_Path_ASSIGN_LEFT_SQUARE_BRACKET_Object:
    {
      XCONFContext_exit(context);
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

void XCONFContext_clear(XCONFContext *context) {
  do {
    if (context->object) {
      Dict_destroy(context->object);
    }
    XCONFContext_exit(context);
  } while (context->object || !Stack_empty(context->obj_stack));
  Stack_clear(context->obj_stack);
  context->object = nullptr;
}
