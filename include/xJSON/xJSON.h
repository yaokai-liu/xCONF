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
 * Module Name: xJSON
 * Filename: xJSON.h
 * Creator: Yaokai Liu
 * Create Date: 2025-06-11
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#ifndef __XJSON_H__
#define __XJSON_H__

#include <stdint.h>
#include "enum.h"
#include "extint.h"
#include "extfloat.h"

enum XJSON_ERROR_CODE_ENUM {
  XJSON_ERROR_KEY_ERROR,
  XJSON_ERROR_INDEX_OUT_OF_RANGE,
};

typedef struct XJSON XJSON, XJSONObject;
typedef struct XJSONList XJSONList;

uint32_t XJSON_load(const char *__filepath, XJSON **__json);
uint32_t XJSON_dump(XJSON **__json, const char *__filepath);
uint32_t XJSON_parse(char *__string, XJSON **__json);
uint32_t XJSON_compose(XJSON **__json, char *__string);

uint32_t XJSON_createObject(XJSON *__json, char *__path, XJSONObject **object);
uint32_t XJSON_createList(XJSON *__json, char *__path, XJSONList **list);

uint32_t XJSON_set_float32(XJSON *__json, char *__path, float32_t value);
uint32_t XJSON_set_float64(XJSON *__json, char *__path, float64_t value);
uint32_t XJSON_set_float128(XJSON *__json, char *__path, float128_t value);
uint32_t XJSON_set_float256(XJSON *__json, char *__path, float256_t value);

uint32_t XJSON_set_uint32(XJSON *__json, char *__path, uint32_t value);
uint32_t XJSON_set_uint64(XJSON *__json, char *__path, uint64_t value);
uint32_t XJSON_set_uint128(XJSON *__json, char *__path, uint128_t value);
uint32_t XJSON_set_uint256(XJSON *__json, char *__path, uint256_t value);

uint32_t XJSON_set_int32(XJSON *__json, char *__path, int32_t value);
uint32_t XJSON_set_int64(XJSON *__json, char *__path, int64_t value);
uint32_t XJSON_set_int128(XJSON *__json, char *__path, int128_t value);
uint32_t XJSON_set_int256(XJSON *__json, char *__path, int256_t value);

uint32_t XJSON_set_text(XJSON *__json, char *__path, const char *text);
uint32_t XJSON_set_object(XJSON *__json, char *__path, XJSONObject *object);
uint32_t XJSON_set_list(XJSON *__json, char *__path, XJSONList *list);

uint32_t XJSON_getValueType(XJSON *__json, char *__path, XJSON_val_type *type);

uint32_t XJSON_get_float32(XJSON *__json, char *__path, float32_t *value);
uint32_t XJSON_get_float64(XJSON *__json, char *__path, float64_t *value);
uint32_t XJSON_get_float128(XJSON *__json, char *__path, float128_t *value);
uint32_t XJSON_get_float256(XJSON *__json, char *__path, float256_t *value);

uint32_t XJSON_get_uint32(XJSON *__json, char *__path, uint32_t *value);
uint32_t XJSON_get_uint64(XJSON *__json, char *__path, uint64_t *value);
uint32_t XJSON_get_uint128(XJSON *__json, char *__path, uint128_t *value);
uint32_t XJSON_get_uint256(XJSON *__json, char *__path, uint256_t *value);

uint32_t XJSON_get_int32(XJSON *__json, char *__path, int32_t *value);
uint32_t XJSON_get_int64(XJSON *__json, char *__path, int64_t *value);
uint32_t XJSON_get_int128(XJSON *__json, char *__path, int128_t *value);
uint32_t XJSON_get_int256(XJSON *__json, char *__path, int256_t *value);

uint32_t XJSON_get_text(XJSON *__json, char *__path, const char **text);
uint32_t XJSON_get_list(XJSON *__json, char *__path, XJSONList **list);
uint32_t XJSON_get_object(XJSON *__json, char *__path, XJSONObject **object);

uint32_t XJSONList_set_float32(XJSONList *__list, uint32_t index, float32_t value);
uint32_t XJSONList_set_float64(XJSONList *__list, uint32_t index, float64_t value);
uint32_t XJSONList_set_float128(XJSONList *__list, uint32_t index, float128_t value);
uint32_t XJSONList_set_float256(XJSONList *__list, uint32_t index, float256_t value);

uint32_t XJSONList_set_uint32(XJSONList *__list, uint32_t index, uint32_t value);
uint32_t XJSONList_set_uint64(XJSONList *__list, uint32_t index, uint64_t value);
uint32_t XJSONList_set_uint128(XJSONList *__list, uint32_t index, uint128_t value);
uint32_t XJSONList_set_uint256(XJSONList *__list, uint32_t index, uint256_t value);

uint32_t XJSONList_set_int32(XJSONList *__list, uint32_t index, int32_t value);
uint32_t XJSONList_set_int64(XJSONList *__list, uint32_t index, int64_t value);
uint32_t XJSONList_set_int128(XJSONList *__list, uint32_t index, int128_t value);
uint32_t XJSONList_set_int256(XJSONList *__list, uint32_t index, int256_t value);

uint32_t XJSONList_set_text(XJSONList *__list, uint32_t index, const char *text);
uint32_t XJSONList_set_object(XJSONList *__list, uint32_t index, XJSONObject *object);
uint32_t XJSONList_set_list(XJSONList *__list, uint32_t index, XJSONList *list);

uint32_t XJSONList_getValueType(XJSONList *__list, uint32_t index, XJSON_val_type *type);

uint32_t XJSONList_get_float32(XJSONList *__list, uint32_t index, float32_t *value);
uint32_t XJSONList_get_float64(XJSONList *__list, uint32_t index, float64_t *value);
uint32_t XJSONList_get_float128(XJSONList *__list, uint32_t index, float128_t *value);
uint32_t XJSONList_get_float256(XJSONList *__list, uint32_t index, float256_t *value);

uint32_t XJSONList_get_uint32(XJSONList *__list, uint32_t index, uint32_t *value);
uint32_t XJSONList_get_uint64(XJSONList *__list, uint32_t index, uint64_t *value);
uint32_t XJSONList_get_uint128(XJSONList *__list, uint32_t index, uint128_t *value);
uint32_t XJSONList_get_uint256(XJSONList *__list, uint32_t index, uint256_t *value);

uint32_t XJSONList_get_int32(XJSONList *__list, uint32_t index, int32_t *value);
uint32_t XJSONList_get_int64(XJSONList *__list, uint32_t index, int64_t *value);
uint32_t XJSONList_get_int128(XJSONList *__list, uint32_t index, int128_t *value);
uint32_t XJSONList_get_int256(XJSONList *__list, uint32_t index, int256_t *value);

uint32_t XJSONList_get_text(XJSONList *__list, uint32_t index, const char **text);
uint32_t XJSONList_get_list(XJSONList *__list, uint32_t index, XJSONList **list);
uint32_t XJSONList_get_object(XJSONList *__list, uint32_t index, XJSONObject **object);

uint32_t XJSON_keys(XJSON *__json, const char * const *keys);
uint32_t XJSON_count(XJSON *__json, const char * const *keys);
uint32_t XJSONList_count(XJSONList *__list, uint32_t *count);

#endif //__XJSON_H__
