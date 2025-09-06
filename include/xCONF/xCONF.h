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
 * Module Name: xCONF
 * Filename: xCONF.h
 * Creator: Yaokai Liu
 * Create Date: 2025-06-11
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#ifndef __XCONF_H__
#define __XCONF_H__

#include <stdint.h>

enum XCONF_VALUE_TYPE_ENUM: uint32_t {
  XCONF_VAL_NULL,
  XCONF_VAL_LIST,
  XCONF_VAL_TEXT,
  XCONF_VAL_OBJECT,
  XCONF_VAL_BOOLEAN,

  XCONF_VAL_I32,
  XCONF_VAL_U32,
  XCONF_VAL_I64,
  XCONF_VAL_U64,
  XCONF_VAL_I128,
  XCONF_VAL_U128,
  XCONF_VAL_I256,
  XCONF_VAL_U256,
  XCONF_VAL_F32,
  XCONF_VAL_F64,
  XCONF_VAL_F128,
  XCONF_VAL_F256,

  XCONF_VAL_UNINITIALIZED = UINT32_MAX,
};

enum XCONF_ERROR_CODE_ENUM: uint32_t {
  XCONF_SUCCESS = 0,

  XCONF_ERROR_NO_SUCH_KEY,
  XCONF_ERROR_CONFLICT_KEY,
  XCONF_ERROR_FIELD_UNDEFINED,
  XCONF_ERROR_INDEX_OUT_OF_RANGE,
  XCONF_ERROR_UNRECOGNIZED_SYMBOL,
  XCONF_ERROR_UNEXPECTED_EOF,
  XCONF_ERROR_NO_SUCH_FILE,

  XCONF_ERROR_INVALID_PATH_ACTION,
  XCONF_ERROR_CREATING_EXISTED,
  XCONF_ERROR_OVER_BUFFER_SIZE,
  XCONF_ERROR_DUMP_UNSUPPORTED_VALUE,

  XCONF_UNEXPECTED_TOKEN = UINT32_MAX,
};

typedef signed    _BitInt(128) int128_t;
typedef unsigned  _BitInt(128) uint128_t;
typedef signed    _BitInt(256) int256_t;
typedef unsigned  _BitInt(256) uint256_t;

typedef float         float32_t;
typedef double        float64_t;
typedef long double   float128_t;
typedef float128_t    float256_t; // optional: support for 256-bit float point number.

typedef struct Array          XCONFList;
typedef struct XCONFInstance  XCONFInstance;
typedef struct XCONFObject    XCONFObject, XCONF;

/**
 * @description init the module
 * @return XCONF_SUCCESS
 */
uint32_t XCONF_init();

/**
 * @description clean all involved data and environments except the key cache
 * @return XCONF_SUCCESS
 */
uint32_t XCONF_reset();

/**
* @description clean up the path environments of across configurations operations
 * @return XCONF_SUCCESS
 */
uint32_t XCONF_clear();

/**
 * @description finish the module
 * @return XCONF_SUCCESS
 */
uint32_t XCONF_finish();

/**
 * @description load configurations from a file
 * @param __filepath
 * @param __conf
 * @returns
 *      XCONF_SUCCESS           if there's no error
 *      XCONF_ERROR_SUCH_FILE   if the __filepath refers no file
 *      or                      other parse errors
 */
uint32_t XCONF_load(const char *__filepath, XCONF **__conf);
/**
 * @description parse configurations from a string
 * @param __string
 * @param __conf
 * @returns
 *      XCONF_SUCCESS           if there's no error
 *      or                      other parse errors
 */
uint32_t XCONF_parse(const char *__string, XCONF **__conf);
/**
 *
 * @description dump a configuration to file
 * @param __conf
 * @param __filepath
 * @return
 */
uint32_t XCONF_dump(XCONF *__conf, const char *__filepath);
/**
 * @description compose a configuration to string
 * @param __conf
 * @param buffer
 * @param buffer_size
 * @returns
 *      XCONF_SUCCESS           if there's no error
 *      XCONF_ERROR_OVER_BUFFER_SIZE
 *                              if buffer's size less than requires
 *      or                      other parse errors
 */
uint32_t XCONF_compose(XCONF *__conf, char *buffer, uint32_t buffer_size);

/**
 * @description create a new configuration
 * @param __conf
 * @return
 */
uint32_t XCONF_create_conf(XCONF **__conf);
/**
 * @description create a new configuration and linked to an existed configuration
 * @param __conf
 * @param __path
 * @param object
 * @return
 */
uint32_t XCONF_create_object(XCONF *__conf, const char *__path, XCONFObject **object);
/**
 * @description create a new value list and linked to an existed configuration
 * @param __conf
 * @param __path
 * @param list
 * @return
 */
uint32_t XCONF_create_list(XCONF *__conf, const char *__path, XCONFList **list);

uint32_t XCONF_set_float32(XCONF *__conf, const char *__path, float32_t value);
uint32_t XCONF_set_float64(XCONF *__conf, const char *__path, float64_t value);
uint32_t XCONF_set_float128(XCONF *__conf, const char *__path, float128_t value);
uint32_t XCONF_set_float256(XCONF *__conf, const char *__path, float256_t value);

uint32_t XCONF_set_uint32(XCONF *__conf, const char *__path, uint32_t value);
uint32_t XCONF_set_uint64(XCONF *__conf, const char *__path, uint64_t value);
uint32_t XCONF_set_uint128(XCONF *__conf, const char *__path, uint128_t value);
uint32_t XCONF_set_uint256(XCONF *__conf, const char *__path, uint256_t value);

