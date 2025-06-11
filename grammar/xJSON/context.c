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
 * Filename: context.c
 * Creator: Yaokai Liu
 * Create Date: 2025-06-07
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include "context.h"
#include "generated/xJSON/action-table.gen.h"

void XJSONContext_state_action(XJSONContext *context, uint32_t state, Token *, const Allocator *) {
  switch (state) {
    case XJSON_state_LEFT_BRACKET:
    case XJSON_state_LEFT_BRACKET_Path_ASSIGN_LEFT_BRACKET:
    case XJSON_state_LEFT_BRACKET_Path_ASSIGN_LEFT_SQUARE_BRACKET_LEFT_BRACKET: {
      Stack_push(context->obj_stack, &context->object, sizeof(Object *));
      context->object = context->allocator->calloc(1, sizeof(Object));
      break;
    }
    case XJSON_state_Object:
    case XJSON_state_LEFT_BRACKET_Path_ASSIGN_Object:
    case XJSON_state_LEFT_BRACKET_Path_ASSIGN_LEFT_SQUARE_BRACKET_Object: {
      Stack_pop(context->obj_stack, &context->object, sizeof(Object *));
      break;
    }
    default:{}
  }
}

inline REFER(char_t) XJSONContent_new_text_content(XJSONContext *context, const char_t *text_content, uint32_t size) {
  Array_append(context->text_array, "\0", 1);
  REFER(char_t) v_content = Array_last_virt(context->text_array) + 1;
  Array_append(context->text_array, text_content, size);
  return v_content;
}
