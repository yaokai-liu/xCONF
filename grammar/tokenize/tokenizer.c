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
#include "generated/tokens.gen.h"
#include "generated/xCONF/action-table.gen.h"

typedef struct XCONFTokenizer {
  const Allocator *allocator;
  const char_t *src;
  uint32_t offset;
  uint32_t lineno;
  uint32_t column;
  Array *  ident_array;  // Array<Identifier>
  Trie *   ident_trie;   // Trie<char_t, Identifier>
} XCONFTokenizer;

XCONFTokenizer *XCONFTokenizer_new(const char_t *src, Array *ident_array, Trie *ident_trie, const Allocator *allocator) {
  XCONFTokenizer *tokenizer = allocator->calloc(1, sizeof(XCONFTokenizer));
  tokenizer->ident_array = ident_array;
  tokenizer->ident_trie = ident_trie;
  tokenizer->allocator = allocator;
  tokenizer->lineno = 1;
  tokenizer->column = 1;
  tokenizer->offset = 0;
  tokenizer->src = src;
  return tokenizer;
}

void XCONFTokenizer_setSrc(XCONFTokenizer *tokenizer, const char_t *src) {
  tokenizer->lineno = 1;
  tokenizer->column = 1;
  tokenizer->offset = 0;
  tokenizer->src = src;
}

void XCONFTokenizer_concatSrc(XCONFTokenizer *tokenizer, const char_t *src) {
  tokenizer->src = src;
}

void XCONFTokenizer_destroy(XCONFTokenizer *tokenizer) {
  tokenizer->allocator->free(tokenizer);
}

#define pText (tokenizer->src + tokenizer->offset)
uint32_t
XCONFTokenizer_next(XCONFTokenizer *tokenizer, Token *token, ErrInfo *errInfo, const Allocator *allocator) {
  tokenizer->offset += pass_space(pText, &tokenizer->lineno, &tokenizer->column);
  Terminal terminal = {};
  terminal.type = XCONF_TOKEN_BAD_TOKEN;
  terminal.location.lineno = tokenizer->lineno;
  terminal.location.column = tokenizer->column;
  terminal.location.offset = tokenizer->offset;
  const uint32_t length = single_tokenize(pText, &terminal, allocator);
  if (terminal.type == XCONF_TOKEN_BAD_TOKEN) {
    errInfo->lineno = tokenizer->lineno;
    errInfo->column = tokenizer->column;
    errInfo->offset = tokenizer->offset;
    errInfo->code = XCONF_ERROR_UNRECOGNIZED_SYMBOL;
    return errInfo->code;
  }
  if (terminal.type == XCONF_TOKEN_KEY) {
    REFER(char_t) v_key = Trie_get(tokenizer->ident_trie, terminal.value);
    if (!v_key) {
      v_key = Array_last_virt(tokenizer->ident_array) + 1;
      Array_append(tokenizer->ident_array, terminal.value, terminal.length + 1);
      Trie_set(tokenizer->ident_trie, terminal.value, v_key);
    }
    allocator->free(terminal.value);
    terminal.value = v_key;
  }
  terminal2Token(&terminal, token);
  tokenizer->offset += length;
  tokenizer->column += length;
  return XCONF_SUCCESS;
}
