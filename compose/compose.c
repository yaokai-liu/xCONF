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
#define fprint_f32(file, value)  fprintf(file, "%#e"    , value->val.F32)
#define fprint_f64(file, value)  fprintf(file, "%#lgL"  , value->val.F64)
#define fprint_f128(file, value)  fprintf(file, "%#LgLL"  , value->val.F128)

static void fprint_i128(FILE *file, const Value *value);
static void fprint_u128(FILE *file, const Value *value);
static void fprint_text(FILE *file, const Value *value, const XCONFContext *context);

uint32_t writeValue(FILE *file, const REFER(Value) v_value, REFER(const Value) v_parent,
                    const bool paired, const uint32_t indent, XCONFContext *context) {
  const Value *value = Array_virt2real(context->value_array, v_value);
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
      return paired ? writeCompactPair(file, v_value, v_parent, indent, context)
                    : writeList(file, value->val.LIST, v_value, false, indent, context);
    }
    case XCONF_VAL_OBJECT: {
      return paired ? writeCompactPair(file, v_value, v_parent, indent, context)
                    : writeObject(file, value->val.OBJECT, v_value, false, indent, context);
    }
    case XCONF_VAL_LINKED: {
      fputc('@', file);
      const Value *refer = Array_virt2real(context->value_array, value->val.REFER);
      return writePath(file, refer, context);
    }
    default: {
      return XCONF_ERROR_WRITE_UNSUPPORTED_VALUE;
    }
  }
}

uint32_t writeList(FILE *file, const List *list, REFER(const Value) v_current, bool pathed, const uint32_t indent,
                   XCONFContext *context) {
  uint32_t n_values = Array_length(list);
  if (n_values == 0) { fputs("[]", file); return XCONF_SUCCESS; }
  fputs("[\n", file);
  if (pathed) {
    const REFER(Value) *v_values = Array_first_real(list);
    for (uint32_t i = 0; i < n_values; i++) {
      uint32_t result = writeCompactPair(file, v_values[i], v_current, indent, context);
      if (result != XCONF_SUCCESS) { return result; }
      if (i < n_values - 1) { fputs(",\n", file); } else { fputs("\n", file); }
    }
  } else {
    REFER(Value) *v_values = Array_first_real(list);
    for (uint32_t i = 0; i < n_values; i++) {
      for (uint32_t j = 0; j < indent + 2; j++) { fprintf(file, "  "); }
      uint32_t result = writeValue(file, v_values[i], v_current, false, indent + 2, context);
      if (result != XCONF_SUCCESS) { return result; }
      if (i < n_values - 1) { fputs(",\n", file); } else { fputs("\n", file); }
    }
    for (uint32_t j = 0; j < indent; j++) { fprintf(file, "  "); }
  }

  fputs("]", file);
  return XCONF_SUCCESS;
}

uint32_t writeObject(FILE *file, Object *object, REFER(const Value) v_current,
                     const bool pathed, const uint32_t indent, XCONFContext *context) {
    const uint32_t n_pairs = Dict_count(object);
    if (n_pairs == 0) { fputs("{}", file); return XCONF_SUCCESS; }
    fputs("{\n", file);

  if (pathed) {
    const REFER(Value) *v_values = Dict_elements(object);
    for (uint32_t i = 0; i < n_pairs; i++) {
      uint32_t result = writeCompactPair(file, v_values[i], v_current, indent, context);
      if (result != XCONF_SUCCESS) { return result; }
      if (i < n_pairs - 1) { fputs(",\n", file); } else { fputs("\n", file); }
    }
  } else {
    const REFER(char_t) *v_keys = Dict_keys(object);
    const REFER(Value) *v_values = Dict_elements(object);
    for (uint32_t i = 0; i < n_pairs; i++) {
      char *key = Array_virt2real(context->key_array, v_keys[i]);
      for (uint32_t j = 0; j < indent + 2; j++) { fprintf(file, "  "); }
      fprintf(file, "%s: ", key);
      uint32_t result = writeValue(file, v_values[i], v_current, false, indent + 2, context);
      if (result != XCONF_SUCCESS) { return result; }
      if (i < n_pairs - 1) { fputs(",\n", file); } else { fputs("\n", file); }
    }
    for (uint32_t j = 0; j < indent; j++) { fprintf(file, "  "); }

  }
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

inline void fprint_text(FILE *file, const Value *value, const XCONFContext *context) {
  const char *text = Array_virt2real(context->text_array, value->val.TEXT->content);
  fputc('"', file);
  for (uint32_t i = 0; i < value->val.TEXT->size; i++) {
    if (text[i] == '"') { fputc('\\', file); }
    fputc(text[i], file);
  }
  fputc('"', file);
}

uint32_t writePath(FILE * file, const Value * value, XCONFContext * context) {
  const Value *parent = Array_virt2real(context->value_array, value->path.parent);
  if (parent) {
    writePath(file, parent, context);
    if (parent->type == XCONF_VAL_OBJECT) {
      fputc('.', file);
      const char *key = Array_virt2real(context->key_array, value->path.key);
      fputs(key, file);
    } else if (parent->type == XCONF_VAL_LIST) {
      const uint32_t index = (uint32_t) (uint64_t) value->path.key;
      fprintf(file, "[%u]", index);
    } else { return XCONF_ERROR_CONFLICT_KEY_TYPE; }
  } else {
    const char *key = Array_virt2real(context->key_array, value->path.key);
    fputs(key, file);
  }
  return XCONF_SUCCESS;
}

uint32_t writeCompactPair(FILE *file, REFER(const Value) v_current, const REFER(Value) v_parent, const uint32_t indent, XCONFContext *context) {
  const Value *current = Array_virt2real(context->value_array, v_current);
  if (current->type == XCONF_VAL_OBJECT) {
    const uint32_t n_values = Dict_count(current->val.OBJECT);
    const REFER(Value) *v_values = Dict_elements(current->val.OBJECT);
    for (uint32_t i = 0; i < n_values; i++) {
      uint32_t result = writeCompactPair(file, v_values[i], v_current, indent, context);
      if (result != XCONF_SUCCESS) { return result; }
      if (i < n_values - 1) { fputs(",\n", file); }
    }
  } else if (current->type == XCONF_VAL_LIST) {
    const uint32_t n_values = Array_length(current->val.LIST);
    const REFER(Value) *v_values = Array_first_real(current->val.LIST);
    for (uint32_t i = 0; i < n_values; i++) {
      uint32_t result = writeCompactPair(file, v_values[i], v_current, indent, context);
      if (result != XCONF_SUCCESS) { return result; }
      if (i < n_values - 1) { fputs(",\n", file); }
    }
  } else {
    for (uint32_t j = 0; j < indent; j++) { fprintf(file, "  "); }
    writePath(file, current, context);
    fputs(" = ", file);
    uint32_t result = writeValue(file, v_current, v_parent, true, indent, context);
    if (result != XCONF_SUCCESS) { return result; }
  }
  return XCONF_SUCCESS;
}
