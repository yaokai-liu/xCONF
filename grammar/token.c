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
 * Module Name: grammar
 * Filename: token.c
 * Creator: Yaokai Liu
 * Create Date: 2025-06-09
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/


#include "allocator.h"
#include "array.h"
#include "xJSON/target.h"
#include "generated/tokens.gen.h"
#include "xJSON/token.h"

const char_t *get_name(uint16_t type) {
  return TOKEN_NAMES[type];
}


#define releaseArrayCase(array, ele)                        \
  case XJSON_TOKEN_##array: {                               \
    Array_reset(token->value, (destruct_t *) release##ele); \
    Array_destroy(token->value);                            \
    break;                                                  \
  }
#define releaseTokenCase(t, r)           \
  case XJSON_TOKEN_##t: {                \
    release##r(token->value, allocator); \
    break;                               \
  }

void releaseToken(Token *token, const Allocator *allocator) {
  switch (token->type) {
    // token array
    releaseArrayCase(Values, Value)
    releaseArrayCase(Pairs, Pair)
    // token
    releaseTokenCase(Texts, Texts)
    releaseTokenCase(Value, Value)
    releaseTokenCase(Pair, Pair)
    releaseTokenCase(Path, Path)
    releaseTokenCase(Object, Object)
    releaseTokenCase(List, List)
    releaseTokenCase(NUMBER, Value)
    releaseTokenCase(TEXT, WrapperedText)
    case XJSON_TOKEN_KEY: {
      if (token->value) { allocator->free(token->value); }
    }
    default: {}
  }
}
