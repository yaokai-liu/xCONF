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
 * Filename: tokenize.c
 * Creator: Yaokai Liu
 * Create Date: 2025-05-30
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include "tokenize.h"
#include "xCONF/target.h"
#include "string_t.h"
#include "token.h"
#include "generated/tokens.gen.h"
#include "xCONF/xCONF.h"
#include <tgmath.h>

#define lenof(str_literal) ((sizeof str_literal) - 1)
#define max(a, b)          ((a) > (b) ? (a) : (b))
#define min(a, b)          ((a) < (b) ? (a) : (b))

static uint32_t t_KEY(const char_t *input, Terminal *result, const Allocator *allocator);
static uint32_t t_NUMBER(const char_t *input, Terminal *result, bool negative ,
                         uint32_t adic, const Allocator *allocator);
static uint32_t t_INT_DIGITS_adic16(const char_t *input, uint32_t *effective_length, uint256_t *value);
static uint32_t t_INT_DIGITS_adic10(const char_t *input, uint32_t *effective_length, uint256_t *value);
static uint32_t t_INT_DIGITS_adic8 (const char_t *input, uint32_t *effective_length, uint256_t *value);
static uint32_t t_INT_DIGITS_adic2 (const char_t *input, uint32_t *effective_length, uint256_t *value);
static uint32_t t_FRAC_DIGITS_adic16(const char_t *input, uint32_t *effective_length, uint256_t *value);
static uint32_t t_FRAC_DIGITS_adic10(const char_t *input, uint32_t *effective_length, uint256_t *value);
static uint32_t t_FRAC_DIGITS_adic8 (const char_t *input, uint32_t *effective_length, uint256_t *value);
static uint32_t t_FRAC_DIGITS_adic2 (const char_t *input, uint32_t *effective_length, uint256_t *value);

static uint32_t try_keyword_FALSE(const char_t *input, uint32_t offs, Terminal *result, const Allocator *allocator);
static uint32_t try_keyword_NULL(const char_t *input, uint32_t offs, Terminal *result, const Allocator *allocator);
static uint32_t try_keyword_TRUE(const char_t *input, uint32_t offs, Terminal *result, const Allocator *allocator);
static uint32_t try_keyword_false(const char_t *input, uint32_t offs, Terminal *result, const Allocator *allocator);
static uint32_t try_keyword_null(const char_t *input, uint32_t offs, Terminal *result, const Allocator *allocator);
static uint32_t try_keyword_true(const char_t *input, uint32_t offs, Terminal *result, const Allocator *allocator);

static uint32_t try_keyword_inf(const char_t *input, uint32_t offs, Terminal *result, const Allocator *allocator);
static uint32_t try_keyword_INF(const char_t *input, uint32_t offs, Terminal *result, const Allocator *allocator);
static uint32_t try_keyword_nan(const char_t *input, uint32_t offs, Terminal *result, const Allocator *allocator);
static uint32_t try_keyword_NAN(const char_t *input, uint32_t offs, Terminal *result, const Allocator *allocator);

static uint32_t tokenize_letter_n(const char_t * input, Terminal * result, const Allocator * allocator);
static uint32_t tokenize_letter_N(const char_t * input, Terminal * result, const Allocator * allocator);

static uint32_t tokenize_single_symbol(const char_t *input, Terminal *result, const Allocator *allocator);
static uint32_t tokenize_number(const char_t *input, Terminal *result, const Allocator *allocator);
static uint32_t tokenize_text(const char_t *input, uint32_t n_pred,
                              const char_t *succ, uint32_t n_succ,
                              Terminal *result, const Allocator *allocator);

static uint32_t try_pass_comment(const char *input, uint32_t *lineno, uint32_t *column);

#define isSign(pText)             ((*pText == '-') || (*pText == '+'))
#define isKeyHeader(pText)        (startswithLetter(pText) || (*pText == '_'))
#define isKeyChar(pText)          (startswithLetter(pText) || isDecDigital(pText) || (*pText == '_') || (*pText == '-'))

