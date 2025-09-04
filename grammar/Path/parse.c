/* License
 *
 * ${PROJECT_DESCRIPTION}
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
 * Filename: parse.c
 * Creator: Yaokai Liu
 * Create Date: 2025-09-04
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include "parse.h"
#include "action.h"
#include "generated/tokens.gen.h"
#include "generated/xCONF/action-table.gen.h"
#include "generated/xCONF/rules.gen.h"


Path *parsePath(XCONFTokenizer *tokenizer, XCONFContext *context, ErrInfo *errInfo, const Allocator *allocator) {
  Token token = {};
  uint32_t argc = 0;
  Token args[4] = {};
  uint32_t state = XCONF_state_LEFT_BRACKET_LEFT_SQUARE_BRACKET;

  uint32_t status = XCONFTokenizer_next(tokenizer, &token, errInfo, allocator);
  if (status != XCONF_SUCCESS) { return nullptr; }
  while (true) {
    if (token.type == XCONF_TOKEN_TERMINATOR) { return args[0].value; }
    if (token.type != XCONF_TOKEN_DOT && token.type != XCONF_TOKEN_KEY) { return nullptr; }
    const struct grammar_action *act = getParseAction(state, token.type);
    if (!act) { return nullptr; }
    if (act->action == XCONF_action_stack) {
      state = act->offset;
      args[argc] = token;
      status = XCONFTokenizer_next(tokenizer, &token, errInfo, allocator);
      if (status != XCONF_SUCCESS) { return nullptr; }
      XCONFContext_state_action(context, state, &token, allocator);
    } else if (act->action == XCONF_action_reduce) {
      fn_xconf_reduce *func = XCONF_PRODUCTS[act->offset];
      token.type = act->type;
      token.value = func(args, context, errInfo, allocator);
      if (!token.value) { return nullptr; }
      state = parseJumpState(XCONF_state_LEFT_BRACKET_LEFT_SQUARE_BRACKET, act->type);
      if (state == XCONF_BAD_STATE) { return nullptr; }
      argc -= act->count;
      args[argc] = token;
    } else {
      // never be touched
    }
  }
}
