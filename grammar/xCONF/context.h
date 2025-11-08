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
 * Filename: context.h
 * Creator: Yaokai Liu
 * Create Date: 2025-06-07
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#ifndef XCONF_GRAMMAR_XCONF_CONTEXT_H
#define XCONF_GRAMMAR_XCONF_CONTEXT_H

#include "array.h"
#include "trie.h"
#include "target.h"
#include "stack.h"
#include "error_info.h"
#include "enum.h"

typedef struct XCONFContext {
  const Allocator *allocator;
  XCONFInstance *instance;
  Trie      *key_trie;  // Trie<char_t, REFER(char_t)>
  Array     *key_array; // Array<char_t>

  Array   *text_array;    // Array<char_t>
  Array   *value_array;   // Array<Value>
  Stack   *obj_stack;     // Stack<Object *>
  Object  *object;
  uint32_t path_action;
  bool     parsing_refer;
} XCONFContext;

#define UNINITIALIZED_VAL_INDEX (0)
#define UNINITIALIZED_VAL_REFER ((REFER(Value)) ((((uint64_t) XCONF_VALUE_ARRAY) << 32) | UNINITIALIZED_VAL_INDEX))

XCONFContext *XCONFContext_new(XCONFInstance *instance, const Allocator *allocator);
void XCONFContext_destroy(XCONFContext *context);

void XCONFContext_state_action(XCONFContext *context, uint32_t state, Token *, const Allocator *allocator);

void XCONFContext_enter(XCONFContext *context, Object *object);
void XCONFContext_exit(XCONFContext *context);
void XCONFContext_reset(XCONFContext *context);
void XCONFContext_clear(XCONFContext *context);

REFER(char_t) XCONFContext_new_text_content(XCONFContext *context, const char_t *text_content, uint32_t size);

#define XCONFContent_add_text_content(context, text_content, size) \
                         Array_append((context)->text_array, (text_content), (size))

uint32_t XCONFContext_buildObjectRefer(const XCONFContext *context, Object *object, ErrInfo *errInfo);

uint32_t XCONFContext_buildListRefer(const XCONFContext *context, const List *list, ErrInfo *errInfo);
REFER(Value) XCONFContext_getReferValue(const XCONFContext *context, const ReferArray *refer_array, ErrInfo *errInfo);

#endif //XCONF_GRAMMAR_XCONF_CONTEXT_H