#define startswithLetter(pText)   (('a' <= (pText)[0] && (pText)[0] <= 'z') || ('A' <= (pText)[0] && (pText)[0] <= 'Z'))
#define isHexLetter(pText)        (('a' <= (pText)[0] && (pText)[0] <= 'f') || ('A' <= (pText)[0] && (pText)[0] <= 'F'))

#define isBinDigital(pText)       ('0' == (pText)[0] || (pText)[0] == '1')
#define isOctDigital(pText)       ('0' <= (pText)[0] && (pText)[0] <= '7')
#define isDecDigital(pText)       ('0' <= (pText)[0] && (pText)[0] <= '9')
#define isHexDigital(pText)       (isDecDigital(pText) || isHexLetter(pText))


// [a-fA-F0-9]+
inline uint32_t t_INT_DIGITS_adic16(const char_t *const input, uint32_t *effective_length, uint256_t *value) {
  const char_t *pZero = input;
  if (!isHexDigital(pZero)) { return 0; }
  while ('0' == *pZero) { pZero++; }
  const char_t *pText = pZero;
  uint32_t n_separator = 0;
  while (*pText) {
    if ('0' <= *pText && *pText <= '9') {
      *value = (*value << 4) + (*pText - '0');
    } else if ('a' <= *pText && *pText <= 'f') {
      *value = (*value << 4) + (*pText - 'a' + 0xa);
    } else if ('A' <= *pText && *pText <= 'F') {
      *value = (*value << 4) + (*pText - 'A' + 0xA);
    } else if (*pText == '\'') {
      if (!isHexDigital(pText + 1)) { break; }
      n_separator ++;
    } else { break; }
    pText ++;
  }
  if (effective_length) { *effective_length = pText - pZero - n_separator; }
  return pText - input;
}
// [0-9]+
inline uint32_t t_INT_DIGITS_adic10(const char_t *const input, uint32_t *effective_length, uint256_t *value) {
  const char_t *pZero = input;
  if (!isDecDigital(pZero)) { return 0; }
  while ('0' == *pZero) { pZero++; }
  const char_t *pText = pZero;
  uint32_t n_separator = 0;
  while (*pText) {
    if (isDecDigital(pText)) {
      *value = (*value) * 10 + (*pText - '0');
    } else if (*pText == '\'') {
      if (!isDecDigital(pText + 1)) { break; }
      n_separator ++;
    } else { break; }
    pText ++;
  }
  if (effective_length) { *effective_length = pText - pZero - n_separator; }
  return pText - input;
}
// [0-7]+
inline uint32_t t_INT_DIGITS_adic8(const char_t *const input, uint32_t *effective_length, uint256_t *value) {
  const char_t *pZero = input;
  if (!isOctDigital(pZero)) { return 0; }
  while ('0' == *pZero) { pZero++; }
  const char_t *pText = pZero;
  uint32_t n_separator = 0;
  while (*pText) {
    if (isOctDigital(pText)) {
      *value = (*value << 3) + (*pText - '0');
    } else if (*pText == '\'') {
      if (!isOctDigital(pText + 1)) { break; }
      n_separator ++;
    } else { break; }
    pText ++;
  }
  if (effective_length) { *effective_length = pText - pZero - n_separator; }
  return pText - input;
}
// [01]+
inline uint32_t t_INT_DIGITS_adic2(const char_t *const input, uint32_t *effective_length, uint256_t *value) {
  const char_t *pZero = input;
  if (!isBinDigital(pZero)) { return 0; }
  while ('0' == *pZero) { pZero++; }
  const char_t *pText = pZero;
  uint32_t n_separator = 0;
  while (*pText) {
    if (isOctDigital(pText)) {
      *value = (*value << 1) + (*pText - '0');
    } else if (*pText == '\'') {
      if (!isBinDigital(pText + 1)) { break; }
      n_separator ++;
    } else { break; }
    pText ++;
  }
  if (effective_length) { *effective_length = pText - pZero - n_separator; }
  return pText - input;
}
// (?=\.)[a-fA-F0-9]+
inline uint32_t t_FRAC_DIGITS_adic16(const char_t *const input, uint32_t *effective_length, uint256_t *value) {
  const char_t *pText = input;
  if (!isHexDigital(pText)) { return 0; }
  uint32_t trialing_zeros = 0;
  uint32_t n_separator = 0;
  while (*pText) {
    if (*pText == '\'') {
      if (!isHexDigital(pText + 1)) { break; }
      n_separator ++; pText ++; continue;
    }
    if ('0' == *pText) { trialing_zeros ++; continue; }
    uint32_t shift_count = 4 * (1 + trialing_zeros);
    if ('1' <= *pText && *pText <= '9') {
      *value = (*value << shift_count) + (*pText - '0');
    } else if ('a' <= *pText && *pText <= 'f') {
      *value = (*value << shift_count) + (*pText - 'a' + 0xa);
    } else if ('A' <= *pText && *pText <= 'F') {
      *value = (*value << shift_count) + (*pText - 'A' + 0xA);
    } else { break; }
    trialing_zeros = 0;
    pText ++;
  }
  uint32_t length = pText - input;
  if (effective_length) { *effective_length = length - trialing_zeros - n_separator; }
  return length;
}
// (?=\.)[0-9]+
inline uint32_t t_FRAC_DIGITS_adic10(const char_t *const input, uint32_t *effective_length, uint256_t *value) {
  const char_t *pText = input;
  if (!isDecDigital(pText)) { return 0; }
  uint32_t trialing_zeros = 0;
  uint32_t n_separator = 0;
  while (*pText) {
    if (*pText == '\'') {
      if (!isDecDigital(pText + 1)) { break; }
      n_separator ++; pText ++; continue;
    }
    if ('0' == *pText) { trialing_zeros ++; pText++; continue; }
    uint32_t rate = 10;
    for (uint32_t i = 0; i < trialing_zeros; i++) { rate *= 10; }
    if ('1' <= *pText && *pText <= '9') {
      *value = ((*value) * rate) + (*pText - '0');
    } else { break; }
    trialing_zeros = 0;
    pText ++;
  }
  uint32_t length = pText - input;
  if (effective_length) { *effective_length = length - trialing_zeros - n_separator; }
  return length;
}
// (?=\.)[0-7]+
inline uint32_t t_FRAC_DIGITS_adic8(const char_t *const input, uint32_t *effective_length, uint256_t *value) {
  const char_t *pText = input;
  if (!isOctDigital(pText)) { return 0; }
  uint32_t trialing_zeros = 0;
  uint32_t n_separator = 0;
  while (*pText) {
    if (*pText == '\'') {
      if (!isOctDigital(pText + 1)) { break; }
      n_separator ++; pText ++; continue;
    }
    if ('0' == *pText) { trialing_zeros ++; continue; }
    uint32_t shift_count = 3 * (1 + trialing_zeros);
    if ('1' <= *pText && *pText <= '7') {
      *value = ((*value) << shift_count) + (*pText - '0');
    } else { break; }
    trialing_zeros = 0;
    pText ++;
  }
  uint32_t length = pText - input;
  if (effective_length) { *effective_length = length - trialing_zeros - n_separator; }
  return length;
}
// (?=\.)[01]+
inline uint32_t t_FRAC_DIGITS_adic2(const char_t *const input, uint32_t *effective_length, uint256_t *value) {
  const char_t *pText = input;
  if (!isBinDigital(pText)) { return 0; }
  uint32_t trialing_zeros = 0;
  uint32_t n_separator = 0;
  while (*pText) {
    if (*pText == '\'') {
      if (!isBinDigital(pText + 1)) { break; }
      n_separator ++; pText ++; continue;
    }
    if ('0' == *pText) { trialing_zeros ++; continue; }
    uint32_t shift_count = 1 + trialing_zeros;
    if ('1' == *pText) {
      *value = ((*value) << shift_count) + (*pText - '0');
    } else { break; }
    trialing_zeros = 0;
    pText ++;
  }
  uint32_t length = pText - input;
  if (effective_length) { *effective_length = length - trialing_zeros - n_separator; }
  return length;
}