uint32_t XCONF_set_int32(XCONF *__conf, const char *__path, int32_t value);
uint32_t XCONF_set_int64(XCONF *__conf, const char *__path, int64_t value);
uint32_t XCONF_set_int128(XCONF *__conf, const char *__path, int128_t value);
uint32_t XCONF_set_int256(XCONF *__conf, const char *__path, int256_t value);

uint32_t XCONF_set_null(XCONF *__conf, const char *__path);
uint32_t XCONF_set_bool(XCONF *__conf, const char *__path, bool value);
uint32_t XCONF_set_list(XCONF *__conf, const char *__path, XCONFList *list);
uint32_t XCONF_set_text(XCONF *__conf, const char *__path, const char *text);
uint32_t XCONF_set_object(XCONF *__conf, const char *__path, XCONFObject *object);

uint32_t XCONF_getValueType(XCONF *__conf, const char *__path, enum XCONF_VALUE_TYPE_ENUM *type);

uint32_t XCONF_get_float32(XCONF *__conf, const char *__path, float32_t *value);
uint32_t XCONF_get_float64(XCONF *__conf, const char *__path, float64_t *value);
uint32_t XCONF_get_float128(XCONF *__conf, const char *__path, float128_t *value);
uint32_t XCONF_get_float256(XCONF *__conf, const char *__path, float256_t *value);

uint32_t XCONF_get_uint32(XCONF *__conf, const char *__path, uint32_t *value);
uint32_t XCONF_get_uint64(XCONF *__conf, const char *__path, uint64_t *value);
uint32_t XCONF_get_uint128(XCONF *__conf, const char *__path, uint128_t *value);
uint32_t XCONF_get_uint256(XCONF *__conf, const char *__path, uint256_t *value);

uint32_t XCONF_get_int32(XCONF *__conf, const char *__path, int32_t *value);
uint32_t XCONF_get_int64(XCONF *__conf, const char *__path, int64_t *value);
uint32_t XCONF_get_int128(XCONF *__conf, const char *__path, int128_t *value);
uint32_t XCONF_get_int256(XCONF *__conf, const char *__path, int256_t *value);

uint32_t XCONF_get_bool(XCONF *__conf, const char *__path, bool *value);
uint32_t XCONF_get_list(XCONF *__conf, const char *__path, XCONFList **list);
uint32_t XCONF_get_object(XCONF *__conf, const char *__path, XCONFObject **object);
uint32_t XCONF_get_text(XCONF *__conf, const char *__path, const char **text, uint32_t *size);

uint32_t XCONFList_set_float32(XCONFList *__list, uint32_t index, float32_t value);
uint32_t XCONFList_set_float64(XCONFList *__list, uint32_t index, float64_t value);
uint32_t XCONFList_set_float128(XCONFList *__list, uint32_t index, float128_t value);
uint32_t XCONFList_set_float256(XCONFList *__list, uint32_t index, float256_t value);

uint32_t XCONFList_set_uint32(XCONFList *__list, uint32_t index, uint32_t value);
uint32_t XCONFList_set_uint64(XCONFList *__list, uint32_t index, uint64_t value);
uint32_t XCONFList_set_uint128(XCONFList *__list, uint32_t index, uint128_t value);
uint32_t XCONFList_set_uint256(XCONFList *__list, uint32_t index, uint256_t value);

uint32_t XCONFList_set_int32(XCONFList *__list, uint32_t index, int32_t value);
uint32_t XCONFList_set_int64(XCONFList *__list, uint32_t index, int64_t value);
uint32_t XCONFList_set_int128(XCONFList *__list, uint32_t index, int128_t value);
uint32_t XCONFList_set_int256(XCONFList *__list, uint32_t index, int256_t value);

uint32_t XCONFList_set_text(XCONFList *__list, uint32_t index, const char *text);
uint32_t XCONFList_set_object(XCONFList *__list, uint32_t index, XCONFObject *object);
uint32_t XCONFList_set_list(XCONFList *__list, uint32_t index, XCONFList *list);

uint32_t XCONFList_getValueType(XCONFList *__list, uint32_t index, enum XCONF_VALUE_TYPE_ENUM *type);

uint32_t XCONFList_get_float32(XCONFList *__list, uint32_t index, float32_t *value);
uint32_t XCONFList_get_float64(XCONFList *__list, uint32_t index, float64_t *value);
uint32_t XCONFList_get_float128(XCONFList *__list, uint32_t index, float128_t *value);
uint32_t XCONFList_get_float256(XCONFList *__list, uint32_t index, float256_t *value);

uint32_t XCONFList_get_uint32(XCONFList *__list, uint32_t index, uint32_t *value);
uint32_t XCONFList_get_uint64(XCONFList *__list, uint32_t index, uint64_t *value);
uint32_t XCONFList_get_uint128(XCONFList *__list, uint32_t index, uint128_t *value);
uint32_t XCONFList_get_uint256(XCONFList *__list, uint32_t index, uint256_t *value);

uint32_t XCONFList_get_int32(XCONFList *__list, uint32_t index, int32_t *value);
uint32_t XCONFList_get_int64(XCONFList *__list, uint32_t index, int64_t *value);
uint32_t XCONFList_get_int128(XCONFList *__list, uint32_t index, int128_t *value);
uint32_t XCONFList_get_int256(XCONFList *__list, uint32_t index, int256_t *value);

uint32_t XCONFList_get_list(XCONFList *__list, uint32_t index, XCONFList **list);
uint32_t XCONFList_get_object(XCONFList *__list, uint32_t index, XCONFObject **object);
uint32_t XCONFList_get_text(XCONFList *__list, uint32_t index, const char **text, uint32_t *size);

uint32_t XCONF_keys(XCONF *__conf, const char **keys, uint32_t *count);
uint32_t XCONFList_count(XCONFList *__list, uint32_t *count);

#endif //__XCONF_H__
