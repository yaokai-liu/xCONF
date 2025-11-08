/* License
 *
 * xLR - Dynamic LR(1) Grammar Parser
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
 * Project Name: xLR
 * Module Name: grammar/tokenize
 * Filename: tokenizer.c
 * Creator: Yaokai Liu
 * Create Date: 2025-05-30
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include "tokenizer.h"
#include "tokenize.h"
#include "instance.h"
#include "generated/tokens.gen.h"
#include "generated/xCONF/action-table.gen.h"

struct SrcPosition {
  const char_t *filepath;
  const char_t *src;
  uint32_t offset;
  uint32_t lineno;
  uint32_t column;
};

typedef struct XCONFTokenizer {
  const Allocator *allocator;
  const XCONFInstance * instance;
  Stack *pos_stack;
  struct SrcPosition position;
} XCONFTokenizer;

XCONFTokenizer *XCONFTokenizer_new(const XCONFInstance *instance, const Allocator *allocator) {
  XCONFTokenizer *tokenizer = allocator->calloc(1, sizeof(XCONFTokenizer));
  tokenizer->allocator = allocator;
  tokenizer->position.lineno = 1;
  tokenizer->position.column = 1;
  tokenizer->position.offset = 0;
  tokenizer->position.src = nullptr;
  tokenizer->instance = instance;
  tokenizer->pos_stack = Stack_new(allocator);
  return tokenizer;
}

void XCONFTokenizer_enter(XCONFTokenizer *tokenizer, const char_t *src) {
  Stack_push(tokenizer->pos_stack, &tokenizer->position, sizeof(struct SrcPosition));
  tokenizer->position.lineno = 1;
  tokenizer->position.column = 1;
  tokenizer->position.offset = 0;
  tokenizer->position.src = src;
}

void XCONFTokenizer_exit(XCONFTokenizer *tokenizer) {
  Stack_pop(tokenizer->pos_stack, &tokenizer->position, sizeof(struct SrcPosition));
}

void XCONFTokenizer_concatSrc(XCONFTokenizer *tokenizer, const char_t *src) {
  tokenizer->position.src = src;
}

void XCONFTokenizer_destroy(XCONFTokenizer *tokenizer) {
  Stack_clear(tokenizer->pos_stack);
  tokenizer->allocator->free(tokenizer->pos_stack);
  tokenizer->allocator->free(tokenizer);
}

#define pText (position->src + position->offset)
uint32_t
XCONFTokenizer_next(XCONFTokenizer *tokenizer, Token *token, ErrInfo *errInfo, const Allocator *allocator) {
  Trie * const key_trie = tokenizer->instance->key_trie;
  Array * const key_array = tokenizer->instance->key_array;
  struct SrcPosition * const position = &tokenizer->position;
  position->offset += pass_space(pText, &position->lineno, &position->column);
  Terminal terminal = {};
  terminal.type = XCONF_TOKEN_BAD_TOKEN;
  terminal.location.lineno = position->lineno;
  terminal.location.column = position->column;
  terminal.location.offset = position->offset;
  const uint32_t length = single_tokenize(pText, &terminal, allocator);
  if (terminal.type == XCONF_TOKEN_BAD_TOKEN) {
    errInfo->start.lineno = position->lineno;
    errInfo->start.column = position->column;
    errInfo->start.offset = position->offset;
    errInfo->end.lineno = position->lineno;
    errInfo->end.column = position->column;
    errInfo->end.offset = position->offset;
    errInfo->code = XCONF_ERROR_UNRECOGNIZED_SYMBOL;
    return errInfo->code;
  }
  if (terminal.type == XCONF_TOKEN_KEY) {
    REFER(char_t) v_key = Trie_get(key_trie, terminal.value);
    if (!v_key) {
      v_key = Array_last_virt(key_array) + 1;
      Array_append(key_array, terminal.value, terminal.length + 1);
      Trie_set(key_trie, terminal.value, v_key);
    }
    allocator->free(terminal.value);
    terminal.value = v_key;
  }
  terminal2Token(&terminal, token);
  position->offset += length;
  position->column += length;
  return XCONF_SUCCESS;
}