#define ADIC_TYPE_16   0
#define ADIC_TYPE_10   1
#define ADIC_TYPE_8    2
#define ADIC_TYPE_2    3
#define INT_DIGITAL_FUNC      0
#define FRAC_DIGITAL_FUNC     1

typedef uint32_t tokenize_t(const char_t *, uint32_t *, uint256_t *);
static tokenize_t *const DIGITAL_FUNC_TOOLS[4][2] = {
    [ADIC_TYPE_16] = { [INT_DIGITAL_FUNC] = t_INT_DIGITS_adic16, [FRAC_DIGITAL_FUNC] = t_FRAC_DIGITS_adic16},
    [ADIC_TYPE_10] = { [INT_DIGITAL_FUNC] = t_INT_DIGITS_adic10, [FRAC_DIGITAL_FUNC] = t_FRAC_DIGITS_adic10},
    [ADIC_TYPE_8 ] = { [INT_DIGITAL_FUNC] = t_INT_DIGITS_adic8 , [FRAC_DIGITAL_FUNC] = t_FRAC_DIGITS_adic8 },
    [ADIC_TYPE_2 ] = { [INT_DIGITAL_FUNC] = t_INT_DIGITS_adic2 , [FRAC_DIGITAL_FUNC] = t_FRAC_DIGITS_adic2 },
};
static const uint32_t ADIC_BASE[] = {
    [ADIC_TYPE_16] = 16,
    [ADIC_TYPE_10] = 10,
    [ADIC_TYPE_8 ] = 8,
    [ADIC_TYPE_2 ] = 2,
};

