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
 * Filename: parse.c
 * Creator: Yaokai Liu
 * Create Date: 2025-06-07
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include "parse.h"
#include "action.h"
#include "generated/xCONF/action-table.gen.h"
#include "generated/xCONF/rules.gen.h"

#define MAX_ARGC       16


static Object *failed_to_get_next_state(Stack *state_stack, Stack *token_stack,
                                 Token *token, const Allocator *allocator);
static Object *failed_to_produce(Stack *state_stack, Stack *token_stack,
                          Token *, uint32_t, const Allocator *allocator);
static Object *clean_parse_stack(Stack *state_stack, Stack *token_stack, const Allocator *allocator);

Object *parse(XCONFTokenizer *tokenizer, XCONFContext *context, ErrInfo *errInfo, const Allocator *allocator) {
  Token token = {}, result = {};
  Token args[MAX_ARGC] = {};
  uint32_t state = XCONF_state_;
  Stack *state_stack = Stack_new(allocator);
  Stack *token_stack = Stack_new(allocator);
  context->path_action = XCONF_PATH_ACTION_BUILD;
  Stack_push(state_stack, &state, sizeof(int32_t));
  uint32_t status = XCONFTokenizer_next(tokenizer, &token, errInfo, allocator);
  if (status != XCONF_SUCCESS) { return nullptr; }
  while (true) {
    const struct grammar_action *act = getParseAction(state, token.type);
    if (!act) {
      errInfo->code = XCONF_ERROR_UNEXPECTED_TOKEN;
      errInfo->start = token.start;
      errInfo->end = token.end;
      errInfo->token = token.type;
      errInfo->info = state;
      return clean_parse_stack(state_stack, token_stack, allocator);
    }
    if (act->action == XCONF_action_stack) {
      state = act->offset;
      Stack_push(token_stack, &token, sizeof(Token));
      Stack_push(state_stack, &state, sizeof(uint32_t));
      status = XCONFTokenizer_next(tokenizer, &token, errInfo, allocator);
      if (status != XCONF_SUCCESS) {
        return clean_parse_stack(state_stack, token_stack, allocator);
      }
      XCONFContext_state_action(context, state, &token, allocator);
    } else if (act->action == XCONF_action_reduce) {
      Stack_pop(token_stack, args, act->count * sizeof(Token));
      Stack_pop(state_stack, nullptr, act->count * sizeof(uint32_t));
      Stack_top(state_stack, (uint32_t *) &state, sizeof(uint32_t));
      fn_xconf_reduce *func = XCONF_PRODUCTS[act->offset];
      result.type = act->type;
      result.start.offset = args[0].start.offset;
      result.start.lineno = args[0].start.lineno;
      result.start.column = args[0].start.column;
      result.end.offset = args[act->count - 1].end.offset;
      result.end.lineno = args[act->count - 1].end.lineno;
      result.end.column = args[act->count - 1].end.column;
      result.length = result.end.offset - result.start.offset;
      result.value = func(args, context, errInfo, allocator);
      if (!result.value) {
        return failed_to_produce(state_stack, token_stack, args, act->count, allocator);
      }
      state = parseJumpState(state, act->type);
      if (state == XCONF_BAD_STATE) {
        errInfo->code = XCONF_ERROR_UNEXPECTED_TOKEN;
        errInfo->start = result.start;
        errInfo->end = result.end;
        errInfo->token = result.type;
        errInfo->info = state;
        return failed_to_get_next_state(state_stack, token_stack, &token, allocator);
      }
      Stack_push(token_stack, &result, sizeof(Token));
      Stack_push(state_stack, &state, sizeof(uint32_t));
      if (act->offset == XCONF_RULE_Object_EXT) { break; }
      XCONFContext_state_action(context, state, &token, allocator);
    } else {
      // never be touched
    }
  }
  Stack_clear(token_stack);
  Stack_clear(state_stack);
  allocator->free(token_stack);
  allocator->free(state_stack);

  XCONFContext_enter(context, result.value);
  XCONFContext_buildObjectRefer(context, result.value, errInfo);
  XCONFContext_exit(context);
  if (errInfo->code != XCONF_SUCCESS) { return nullptr; }

  return result.value;
}

Object *failed_to_get_next_state(
    Stack *state_stack, Stack *token_stack, Token *token, const Allocator *allocator
) {
  int32_t state = 0;
  Stack_top(state_stack, (&state), sizeof(int32_t));
  releaseToken(token, allocator);
  allocator->free(token);
  return clean_parse_stack(state_stack, token_stack, allocator);
}

Object *failed_to_produce(
    Stack *state_stack, Stack *token_stack, Token args[], uint32_t argc, const Allocator *allocator
) {
  for (uint32_t i = 0; i < argc; i++) {
    releaseToken(&args[i], allocator);
  }
  return clean_parse_stack(state_stack, token_stack, allocator);
}

Object *clean_parse_stack(Stack *state_stack, Stack *token_stack, const Allocator *allocator) {
  Token token = {};
  while (!Stack_empty(token_stack)) {
    Stack_pop(token_stack, &token, sizeof(Token));
    releaseToken(&token, allocator);
  }
  Stack_clear(token_stack);
  Stack_clear(state_stack);
  allocator->free(token_stack);
  allocator->free(state_stack);
  return nullptr;
}
