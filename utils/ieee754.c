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
 * Module Name: utils
 * Filename: ieee754.c
 * Creator: Yaokai Liu
 * Create Date: 2025-06-08
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include "ieee754.h"
#include <stdbit.h>
#include "xJSON/extint.h"

unsigned int first_leading_one_u128 (uint128_t x);
unsigned int first_leading_one_u256 (uint256_t x);

inline unsigned int first_leading_one_u128 (uint128_t x) {
  uint64_t p;
  if ((p = (x >> 64) & ~0ULL)) { return stdc_first_leading_one_ul(p) + 64; }
  if ((p = x & ~0ULL)) { return stdc_first_leading_one_ul(p); }
  return 0;
}

inline unsigned int first_leading_one_u256 (uint256_t x) {
  uint64_t p;
  if ((p = (x >> 192) & ~0ULL)) { return stdc_first_leading_one_ul(p) + 192; }
  if ((p = (x >> 128) & ~0ULL)) { return stdc_first_leading_one_ul(p) + 128; }
  if ((p = (x >> 64) & ~0ULL)) { return stdc_first_leading_one_ul(p) + 64; }
  if ((p = x & ~0ULL)) { return stdc_first_leading_one_ul(p); }
  return 0;
}

#define first_leading_one(u)  _Generic((u),   \
  uint32_t:   stdc_first_leading_one_ui(u),   \
  uint64_t:   stdc_first_leading_one_ul(u),   \
  uint128_t:  first_leading_one_u128(u),      \
  uint256_t:  first_leading_one_u256(u),      \
  default:    0                               \
)

#define FLOAT(w) float##w##_t
#define UINT(w) uint##w##_t
#define INT(w) int##w##_t

#define u_to_ieee754bin(_w, _ew, _eb)                                                 \
inline FLOAT(_w) ieee754bin##_w(bool sign, UINT(_w) integer, UINT(_ew) exponent) {    \
  constexpr uint32_t result_bits = _w;                                                \
  constexpr uint32_t order_bits = _eb;                                                \
  constexpr UINT(_w) order_MASK = (((UINT(_w)) 1) << order_bits) - 1;                 \
  constexpr uint32_t order_offs = (1 << (order_bits - 1)) - 1;                        \
  constexpr uint32_t tail_bits = result_bits - order_bits - 1;                        \
  constexpr UINT(_w) tail_MASK = (((UINT(_w)) 1) << tail_bits) - 1;                   \
  const uint32_t effect_bits = result_bits - first_leading_one(integer) + 1;          \
  sign = (sign != 0);                                                                 \
  exponent = (exponent + order_offs) & order_MASK;                                    \
  uint32_t adjust = tail_bits - effect_bits + (exponent != 0);                        \
  integer = (effect_bits <= tail_bits) ? (integer << adjust) : (integer >> -adjust);  \
  union { UINT(_w) I; FLOAT(_w) F; } val = {};                                        \
  val.I = (((UINT(_w)) sign) << (result_bits - 1))                                    \
        | (((UINT(_w)) exponent) << tail_bits)                                        \
        | (integer & tail_MASK);                                                      \
  return val.F;                                                                       \
}

u_to_ieee754bin(32, 8, 8)
u_to_ieee754bin(64, 16, 11)
u_to_ieee754bin(128, 16, 15)
/*
 * gcc not support float type in IEEE-754 binary 256
 * u_to_ieee754bin(256, 32, 19)
 */