/*
 * if adic
 * is ADIC_TYPE_16:    [a-fA-F0-9]+((\.[a-fA-F0-9]+([pP][+-]?[0-9]+)?)?[lL]{0,2})|[lL]{0,2}[uU]?)
 * is ADIC_TYPE_10:    [01]+((\.[01]+([pPeE][+-]?[0-9]+)?)?[lL]{0,2})|[lL]{0,2}[uU]?)
 * is ADIC_TYPE_8:     [0-7]+((\.[0-7]+([pPeE][+-]?[0-9]+)?)?[lL]{0,2}|[lL]{0,2}[uU]?)
 * is ADIC_TYPE_2:     [0-9]+((\.[0-9]+([pPeE][+-]?[0-9]+)?)?[lL]{0,2}|[lL]{0,2}[uU]?)
 */
inline uint32_t t_NUMBER(const char_t *const input, Terminal *const result,
                         const bool negative , const uint32_t adic, const Allocator *const allocator) {
  const char_t *pText = input;

  uint32_t size = 4;
  uint256_t integer = 0;
  uint256_t exponent = 0;
  uint32_t int_eff_length = 0;
  uint32_t frac_eff_length = 0;
  bool exp_negative = false;
  enum XCONF_VALUE_CATEGORY_ENUM type = XCONF_VAL_CAT_INT;

  uint32_t length = DIGITAL_FUNC_TOOLS[adic][INT_DIGITAL_FUNC](pText, &int_eff_length, &integer);
  if (!length) { return 0; } else { pText += length; }
  if (*pText == '.') {
    type = XCONF_VAL_CAT_FLOAT; size = 4; pText ++;
    length = DIGITAL_FUNC_TOOLS[adic][FRAC_DIGITAL_FUNC](pText, &frac_eff_length, &integer);
    if (!length) { return 0; }
    pText += length;
  }
  if (*pText == 'p' || *pText == 'P' || *pText == 'e' || *pText == 'E') {
    pText ++;
    exp_negative = (*pText == '-');
    if (isSign(pText)) { pText++; }
    length = DIGITAL_FUNC_TOOLS[ADIC_TYPE_10][INT_DIGITAL_FUNC](pText, nullptr, &exponent);
    if (!length) { return 0; }
    pText += length;
  }
  exponent = exp_negative ? -exponent : exponent;
  exponent += int_eff_length;
  exponent -= frac_eff_length;
  if ((*pText == 'l') || (*pText == 'L')) { size *= 2; pText++; }
  if ((*pText == 'l') || (*pText == 'L')) { size *= 2; pText++; }
  size = min(size, 16);
  if ((*pText == 'u') || (*pText == 'U')) {
    if (type == XCONF_VAL_CAT_FLOAT) { return 0; }
    type = XCONF_VAL_CAT_UINT; pText++;
  }
  if (isKeyChar(pText) || *pText == '.') { return 0; }

  Value *value = allocator->calloc(1, sizeof(Value));
  value->size = size;
  // TODO:
  //  The value obtained in this way is not accurate enough.
  //  Please try to improve the algorithm.
  if (type == XCONF_VAL_CAT_FLOAT) {
    const uint32_t exponent_base = ADIC_BASE[adic];
    if (size == 4 ) {
      float32_t real = ((float32_t) (uint32_t) integer);
      float32_t exp = (float32_t) pow((float32_t) exponent_base, (int32_t) exponent - 1);
      value->val.F32 = (negative) ? -real * exp : real * exp;
      value->type = XCONF_VAL_F32;
    } else if (size == 8 ) {
      float64_t real = ((float64_t) (uint64_t) integer);
      float64_t exp = (float64_t) pow((float64_t) exponent_base, (int64_t) exponent - 1);
      value->val.F64 = (negative) ? -real * exp : real * exp;
      value->type = XCONF_VAL_F64;
    } else if (size == 16) {
      float128_t real = ((float128_t) (uint128_t) integer);
      float128_t exp = (float128_t) pow((float128_t) exponent_base, (int128_t) exponent - 1);
      value->val.F128 = (negative) ? -real * exp : real * exp;
      value->type = XCONF_VAL_F128;
    } else { return 0; }
  } else if (type == XCONF_VAL_CAT_UINT) {
    if (negative) { integer = -integer; }
    if (size == 4) { value->val.U32 = integer; value->type = XCONF_VAL_U32; }
    else if (size == 8) { value->val.U64 = integer; value->type = XCONF_VAL_U64; }
    else if (size == 16) { value->val.U128 = integer; value->type = XCONF_VAL_U128; }
    else { return 0; }
  } else {
    if (negative) { integer = -integer; }
    if (size == 4) { value->val.I32 = integer; value->type = XCONF_VAL_I32; }
    else if (size == 8) { value->val.I64 = integer; value->type = XCONF_VAL_I64; }
    else if (size == 16) { value->val.I128 = integer; value->type = XCONF_VAL_I128; }
    else { return 0; }
  }

  result->type = XCONF_TOKEN_NUMBER;
  result->length = pText - input;
  result->value = value;
  return result->length;
}

