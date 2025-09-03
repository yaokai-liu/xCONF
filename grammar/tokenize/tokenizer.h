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
 * Filename: tokenizer.h
 * Creator: Yaokai Liu
 * Create Date: 2025-05-30
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#ifndef XCONF_GRAMMAR_TOKENIZE_TOKENIZER_H
#define XCONF_GRAMMAR_TOKENIZE_TOKENIZER_H

#include "token.h"
#include "xCONF/xCONF.h"
#include "array.h"
#include "trie.h"
#include "error_info.h"

typedef struct XCONFTokenizer XCONFTokenizer;

XCONFTokenizer *XCONFTokenizer_new(const char_t *src, Array *ident_array, Trie *ident_trie, const Allocator *allocator);

void XCONFTokenizer_setSrc(XCONFTokenizer *tokenizer, const char_t *src);
void XCONFTokenizer_concatSrc(XCONFTokenizer *tokenizer, const char_t *src);

uint32_t
XCONFTokenizer_next(XCONFTokenizer *tokenizer, Token *token, ErrInfo *errInfo, const Allocator *allocator);

void XCONFTokenizer_destroy(XCONFTokenizer *tokenizer);

#endif //XCONF_GRAMMAR_TOKENIZE_TOKENIZER_H
