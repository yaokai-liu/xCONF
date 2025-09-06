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

#define fprint_i32(file, value)  fprintf(file, "%d"     , value->val.I32)
#define fprint_u32(file, value)  fprintf(file, "%u"     , value->val.U32)
#define fprint_i64(file, value)  fprintf(file, "%ldL"   , value->val.I64)
#define fprint_u64(file, value)  fprintf(file, "%luLU"  , value->val.U64)
#define fprint_f32(file, value)  fprintf(file, "%.1f"   , value->val.F32)
#define fprint_f64(file, value)  fprintf(file, "%.1lfL" , value->val.F64)

static void fprint_i128(FILE *file, const Value *value);
static void fprint_u128(FILE *file, const Value *value);
static void fprint_f128(FILE *file, const Value *value);
static void fprint_text(FILE *file, const Value *value, const XCONFContext *context);

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
      fprint_text(file, value, context);
      return XCONF_SUCCESS;
    }
    case XCONF_VAL_I32: {
      fprint_i32(file, value);
      return XCONF_SUCCESS;
    }
    case XCONF_VAL_U32: {
      fprint_u32(file, value);
      return XCONF_SUCCESS;
    }
    case XCONF_VAL_I64: {
      fprint_i64(file, value);
      return XCONF_SUCCESS;
    }
    case XCONF_VAL_U64: {
      fprint_u64(file, value);
      return XCONF_SUCCESS;
    }
    case XCONF_VAL_I128: {
      fprint_i128(file, value);
      return XCONF_SUCCESS;
    }
    case XCONF_VAL_U128: {
      fprint_u128(file, value);
      return XCONF_SUCCESS;
    }
    case XCONF_VAL_F32: {
      fprint_f32(file, value);
      return XCONF_SUCCESS;
    }
    case XCONF_VAL_F64: {
      fprint_f64(file, value);
      return XCONF_SUCCESS;
    }
    case XCONF_VAL_F128: {
      fprint_f128(file, value);
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
  fputs("[\n", file);
  uint32_t n_values = Array_length(list);
  REFER(Value) *v_values = Array_first_real(list);
  for (uint32_t i = 0; i < n_values; i++) {
    Value *val = Array_virt2real(context->value_array, v_values[i]);
    for (uint32_t j = 0; j < indent + 1; j++) { fprintf(file, "  "); }
    uint32_t result = writeValue(file, val, indent + 1, context);
    if (result != XCONF_SUCCESS) { return result; }
    if (i < n_values - 1) { fputs(",\n", file); } else { fputs("\n", file); }
  }
  for (uint32_t j = 0; j < indent; j++) { fprintf(file, "  "); }
  fputs("]", file);
  return XCONF_SUCCESS;
}

uint32_t writeObject(FILE *file, Object *object, uint32_t indent, XCONFContext *context) {
  fputs("{\n", file);
  uint32_t n_pairs = Array_length(object->pairs);
  Pair *pairs = Array_first_real(object->pairs);
  for (uint32_t i = 0; i < n_pairs; i++) {
    char *key = Array_virt2real(context->key_array, pairs[i].key);
    for (uint32_t j = 0; j < indent + 1; j++) { fprintf(file, "  "); }
    fprintf(file, "%s: ", key);
    Value *val = Array_virt2real(context->value_array, pairs[i].value);
    uint32_t result = writeValue(file, val, indent + 1, context);
    if (result != XCONF_SUCCESS) { return result; }
    if (i < n_pairs - 1) { fputs(",\n", file); } else { fputs("\n", file); }
  }
  for (uint32_t j = 0; j < indent; j++) { fprintf(file, "  "); }
  fputs("}", file);
  return XCONF_SUCCESS;
}

inline void fprint_i128(FILE *file, const Value *value) {
  if (value->val.I128 < 0) { fputc('-', file); }
  int128_t val = value->val.I128 < 0 ? - value->val.I128 : value->val.I128;
  char digits[64] = {}; uint32_t count = 0;
  while (val > 0) {
    digits[count++] = val % 10 + '0';
    val /= 10;
  }
  for (uint32_t i = count - 1; i < count; i--) { fputc(digits[i], file); }
  fputs("LL", file);
}

inline void fprint_u128(FILE *file, const Value *value) {
  uint128_t val = value->val.U128;
  char digits[64] = {}; uint32_t count = 0;
  while (val > 0) {
    digits[count++] = val % 10 + '0';
    val /= 10;
  }
  for (uint32_t i = count - 1; i < count; i--) { fputc(digits[i], file); }
  fputs("LLU", file);
}

inline void fprint_f128(FILE *file, const Value *value) {
  // TODO: real format print function for 128bit float number
  fputs("float(", file);
  uint128_t val = value->val.U128;
  char digits[64] = {}; uint32_t count = 0;
  while (val > 0) {
    digits[count++] = val % 10 + '0';
    val /= 10;
  }
  for (uint32_t i = count - 1; i < count; i--) { fputc(digits[i], file); }
  fputs("LLU)", file);
}

inline void fprint_text(FILE *file, const Value *value, const XCONFContext *context) {
  const char *text = Array_virt2real(context->text_array, value->val.TEXT->content);
  putc('"', file);
  for (uint32_t i = 0; i < value->val.TEXT->size; i++) {
    if (text[i] == '"') { putc('\\', file); }
    putc(text[i], file);
  }
  putc('"', file);
}