inline uint32_t t_KEY(const char_t * const input, Terminal * const result, const Allocator * const allocator) {
  const char_t *pText = input;
  if (isKeyHeader(pText)) {
    pText++;
  } else {
    result->length = pText - input;
    return 0;
  }
  while (true) {
    if (isDecDigital(pText) || startswithLetter(pText)) { pText++; continue; }
    // "_-" and "-_" are forbidden
    if (*pText == '_') {
      if (pText[1] == '-') { result->length = pText - input; return 0; } else { pText++; continue; }
    }
    if (*pText == '-') {
      if (pText[1] == '_') { result->length = pText - input; return 0; } else { pText++; continue; }
    }
    break;
  }

  // endswith '-' is forbidden
  if (*(pText - 1) == '-') { result->length = pText - input; return 0; }

  const uint32_t length = pText - input;
  result->type = XCONF_TOKEN_KEY;
  result->value = allocator->malloc((length + 1) * sizeof(char_t));
  allocator->memcpy(result->value, input, length);
  ((char_t *) result->value)[length] = '\0';
  result->length = pText - input;
  return result->length;
}

#define tokenize_adic_number(text_off, adic_type) do {            \
  uint32_t length = t_NUMBER(pText + (text_off), result,          \
                             negative, (adic_type), allocator);   \
  result->length += pText - input;                                \
  return length ? result->length + text_off : 0;                  \
} while(false)

