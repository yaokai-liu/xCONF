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
 * Module Name: compose
 * Filename: compose.c
 * Creator: Yaokai Liu
 * Create Date: 2025-06-12
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include "compose.h"
#include "xCONF/context.h"
#include <string.h>
#include <stdio.h>

uint32_t writeValue(FILE *file, Value *value, uint32_t indent, XCONFContext *context) {
  switch (value->type) {
    case XCONF_VAL_NULL: {
      fputs("NULL", file);
      return XCONF_SUCCESS;
    }
    case XCONF_VAL_BOOLEAN: {
      fputs(value->val.BOOLEAN ? "TRUE" : "FALSE", file);
      return XCONF_SUCCESS;
    }
    case XCONF_VAL_TEXT: {
      putc('"', file);
      for (uint32_t i = 0; i < value->val.TEXT->size; i++) {
        if (value->val.TEXT->content[i] == '"') { putc('\\', file); }
        putc(value->val.TEXT->content[i], file);
      }
      putc('"', file);
      return XCONF_SUCCESS;
    }
    case XCONF_VAL_I32: {
      fprintf(file, "%d", value->val.I32);
      return XCONF_SUCCESS;
    }
    case XCONF_VAL_U32: {
      fprintf(file, "%uU", value->val.U32);
      return XCONF_SUCCESS;
    }
    case XCONF_VAL_I64: {
      fprintf(file, "%ldL", value->val.I64);
      return XCONF_SUCCESS;
    }
    case XCONF_VAL_U64: {
      fprintf(file, "%luLU", value->val.I64);
      return XCONF_SUCCESS;
    }
    case XCONF_VAL_I128: {
      fprintf(file, "%lldLL", value->val.I128);
      return XCONF_SUCCESS;
    }
    case XCONF_VAL_U128: {
      fprintf(file, "%lluLLU", value->val.U128);
      return XCONF_SUCCESS;
    }
    case XCONF_VAL_F32: {
      fprintf(file, "%f", value->val.F32);
      return XCONF_SUCCESS;
    }
    case XCONF_VAL_F64: {
      fprintf(file, "%lfL", value->val.F64);
      return XCONF_SUCCESS;
    }
    case XCONF_VAL_F128: {
      fprintf(file, "%llfLL", value->val.F128);
      return XCONF_SUCCESS;
    }
    case XCONF_VAL_LIST: {
      return writeList(file, value->val.LIST, indent, context);
    }
    case XCONF_VAL_OBJECT: {
      return writeObject(file, value->val.OBJECT, indent, context);
    }
    default: {
      return XCONF_ERROR_DUMP_UNSUPPORTED_VALUE;
    }
  }
}

uint32_t writeList(FILE *file, List *list, uint32_t indent, XCONFContext *context) {
  fprintf(file, "[\n");
  uint32_t n_values = Array_length(list);
  REFER(Value) *v_values = Array_first_real(list);
  for (uint32_t i = 0; i < n_values; i++) {
    Value *val = Array_virt2real(context->value_array, v_values[i]);
    for (uint32_t j = 0; j < indent + 1; j++) { fprintf(file, "  "); }
    uint32_t result = writeValue(file, val, indent + 1, context);
    if (result != XCONF_SUCCESS) { return result; }
    if (i < n_values - 1) { fprintf(file, ",\n"); }
  }
  for (uint32_t j = 0; j < indent; j++) { fprintf(file, "  "); }
  fprintf(file, "]\n");
  return XCONF_SUCCESS;
}

uint32_t writeObject(FILE *file, Object *object, uint32_t indent, XCONFContext *context) {
  fprintf(file, "{\n");
  uint32_t n_keys = Array_length(object->keys);
  REFER(Value) *keys = Array_first_real(object->keys);
  for (uint32_t i = 0; i < n_keys; i++) {
    char *key = Array_virt2real(context->key_array, keys[i]);
    for (uint32_t j = 0; j < indent + 1; j++) { fprintf(file, "  "); }
    fprintf(file, "%s = ", key);
    Value *v_val = AVLTree_get(object->mapping, (uint64_t) keys[i]);
    Value *val = Array_virt2real(context->value_array, v_val);
    uint32_t result = writeValue(file, val, indent + 1, context);
    if (result != XCONF_SUCCESS) { return result; }
    if (i < n_keys - 1) { fprintf(file, ",\n"); }
  }
  for (uint32_t j = 0; j < indent; j++) { fprintf(file, "  "); }
  fprintf(file, "}\n");
  return XCONF_SUCCESS;
}
