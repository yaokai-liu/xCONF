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
 * Filename: parse.c
 * Creator: Yaokai Liu
 * Create Date: 2025-09-04
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include "parse.h"
#include "action.h"
#include "generated/tokens.gen.h"
#include "generated/Path/action-table.gen.h"
#include "generated/Path/rules.gen.h"

#define MAX_ARGC       16

Path *parsePath(XCONFTokenizer *tokenizer, XCONFContext *context, ErrInfo *errInfo, const Allocator *allocator) {
  Token token = {}, result = {};
  uint32_t argc = 0;
  Token args[MAX_ARGC] = {};
  uint32_t state = XCONF_Path_state_;

  uint32_t status = XCONFTokenizer_next(tokenizer, &token, errInfo, allocator);
  if (status != XCONF_SUCCESS) { return nullptr; }
  while (true) {
    const struct grammar_action *act = getParsePathAction(state, token.type);
    if (!act) {
      return nullptr;
    }
    if (act->action == XCONF_action_stack) {
      state = act->offset;
      args[argc++] = token;
      status = XCONFTokenizer_next(tokenizer, &token, errInfo, allocator);
      if (status != XCONF_SUCCESS) { return nullptr; }
      XCONFContext_state_action(context, state, &token, allocator);
    } else if (act->action == XCONF_action_reduce) {
      fn_xconf_path_reduce *func = XCONF_PATH_PRODUCTS[act->offset];
      result.type = act->type;
      result.value = func(args, context, errInfo, allocator);
      if (!result.value) { return nullptr; }
      state = parsePathJumpState(XCONF_Path_state_, act->type);
      if (state == XCONF_Path_BAD_STATE) { return nullptr; }
      if (act->offset == XCONF_PATH_RULE_Path_EXT) { break; }
      argc -= act->count;
      args[0] = result;
    } else {
      // never be touched
    }
  }
  return result.value;
}