#define try_special_number(text_off, pattern, _type, _size, _value) do {    \
  if (strcmp_o(pText, &(pattern[text_off])) == (lenof(pattern) - text_off)  \
       && !isKeyChar(pText + lenof(pattern))) {                             \
    Value *value = allocator->calloc(1, sizeof(Value));                     \
    value->type = _type;                                                    \
    value->size = _size;                                                    \
    value->val.F32 = negative ? -(_value) : (_value);                       \
    result->type = XCONF_TOKEN_NUMBER;                                      \
    result->value = value;                                                  \
    result->length = pText - input + lenof(pattern);                        \
    return result->length;                                                  \
  }                                                                         \
} while (false)

/*
 * [+-]?0[xX][a-fA-F0-9]+((\.[a-fA-F0-9]+([pP][+-]?[0-9]+)?)?[lL]{0,2})|[lL]{0,2}[uU]?) |
 * [+-]?0[bB][01]+((\.[01]+([pPeE][+-]?[0-9]+)?)?[lL]{0,2})|[lL]{0,2}[uU]?) |
 * [+-]?0[oO]?[0-7]+((\.[0-7]+([pPeE][+-]?[0-9]+)?)?[lL]{0,2}|[lL]{0,2}[uU]?) |
 * [+-]?[0-9]+((\.[0-9]+([pPeE][+-]?[0-9]+)?)?[lL]{0,2}|[lL]{0,2}[uU]?)
 */
inline uint32_t tokenize_number(const char_t * const input, Terminal * const result,
                                const Allocator * const allocator) {
  const char_t *pText = input;
  const bool negative = ('-' == *pText);
  if (isSign(pText)) {
    pText++;
    while (*pText == ' ' || *pText == '\t') { pText++; }
  }
  switch (*pText) {
    case 'i': {
      try_special_number(0, "inf", XCONF_VAL_F32, 4, INFINITY);
      return 0;
     }
    case 'I': {
      try_special_number(0, "INF", XCONF_VAL_F32, 4, INFINITY);
      return 0;
     }
    case 'n': {
      try_special_number(0, "nan", XCONF_VAL_F32, 4, NAN);
      return 0;
     }
    case 'N': {
      try_special_number(0, "NAN", XCONF_VAL_F32, 4, NAN);
      return 0;
    }
    case '0': {
      switch (pText[1]) {
        case 'x':
        case 'X': {
          tokenize_adic_number(2, ADIC_TYPE_16);
        }
        case 'o':
        case 'O': {
          tokenize_adic_number(2, ADIC_TYPE_8);
        }
        case 'b':
        case 'B': {
          tokenize_adic_number(2, ADIC_TYPE_2);
        }
        default: {}
      }
    }
    default: tokenize_adic_number(0, ADIC_TYPE_10);
  }
}

inline uint32_t try_keyword_inf(const char_t * const input, const uint32_t offs, Terminal * const result,
                                const Allocator * const allocator) {
  const char *pText = input;
  constexpr bool negative = false;
  try_special_number(offs, "inf", XCONF_VAL_F32, 4, INFINITY);
  return t_KEY(input - offs, result, allocator);
}

inline uint32_t try_keyword_INF(const char_t * const input, const uint32_t offs, Terminal * const result,
                                const Allocator * const allocator) {
  const char *pText = input;
  constexpr bool negative = false;
  try_special_number(offs, "INF", XCONF_VAL_F32, 4, INFINITY);
  return t_KEY(input - offs, result, allocator);
}

inline uint32_t try_keyword_nan(const char_t * const input, const uint32_t offs, Terminal * const result,
                                const Allocator * const allocator) {
  const char *pText = input;
  constexpr bool negative = false;
  try_special_number(1, "inf", XCONF_VAL_F32, 4, NAN);
  return t_KEY(input - offs, result, allocator);
}

