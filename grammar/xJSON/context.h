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
 * Filename: context.h
 * Creator: Yaokai Liu
 * Create Date: 2025-06-07
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#ifndef XJSON_GRAMMAR_XJSON_CONTEXT_H
#define XJSON_GRAMMAR_XJSON_CONTEXT_H

#include "xJSON/error.h"
#include "array.h"
#include "trie.h"

typedef struct XJSONContext {
  Array *  ident_array;  // Array<char_t>
  Trie *   ident_trie;   // Trie<char_t, REFER(char_t)>
  Array *  text_array;  // Array<char_t>
} XJSONContext;

void XJSONContext_state_action(XJSONContext *context, uint32_t state, Token *, const Allocator *allocator);

REFER(char_t) XJSONContent_new_text_content(XJSONContext *context, const char_t *text_content, uint32_t size);

#define XJSONContent_add_text_content(context, text_content, size) \
                         Array_append((context)->text_array, (text_content), (size))


#endif //XJSON_GRAMMAR_XJSON_CONTEXT_H
