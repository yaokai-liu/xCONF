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

#ifndef XCONF_H
#define XCONF_H

#include <stdint.h>

#include "array.h"
#include "trie.h"

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

  XCONF_VAL_LINKAGE,
  XCONF_VAL_LINKED,
  XCONF_VAL_UNINITIALIZED = UINT32_MAX,
};

enum XCONF_ERROR_CODE_ENUM: uint32_t {
  XCONF_SUCCESS = 0,

  XCONF_ERROR_NO_SUCH_KEY,
  XCONF_ERROR_CONFLICT_KEY_TYPE,
  XCONF_ERROR_FIELD_UNDEFINED,
  XCONF_ERROR_INDEX_OUT_OF_RANGE,
  XCONF_ERROR_UNRECOGNIZED_SYMBOL,
  XCONF_ERROR_UNEXPECTED_EOF,
  XCONF_ERROR_UNEXPECTED_TOKEN,
  XCONF_ERROR_NO_SUCH_FILE,
  XCONF_ERROR_NON_INDEXABLE_VALUE,
  XCONF_ERROR_SELF_REFER,

  XCONF_ERROR_FAILED_TO_IMPORT_CONFIG,
  XCONF_ERROR_INVALID_PATH_ACTION,
  XCONF_ERROR_CREATING_EXISTED,
  XCONF_ERROR_OVER_BUFFER_SIZE,
  XCONF_ERROR_WRITE_UNSUPPORTED_VALUE,
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

typedef struct XCONFInstance  XCONFInstance;
typedef struct Dict           XCONFObject;
typedef struct Array          XCONFList;

/**
 * @description init the module
 * @return XCONF_SUCCESS
 */
XCONFInstance *XCONF_new(Trie *key_trie, Array *key_array, const Allocator *allocator);

/**
 * @description clean all involved data and environments except the key cache
 * @return XCONF_SUCCESS
 */
uint32_t XCONF_reset(const XCONFInstance *);

/**
* @description clean up the path environments of across configurations operations
 * @return XCONF_SUCCESS
 */
uint32_t XCONF_clear(const XCONFInstance *);

/**
 * @description finish the module
 * @return XCONF_SUCCESS
 */
void XCONF_destroy(XCONFInstance *);

/**
 * @description load configurations from a file
 * @param filepath
 * @param conf
 * @returns
 *      XCONF_SUCCESS           if there's no error
 *      XCONF_ERROR_SUCH_FILE   if the __filepath refers no file
 *      or                      other parse errors
 */
uint32_t XCONF_load(XCONFInstance *, const char *filepath, XCONFObject **conf);
/**
 * @description parse configurations from a string
 * @param string
 * @param conf
 * @returns
 *      XCONF_SUCCESS           if there's no error
 *      or                      other parse errors
 */
uint32_t XCONF_parse(XCONFInstance *, const char *string, XCONFObject **conf);
/**
 *
 * @description dump a configuration to file
 * @param conf
 * @param _filepath
 * @param compact
 * @return
 */
uint32_t XCONF_dump(const XCONFInstance *, XCONFObject *conf, bool compact, const char *_filepath);
/**
 * @description compose a configuration to string
 * @param conf
 * @param buffer
 * @param buffer_size
 * @returns
 *      XCONF_SUCCESS           if there's no error
 *      XCONF_ERROR_OVER_BUFFER_SIZE
 *                              if buffer's size less than requires
 *      or                      other parse errors
 */
uint32_t XCONF_compose(XCONFInstance *, XCONFObject *conf, bool compact, char *buffer, uint32_t buffer_size);

/**
 * @description create a new configuration
 * @param conf
 * @return
 */
uint32_t XCONF_create_conf(XCONFInstance *, XCONFObject **conf);
/**
 * @description create a new configuration and linked to an existed configuration
 * @param instance
 * @param conf
 * @param path
 * @param object
 * @return
 */
uint32_t XCONF_create_object(XCONFInstance *instance, XCONFObject *restrict conf, const char *path, XCONFObject *restrict *object);
/**
 * @description create a new value list and linked to an existed configuration
 * @param instance
 * @param conf
 * @param path
 * @param list
 * @return
 */
uint32_t XCONF_create_list(XCONFInstance *instance, XCONFObject *conf, const char *path, XCONFList **list);

uint32_t XCONFObject_set_float32(XCONFInstance *instance, XCONFObject *conf, const char *_path, float32_t value);
uint32_t XCONFObject_set_float64(XCONFInstance *instance, XCONFObject *conf, const char *_path, float64_t value);
uint32_t XCONFObject_set_float128(XCONFInstance *instance, XCONFObject *conf, const char *_path, float128_t value);
uint32_t XCONFObject_set_float256(XCONFInstance *instance, XCONFObject *conf, const char *_path, float256_t value);

uint32_t XCONFObject_set_uint32(XCONFInstance *instance, XCONFObject *conf, const char *_path, uint32_t value);
uint32_t XCONFObject_set_uint64(XCONFInstance *instance, XCONFObject *conf, const char *_path, uint64_t value);
uint32_t XCONFObject_set_uint128(XCONFInstance *instance, XCONFObject *conf, const char *_path, uint128_t value);
uint32_t XCONFObject_set_uint256(XCONFInstance *instance, XCONFObject *conf, const char *_path, uint256_t value);

uint32_t XCONFObject_set_int32(XCONFInstance *instance, XCONFObject *conf, const char *_path, int32_t value);
uint32_t XCONFObject_set_int64(XCONFInstance *instance, XCONFObject *conf, const char *_path, int64_t value);
uint32_t XCONFObject_set_int128(XCONFInstance *instance, XCONFObject *conf, const char *_path, int128_t value);
uint32_t XCONFObject_set_int256(XCONFInstance *instance, XCONFObject *conf, const char *_path, int256_t value);

uint32_t XCONFObject_set_null(XCONFInstance *instance, XCONFObject *conf, const char *_path);
uint32_t XCONFObject_set_bool(XCONFInstance *instance, XCONFObject *conf, const char *_path, bool value);
uint32_t XCONFObject_set_list(XCONFInstance *instance, XCONFObject *conf, const char *_path, XCONFList *list);
uint32_t XCONFObject_set_text(XCONFInstance *instance, XCONFObject *conf, const char *_path, const char *text);
uint32_t XCONFObject_set_object(XCONFInstance *instance, XCONFObject *conf, const char *_path, XCONFObject *object);

uint32_t XCONFObject_getValueType(XCONFInstance *instance, XCONFObject *conf, const char *_path, enum XCONF_VALUE_TYPE_ENUM *type);

uint32_t XCONFObject_get_float32(XCONFInstance *instance, XCONFObject *conf, const char *_path, float32_t *value);
uint32_t XCONFObject_get_float64(XCONFInstance *instance, XCONFObject *conf, const char *_path, float64_t *value);
uint32_t XCONFObject_get_float128(XCONFInstance *instance, XCONFObject *conf, const char *_path, float128_t *value);
uint32_t XCONFObject_get_float256(XCONFInstance *instance, XCONFObject *conf, const char *_path, float256_t *value);

uint32_t XCONFObject_get_uint32(XCONFInstance *instance, XCONFObject *conf, const char *_path, uint32_t *value);
uint32_t XCONFObject_get_uint64(XCONFInstance *instance, XCONFObject *conf, const char *_path, uint64_t *value);
uint32_t XCONFObject_get_uint128(XCONFInstance *instance, XCONFObject *conf, const char *_path, uint128_t *value);
uint32_t XCONFObject_get_uint256(XCONFInstance *instance, XCONFObject *conf, const char *_path, uint256_t *value);

uint32_t XCONFObject_get_int32(XCONFInstance *instance, XCONFObject *conf, const char *_path, int32_t *value);
uint32_t XCONFObject_get_int64(XCONFInstance *instance, XCONFObject *conf, const char *_path, int64_t *value);
uint32_t XCONFObject_get_int128(XCONFInstance *instance, XCONFObject *conf, const char *_path, int128_t *value);
uint32_t XCONFObject_get_int256(XCONFInstance *instance, XCONFObject *conf, const char *_path, int256_t *value);

uint32_t XCONFObject_get_bool(XCONFInstance *instance, XCONFObject *conf, const char *_path, bool *value);
uint32_t XCONFObject_get_list(XCONFInstance *instance, XCONFObject *conf, const char *_path, XCONFList **list);
uint32_t XCONFObject_get_object(XCONFInstance *instance, XCONFObject *conf, const char *_path, XCONFObject **object);
uint32_t XCONFObject_get_text(XCONFInstance *instance, XCONFObject *conf, const char *_path, const char **text, uint32_t *size);

uint32_t XCONFList_set_float32(XCONFInstance *instance, XCONFList *list, uint32_t index, float32_t value);
uint32_t XCONFList_set_float64(XCONFInstance *instance, XCONFList *list, uint32_t index, float64_t value);
uint32_t XCONFList_set_float128(XCONFInstance *instance, XCONFList *list, uint32_t index, float128_t value);
uint32_t XCONFList_set_float256(XCONFInstance *instance, XCONFList *list, uint32_t index, float256_t value);

uint32_t XCONFList_set_uint32(XCONFInstance *instance, XCONFList *list, uint32_t index, uint32_t value);
uint32_t XCONFList_set_uint64(XCONFInstance *instance, XCONFList *list, uint32_t index, uint64_t value);
uint32_t XCONFList_set_uint128(XCONFInstance *instance, XCONFList *list, uint32_t index, uint128_t value);
uint32_t XCONFList_set_uint256(XCONFInstance *instance, XCONFList *list, uint32_t index, uint256_t value);

uint32_t XCONFList_set_int32(XCONFInstance *instance, XCONFList *list, uint32_t index, int32_t value);
uint32_t XCONFList_set_int64(XCONFInstance *instance, XCONFList *list, uint32_t index, int64_t value);
uint32_t XCONFList_set_int128(XCONFInstance *instance, XCONFList *list, uint32_t index, int128_t value);
uint32_t XCONFList_set_int256(XCONFInstance *instance, XCONFList *list, uint32_t index, int256_t value);

uint32_t XCONFList_set_text(XCONFInstance *instance, XCONFList *list, uint32_t index, const char *text);
uint32_t XCONFList_set_object(XCONFInstance *instance, XCONFList *list, uint32_t index, XCONFObject *object);
uint32_t XCONFList_set_list(XCONFInstance *instance, XCONFList *list, uint32_t index, XCONFList *_list);

uint32_t XCONFList_getValueType(XCONFInstance *instance, XCONFList *list, uint32_t index, enum XCONF_VALUE_TYPE_ENUM *type);

uint32_t XCONFList_get_float32(XCONFInstance *instance, XCONFList *list, uint32_t index, float32_t *value);
uint32_t XCONFList_get_float64(XCONFInstance *instance, XCONFList *list, uint32_t index, float64_t *value);
uint32_t XCONFList_get_float128(XCONFInstance *instance, XCONFList *list, uint32_t index, float128_t *value);
uint32_t XCONFList_get_float256(XCONFInstance *instance, XCONFList *list, uint32_t index, float256_t *value);

uint32_t XCONFList_get_uint32(XCONFInstance *instance, XCONFList *list, uint32_t index, uint32_t *value);
uint32_t XCONFList_get_uint64(XCONFInstance *instance, XCONFList *list, uint32_t index, uint64_t *value);
uint32_t XCONFList_get_uint128(XCONFInstance *instance, XCONFList *list, uint32_t index, uint128_t *value);
uint32_t XCONFList_get_uint256(XCONFInstance *instance, XCONFList *list, uint32_t index, uint256_t *value);

uint32_t XCONFList_get_int32(XCONFInstance *instance, XCONFList *list, uint32_t index, int32_t *value);
uint32_t XCONFList_get_int64(XCONFInstance *instance, XCONFList *list, uint32_t index, int64_t *value);
uint32_t XCONFList_get_int128(XCONFInstance *instance, XCONFList *list, uint32_t index, int128_t *value);
uint32_t XCONFList_get_int256(XCONFInstance *instance, XCONFList *list, uint32_t index, int256_t *value);

uint32_t XCONFList_get_list(XCONFInstance *instance, XCONFList *list, uint32_t index, XCONFList **_list);
uint32_t XCONFList_get_object(XCONFInstance *instance, XCONFList *list, uint32_t index, XCONFObject **object);
uint32_t XCONFList_get_text(XCONFInstance *instance, XCONFList *list, uint32_t index, const char **text, uint32_t *size);

uint32_t XCONF_keys(XCONFInstance *instance, XCONFObject *conf, const char **keys, uint32_t *count);
uint32_t XCONFList_count(const XCONFList *list, uint32_t *count);

uint32_t XCONF_remove_key(XCONFInstance *instance, XCONFObject *conf, const char *_path, const char *key);
uint32_t XCONFList_remove_value(XCONFInstance *instance, XCONFList *list, uint32_t index);

uint32_t XCONFObject_destroy(XCONFObject *conf);

#endif //XCONF_H