inline uint32_t try_keyword_NAN(const char_t * const input, const uint32_t offs, Terminal * const result,
                                const Allocator * const allocator) {
  const char *pText = input;
  constexpr bool negative = false;
  try_special_number(offs, "NAN", XCONF_VAL_F32, 4, NAN);
  return t_KEY(input - offs, result, allocator);
}

inline uint32_t tokenize_letter_n(const char_t * const input, Terminal * const result,
                                  const Allocator * const allocator) {
  if (*input == 'u') { return try_keyword_null(input + 1, 2, result, allocator); }
  if (*input == 'a') { return try_keyword_nan(input + 1, 2, result, allocator); }
  return t_KEY(input - 1, result, allocator);
}

inline uint32_t tokenize_letter_N(const char_t * const input, Terminal * const result,
                                  const Allocator * const allocator) {
  if (*input == 'u') { return try_keyword_NULL(input + 1, 2, result, allocator); }
  if (*input == 'a' || *input == 'A') { return try_keyword_NAN(input + 1, 2, result, allocator); }
  return t_KEY(input - 1, result, allocator);
}

// ${pred}.*${succ}
inline uint32_t tokenize_text(const char_t *const input, const uint32_t n_pred,
                              const char_t *const succ, const uint32_t n_succ,
                              Terminal *const result, const Allocator *const allocator) {
  const char_t *pText = input;
  while (*pText) {
    if (*pText == '\\') {
      if (!*pText++) { return 0; }
      pText++;
    }
    uint32_t length = strcmp_o(pText, succ);
    if (length == n_succ) { break; }
    if (!pText[length]) { return 0; }
    pText ++;
  }

  uint32_t content_length = pText - input;
  const uint32_t cost_length = n_pred + content_length + n_succ;
  WrapperedText *text = allocator->calloc(1, sizeof(WrapperedText));
  text->content = allocator->malloc((content_length + 1) * sizeof(char_t));
  text->length = 0;
  for (uint32_t i = 0; i < content_length; i++, text->length++) {
    // TODO: support for escape operations
    if (input[i] == '\\') { i++; }
    text->content[text->length] = input[i];
  }
  text->content[text->length] = '\0';
  text->n_pred = n_pred;
  text->n_succ = n_succ;

  result->type = XCONF_TOKEN_TEXT;
  result->length = cost_length;
  result->value = text;

  return cost_length;
}

#define fn_try_keyword_val(_kw, _type, val)                                                 \
  uint32_t try_keyword_##_kw(const char_t * const input, uint32_t const offs,               \
                             Terminal * const result, const Allocator * const allocator) {  \
    const char_t pattern[] = string_t(#_kw);                                                \
    for (uint32_t i = offs; i < sizeof(pattern) - 1; i++) {                                 \
      if (input[i - offs] != pattern[i]) { goto __failed_kw_##_kw; }                        \
    }                                                                                       \
    const char_t * const tail = &input[sizeof(pattern) - 1 - offs];                         \
    if (isKeyChar(tail)) { goto __failed_kw_##_kw; }                                        \
    result->type = XCONF_TOKEN_##_type;                                                     \
    result->value = (void *) val;                                                           \
    result->length = lenof(#_kw);                                                           \
    return lenof(#_kw);                                                                     \
    __failed_kw_##_kw : return t_KEY(input - offs, result, allocator);                      \
  }

fn_try_keyword_val(false, BOOLEAN, 0)
fn_try_keyword_val(true, BOOLEAN, 1)
fn_try_keyword_val(null, NULL, 0)
fn_try_keyword_val(FALSE, BOOLEAN, 0)
fn_try_keyword_val(TRUE, BOOLEAN, 1)
fn_try_keyword_val(NULL, NULL, 0)

constexpr uint32_t TERMINAL_TYPE_LITERALS[] = {
  XCONF_TOKEN_DOT,
  XCONF_TOKEN_COMMA,
  XCONF_TOKEN_COLON,
  XCONF_TOKEN_ASSIGN,

  XCONF_TOKEN_LEFT_BRACKET,
  XCONF_TOKEN_RIGHT_BRACKET,
  XCONF_TOKEN_LEFT_SQUARE_BRACKET,
  XCONF_TOKEN_RIGHT_SQUARE_BRACKET,
};
uint32_t tokenize_single_symbol(const char_t * const input, Terminal * const result, const Allocator * const) {
  constexpr char_t SINGLE_LITERAL[] = ".,:={}[]";
  uint32_t length = stridx_o(*input, SINGLE_LITERAL);
  if (length < lenof(SINGLE_LITERAL)) {
    result->type = TERMINAL_TYPE_LITERALS[length];
    result->value = nullptr;
    result->length = 1;
    return 1;
  }
  return 0;
}

uint32_t single_tokenize(const char_t * const input, Terminal * const result,
                         const Allocator * const allocator) {
  if (!*input) {
    result->type = XCONF_TOKEN_TERMINATOR;
    result->value = nullptr;
    result->length = 0;
    return 0;
  }
  uint32_t length = 0;
  if (isDecDigital(input) || (*input == '+') || *input == '-') {
    length = tokenize_number(input, result, allocator);
    return length;
  }
  length = tokenize_single_symbol(input, result, allocator);
  if (length > 0) { return length; }
  switch (*input) {
    case 'i': { return try_keyword_inf(input + 1, 1, result, allocator); }
    case 'I': { return try_keyword_INF(input + 1, 1, result, allocator); }
    case 'f': { return try_keyword_false(input + 1, 1, result, allocator); }
    case 'F': { return try_keyword_FALSE(input + 1, 1, result, allocator); }
    case 't': { return try_keyword_true(input + 1, 1, result, allocator); }
    case 'T': { return try_keyword_TRUE(input + 1, 1, result, allocator); }
    case 'n': { return tokenize_letter_n(input + 1, result, allocator); }
    case 'N': { return tokenize_letter_N(input + 1, result, allocator); }
    case '"': { return tokenize_text(input + 1, 1, "\"", 1, result, allocator); }
    case '\'': { return tokenize_text(input + 1, 1, "\'", 1, result, allocator); }
    default: ;
  }
  length = t_KEY(input, result, allocator);
  if (length > 0) { return length; }
  result->type = XCONF_TOKEN_BAD_TOKEN;
  result->value = nullptr;
  result->length = 0;
  return 0;
}

uint32_t try_pass_comment(const char * const input, uint32_t * const lineno, uint32_t * const column) {
  const char *pText = input + 1;
  if (*pText == '/') {
    do { pText++; } while (*pText != '\n');
  } else if (*pText == '*') {
    pText++;
    do {
      while (*pText != '*') {
        if (*pText == '\n') { (*lineno)++, *column = 1; }
        pText++;
      }
    } while (*(++pText) != '/');
    pText++;
  } else {
    return 0;
  }
  *column += pText - input;
  return pText - input;
}

uint32_t pass_space(const char * const input, uint32_t * const lineno, uint32_t * const column) {
  uint32_t l = lineno ? *lineno : 0;
  uint32_t c = column ? *column : 0;
  const char *pText = input;
  while (*pText) {
    switch (*pText) {
      case '\n': {
        l++;
        c = 1;
        break;
      }
      case '\f':
      case '\r':
      case ' ':
      case '\t': {
        c++;
        break;
      }
      case '/': {
        uint32_t passed = try_pass_comment(pText, &l, &c);
        if (passed) {
          pText += passed;
          continue;
        }
      }
      default: {
        goto __return;
      }
    }
    pText++;
  }
__return:
  lineno ? *lineno = l : 0;
  column ? *column = c : 0;
  return pText - input;
}

void terminal2Token(Terminal *terminal, Token *token) {
  token->type = terminal->type;
  token->start.lineno = terminal->location.lineno;
  token->start.offset = terminal->location.offset;
  token->start.column = terminal->location.column;
  token->end.lineno = terminal->location.lineno;
  token->end.offset = terminal->location.offset + terminal->length;
  token->end.column = terminal->location.column + terminal->length;
  token->length = terminal->length;
  token->value = terminal->value;
}
