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
 * Module Name: interface
 * Filename: interface.c
 * Creator: Yaokai Liu
 * Create Date: 2025-06-11
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include "xCONF/context.h"
#include "xCONF/xCONF.h"
#include "allocator.h"
#include "tokenize/tokenizer.h"
#include "xCONF/parse.h"
#include "compose.h"
#include "Path/parse.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stddef.h>

static struct XCONFInstance {
  const Allocator *allocator;
  XCONFTokenizer *tokenizer;
  XCONFContext *context;
  ErrInfo errInfo;
} INSTANCE = {.allocator = nullptr, .context = nullptr, .tokenizer = nullptr};

uint32_t XCONF_init() {
  INSTANCE.allocator = &STDAllocator;
  INSTANCE.context = XCONFContext_new(INSTANCE.allocator);
  INSTANCE.tokenizer = XCONFTokenizer_new(nullptr, INSTANCE.context->key_array,
                                          INSTANCE.context->key_trie, INSTANCE.allocator);
  return XCONF_SUCCESS;
}

uint32_t XCONF_reset() {
  Array_reset(INSTANCE.context->text_array, nullptr);
  Array_reset(INSTANCE.context->value_array, (destruct_t *) releaseValue);
  INSTANCE.context->path_action = XCONF_PATH_ACTION_UNSET;
  XCONFContext_clear(INSTANCE.context);

  return XCONF_SUCCESS;
}

uint32_t XCONF_clear() {
  INSTANCE.context->object = nullptr;
  XCONFContext_clear(INSTANCE.context);
  XCONFTokenizer_setSrc(INSTANCE.tokenizer, nullptr);
  INSTANCE.context->path_action = XCONF_PATH_ACTION_UNSET;
  return XCONF_SUCCESS;
}

uint32_t XCONF_finish() {
  XCONFContext_destroy(INSTANCE.context);
  XCONFTokenizer_destroy(INSTANCE.tokenizer);
  return XCONF_SUCCESS;
}

uint32_t XCONF_load(const char *__filepath, XCONF **__conf) {
  FILE *file = fopen(__filepath, "r");
  if (!file) { return XCONF_ERROR_NO_SUCH_FILE; }
  fseek(file, 0, SEEK_END);
  uint32_t size = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *buffer = malloc(size * sizeof(char));
  fread(buffer, sizeof(char), size, file);

  uint32_t result = XCONF_parse(buffer, __conf);

  free(buffer);
  fclose(file);

  return result;
}

uint32_t XCONF_parse(const char *__string, XCONF **__conf) {
  XCONFContext_clear(INSTANCE.context);
  XCONFTokenizer_setSrc(INSTANCE.tokenizer, __string);
  INSTANCE.context->path_action = XCONF_PATH_ACTION_BUILD;
  XCONF *conf = parse(INSTANCE.tokenizer, INSTANCE.context, &INSTANCE.errInfo, INSTANCE.allocator);
  if (!conf) { return INSTANCE.errInfo.code; }
  *__conf = conf;
  return XCONF_SUCCESS;
}

uint32_t XCONF_dump(XCONF *__conf, const char *__filepath) {
  FILE *file = fopen(__filepath, "wb");
  uint32_t result = writeObject(file, __conf, 0, INSTANCE.context);
  fclose(file);
  return result;
}

uint32_t XCONF_compose(XCONF *__conf, char *buffer, uint32_t buffer_size) {
  FILE *file = tmpfile();
  uint32_t result = writeObject(file, __conf, 0, INSTANCE.context);
  if (result != XCONF_SUCCESS) { return result; }
  fseek(file, 0, SEEK_END);
  uint32_t size = ftell(file);
  fseek(file, 0, SEEK_SET);
  if (size > buffer_size) { return XCONF_ERROR_OVER_BUFFER_SIZE; }
  fread(buffer, sizeof(char), size, file);
  buffer[size] = '\0';
  fclose(file);
  return XCONF_SUCCESS;
}

uint32_t XCONF_create_conf(XCONF **__conf) {
  *__conf = Object_new(INSTANCE.allocator);
  return XCONF_SUCCESS;
}

static uint32_t XCONF_get_path_value(XCONF *__conf, const char *__path, uint32_t action, Value **value);
static uint32_t XCONFList_get_value(XCONFList *__list, uint32_t index, Value **value);

uint32_t XCONF_create_object(XCONF *__conf, const char *__path, XCONFObject **object) {
  Value *value = nullptr;
  uint32_t result = XCONF_get_path_value(__conf, __path, XCONF_PATH_ACTION_BUILD, &value);
  if (result != XCONF_SUCCESS) { return result; }
  if (value->type != XCONF_VAL_UNINITIALIZED) { return XCONF_ERROR_CREATING_EXISTED; }

  value->val.OBJECT = Object_new(INSTANCE.allocator);
  value->type = XCONF_VAL_OBJECT;
  value->size = 0;

  *object = value->val.OBJECT;

  return XCONF_SUCCESS;
}

uint32_t XCONF_create_list(XCONF *__conf, const char *__path, XCONFList **list) {
  Value *value = nullptr;
  const uint32_t result = XCONF_get_path_value(__conf, __path, XCONF_PATH_ACTION_BUILD, &value);
  if (result != XCONF_SUCCESS) { return result; }
  if (value->type != XCONF_VAL_UNINITIALIZED) { return XCONF_ERROR_CREATING_EXISTED; }

  value->val.LIST = Array_new(sizeof(REFER(Value)), XCONF_REFER_VALUE_ARRAY, INSTANCE.allocator);
  value->type = XCONF_VAL_LIST;
  value->size = 0;

  *list = value->val.LIST;

  return XCONF_SUCCESS;
}


#define XCONF_set(_type, _width, _field)                                                       \
uint32_t XCONF_set_##_type##_width(XCONF *__conf, const char *__path, _type##_width##_t val) { \
  Value *value = nullptr;                                                                      \
  uint32_t result = XCONF_get_path_value(__conf, __path, XCONF_PATH_ACTION_BUILD, &value);     \
  if (result != XCONF_SUCCESS) { return result; }                                              \
  if (value->type != XCONF_VAL_UNINITIALIZED) { releaseValue(value, INSTANCE.allocator); }     \
                                                                                               \
  value->type = XCONF_VAL_##_field##_width;                                                    \
  value->val._field##_width = val;                                                             \
  value->size = _width / 8;                                                                    \
                                                                                               \
  return XCONF_SUCCESS;                                                                        \
}                                                                                              \

#define XCONF_set_keyword(_name, _a_type, _v_type, _field)                                 \
uint32_t XCONF_set_##_name(XCONF *__conf, const char *__path, _a_type val) {               \
  Value *value = nullptr;                                                                  \
  uint32_t result = XCONF_get_path_value(__conf, __path, XCONF_PATH_ACTION_BUILD, &value); \
  if (result != XCONF_SUCCESS) { return result; }                                          \
  if (value->type != XCONF_VAL_UNINITIALIZED) { releaseValue(value, INSTANCE.allocator); } \
                                                                                           \
  value->type = XCONF_VAL_##_v_type;                                                       \
  value->val._field = val;                                                                 \
  value->size = 0;                                                                         \
                                                                                           \
  return XCONF_SUCCESS;                                                                    \
}                                                                                          \

XCONF_set(int,  32, I)
XCONF_set(int,  64, I)
XCONF_set(int, 128, I)
XCONF_set(int, 256, I)

XCONF_set(uint,  32, U)
XCONF_set(uint,  64, U)
XCONF_set(uint, 128, U)
XCONF_set(uint, 256, U)

XCONF_set(float,  32, F)
XCONF_set(float,  64, F)
XCONF_set(float, 128, F)
XCONF_set(float, 256, F)

XCONF_set_keyword(bool, bool, BOOLEAN, BOOLEAN)

uint32_t XCONF_set_null(XCONF *__conf, const char *__path) {
  Value *value = nullptr;
  uint32_t result = XCONF_get_path_value(__conf, __path, XCONF_PATH_ACTION_BUILD, &value);
  if (result != XCONF_SUCCESS) { return result; }
  if (value->type != XCONF_VAL_UNINITIALIZED) { releaseValue(value, INSTANCE.allocator); }
  value->type = XCONF_VAL_NULL;
  value->val.U256 = 0;
  value->size = 0;
  return XCONF_SUCCESS;
}

uint32_t XCONF_set_text(XCONF *__conf, const char *__path, const char *text) {
  Value *value = nullptr;
  uint32_t result = XCONF_get_path_value(__conf, __path, XCONF_PATH_ACTION_BUILD, &value);
  if (result != XCONF_SUCCESS) { return result; }
  if (value->type != XCONF_VAL_UNINITIALIZED) { releaseValue(value, INSTANCE.allocator); }

  value->type = XCONF_VAL_TEXT;
  value->size = strlen(text);
  value->val.TEXT = Array_last_virt(INSTANCE.context->text_array) + 1;
  Array_append(INSTANCE.context->text_array, text, value->size + 1);

  return XCONF_SUCCESS;
}

uint32_t XCONF_set_object(XCONF *__conf, const char *__path, XCONFObject *object) {
  Value *value = nullptr;
  uint32_t result = XCONF_get_path_value(__conf, __path, XCONF_PATH_ACTION_BUILD, &value);
  if (result != XCONF_SUCCESS) { return result; }
  if (value->type != XCONF_VAL_UNINITIALIZED) { releaseValue(value, INSTANCE.allocator); }

  value->type = XCONF_VAL_LIST;
  value->val.OBJECT = object;
  value->size = 0;

  return XCONF_SUCCESS;
}

uint32_t XCONF_set_list(XCONF *__conf, const char *__path, XCONFList *list) {
  Value *value = nullptr;
  uint32_t result = XCONF_get_path_value(__conf, __path, XCONF_PATH_ACTION_BUILD, &value);
  if (result != XCONF_SUCCESS) { return result; }
  if (value->type != XCONF_VAL_UNINITIALIZED) { releaseValue(value, INSTANCE.allocator); }

  value->type = XCONF_VAL_LIST;
  value->val.LIST = list;
  value->size = 0;

  return XCONF_SUCCESS;
}

uint32_t XCONF_getValueType(XCONF *__conf, const char *__path, enum XCONF_VALUE_TYPE_ENUM *type) {
  Value *value = nullptr;
  uint32_t result = XCONF_get_path_value(__conf, __path, XCONF_PATH_ACTION_ACCESS, &value);
  if (result != XCONF_SUCCESS) { return result; }

  *type = value->type;

  return XCONF_SUCCESS;
}

#define XCONF_get(_type, _width, _field)                                                        \
uint32_t XCONF_get_##_type##_width(XCONF *__conf, const char *__path, _type##_width##_t *val) { \
  Value *value = nullptr;                                                                       \
  uint32_t result = XCONF_get_path_value(__conf, __path, XCONF_PATH_ACTION_ACCESS, &value);     \
  if (result != XCONF_SUCCESS) { return result; }                                               \
  if (value->type == XCONF_VAL_UNINITIALIZED) { return XCONF_ERROR_FIELD_UNDEFINED; }           \
                                                                                                \
  *val = value->val._field##_width;                                                             \
                                                                                                \
  return XCONF_SUCCESS;                                                                         \
}                                                                                               \

#define XCONF_get_keyword(_name, _a_type, _field)                                           \
uint32_t XCONF_get_##_name(XCONF *__conf, const char *__path, _a_type *val) {               \
  Value *value = nullptr;                                                                   \
  uint32_t result = XCONF_get_path_value(__conf, __path, XCONF_PATH_ACTION_ACCESS, &value); \
  if (result != XCONF_SUCCESS) { return result; }                                           \
  if (value->type == XCONF_VAL_UNINITIALIZED) { return XCONF_ERROR_FIELD_UNDEFINED; }       \
                                                                                            \
  *val = value->val._field;                                                                 \
                                                                                            \
  return XCONF_SUCCESS;                                                                     \
}                                                                                           \


XCONF_get(int,  32, I)
XCONF_get(int,  64, I)
XCONF_get(int, 128, I)
XCONF_get(int, 256, I)

XCONF_get(uint,  32, U)
XCONF_get(uint,  64, U)
XCONF_get(uint, 128, U)
XCONF_get(uint, 256, U)

XCONF_get(float,  32, F)
XCONF_get(float,  64, F)
XCONF_get(float, 128, F)
XCONF_get(float, 256, F)

// XCONF_get_keyword(bool, bool, BOOLEAN)

uint32_t XCONF_get_bool(XCONF *__conf, const char *__path, bool *val) {
  Value *value = nullptr;
  uint32_t result = XCONF_get_path_value(__conf, __path, XCONF_PATH_ACTION_ACCESS, &value);
  if (result != XCONF_SUCCESS) { return result; }
  if (value->type == XCONF_VAL_UNINITIALIZED) { return XCONF_ERROR_FIELD_UNDEFINED; }
  *val = value->val.BOOLEAN;
  return XCONF_SUCCESS;
}

uint32_t XCONF_get_list(XCONF *__conf, const char *__path, XCONFList **list) {
  Value *value = nullptr;
  uint32_t result = XCONF_get_path_value(__conf, __path, XCONF_PATH_ACTION_ACCESS, &value);
  if (result != XCONF_SUCCESS) { return result; }
  if (value->type != XCONF_VAL_UNINITIALIZED) { releaseValue(value, INSTANCE.allocator); }
  *list = value->val.LIST;
  return XCONF_SUCCESS;
}

uint32_t XCONF_get_object(XCONF *__conf, const char *__path, XCONFObject **object) {
  Value *value = nullptr;
  uint32_t result = XCONF_get_path_value(__conf, __path, XCONF_PATH_ACTION_ACCESS, &value);
  if (result != XCONF_SUCCESS) { return result; }
  if (value->type != XCONF_VAL_UNINITIALIZED) { releaseValue(value, INSTANCE.allocator); }
  *object = value->val.OBJECT;
  return XCONF_SUCCESS;
}

uint32_t XCONF_get_text(XCONF *__conf, const char *__path, const char **text, uint32_t *size) {
  Value *value = nullptr;
  uint32_t result = XCONF_get_path_value(__conf, __path, XCONF_PATH_ACTION_ACCESS, &value);
  if (result != XCONF_SUCCESS) { return result; }
  if (value->type != XCONF_VAL_UNINITIALIZED) { releaseValue(value, INSTANCE.allocator); }
  *text = value->val.TEXT->content;
  *size = value->val.TEXT->size;
  return XCONF_SUCCESS;
}

#define XCONFList_set(_type, _width, _field)                                                            \
  uint32_t XCONFList_set_##_type##_width(XCONFList *__list, uint32_t index, _type##_width##_t val) {    \
  Value *value = nullptr;                                                                               \
  uint32_t result = XCONFList_get_value(__list, index, &value);                                         \
  if (result != XCONF_SUCCESS) { return result; }                                                       \
  if (value->type != XCONF_VAL_UNINITIALIZED) { releaseValue(value, INSTANCE.allocator); }              \
                                                                                                        \
  value->type = XCONF_VAL_##_field##_width;                                                             \
  value->val._field##_width = val;                                                                      \
  value->size = _width / 8;                                                                             \
                                                                                                        \
  return XCONF_SUCCESS;                                                                                 \
}                                                                                                       \

XCONFList_set(int,  32, I)
XCONFList_set(int,  64, I)
XCONFList_set(int, 128, I)
XCONFList_set(int, 256, I)

XCONFList_set(uint,  32, U)
XCONFList_set(uint,  64, U)
XCONFList_set(uint, 128, U)
XCONFList_set(uint, 256, U)

XCONFList_set(float,  32, F)
XCONFList_set(float,  64, F)
XCONFList_set(float, 128, F)
XCONFList_set(float, 256, F)


uint32_t XCONFList_set_text(XCONFList *__list, uint32_t index, const char *text) {
  Value *value = nullptr;
  uint32_t result = XCONFList_get_value(__list, index, &value);
  if (result != XCONF_SUCCESS) { return result; }
  if (value->type != XCONF_VAL_UNINITIALIZED) { releaseValue(value, INSTANCE.allocator); }

  value->type = XCONF_VAL_TEXT;
  value->size = strlen(text);
  value->val.TEXT = Array_last_virt(INSTANCE.context->text_array) + 1;
  Array_append(INSTANCE.context->text_array, text, value->size + 1);

  return XCONF_SUCCESS;
}

uint32_t XCONFList_set_object(XCONFList *__list, uint32_t index, XCONFObject *object) {
  Value *value = nullptr;
  uint32_t result = XCONFList_get_value(__list, index, &value);
  if (result != XCONF_SUCCESS) { return result; }
  if (value->type != XCONF_VAL_UNINITIALIZED) { releaseValue(value, INSTANCE.allocator); }

  value->type = XCONF_VAL_LIST;
  value->val.OBJECT = object;
  value->size = 0;

  return XCONF_SUCCESS;
}

uint32_t XCONFList_set_list(XCONFList *__list, uint32_t index, XCONFList *list) {
  Value *value = nullptr;
  uint32_t result = XCONFList_get_value(__list, index, &value);
  if (result != XCONF_SUCCESS) { return result; }
  if (value->type != XCONF_VAL_UNINITIALIZED) { releaseValue(value, INSTANCE.allocator); }

  value->type = XCONF_VAL_LIST;
  value->val.LIST = list;
  value->size = 0;

  return XCONF_SUCCESS;
}

uint32_t XCONFList_getValueType(XCONFList *__list, uint32_t index, enum XCONF_VALUE_TYPE_ENUM *type) {
  Value *value = nullptr;
  uint32_t result = XCONFList_get_value(__list, index, &value);
  if (result != XCONF_SUCCESS) { return result; }

  *type = value->type;

  return XCONF_SUCCESS;
}

#define XCONFList_get(_type, _width, _field)                                                            \
  uint32_t XCONFList_get_##_type##_width(XCONFList *__list, uint32_t index, _type##_width##_t *val) {   \
  Value *value = nullptr;                                                                               \
  uint32_t result = XCONFList_get_value(__list, index, &value);                                         \
  if (result != XCONF_SUCCESS) { return result; }                                                       \
  if (value->type != XCONF_VAL_UNINITIALIZED) { releaseValue(value, INSTANCE.allocator); }              \
                                                                                                        \
  *val = value->val._field##_width;                                                                     \
                                                                                                        \
  return XCONF_SUCCESS;                                                                                 \
}                                                                                                       \

XCONFList_get(int,  32, I)
XCONFList_get(int,  64, I)
XCONFList_get(int, 128, I)
XCONFList_get(int, 256, I)

XCONFList_get(uint,  32, U)
XCONFList_get(uint,  64, U)
XCONFList_get(uint, 128, U)
XCONFList_get(uint, 256, U)

XCONFList_get(float,  32, F)
XCONFList_get(float,  64, F)
XCONFList_get(float, 128, F)
XCONFList_get(float, 256, F)

uint32_t XCONFList_get_list(XCONFList *__list, uint32_t index, XCONFList **list) {
  Value *value = nullptr;
  uint32_t result = XCONFList_get_value(__list, index, &value);
  if (result != XCONF_SUCCESS) { return result; }
  if (value->type != XCONF_VAL_UNINITIALIZED) { releaseValue(value, INSTANCE.allocator); }
  *list = value->val.LIST;
  return XCONF_SUCCESS;
}

uint32_t XCONFList_get_object(XCONFList *__list, uint32_t index, XCONFObject **object) {
  Value *value = nullptr;
  uint32_t result = XCONFList_get_value(__list, index, &value);
  if (result != XCONF_SUCCESS) { return result; }
  if (value->type != XCONF_VAL_UNINITIALIZED) { releaseValue(value, INSTANCE.allocator); }
  *object = value->val.OBJECT;
  return XCONF_SUCCESS;
}

uint32_t XCONFList_get_text(XCONFList *__list, uint32_t index, const char **text, uint32_t *size) {
  Value *value = nullptr;
  uint32_t result = XCONFList_get_value(__list, index, &value);
  if (result != XCONF_SUCCESS) { return result; }
  if (value->type != XCONF_VAL_UNINITIALIZED) { releaseValue(value, INSTANCE.allocator); }
  *text = value->val.TEXT->content;
  *size = value->val.TEXT->size;
  return XCONF_SUCCESS;
}

static uint32_t XCONF_get_path_value(XCONF *__conf, const char *__path, uint32_t action, Value **value) {
  INSTANCE.context->path_action = action;
  bool entered = false;
  if (INSTANCE.context->object != __conf) { XCONFContext_enter(INSTANCE.context, __conf); entered = true; }
  XCONFTokenizer_setSrc(INSTANCE.tokenizer, __path);
  Path *path = parsePath(INSTANCE.tokenizer, INSTANCE.context, &INSTANCE.errInfo, INSTANCE.allocator);
  if (entered) { XCONFContext_exit(INSTANCE.context); }
  if (!path) { return INSTANCE.errInfo.code; }
  *value = Array_virt2real(INSTANCE.context->value_array, path->value);
  return XCONF_SUCCESS;
}

static uint32_t XCONFList_get_value(XCONFList *__list, uint32_t index, Value **value) {
  REFER(Value) v_value = Array_real_addr(__list, index);
  if (!v_value) { return XCONF_ERROR_INDEX_OUT_OF_RANGE; }
  *value = Array_virt2real(INSTANCE.context->value_array, v_value);
  return XCONF_SUCCESS;
}

uint32_t XCONF_keys(XCONF *__conf, const char **keys, uint32_t *count) {
  *count = Array_length(__conf->pairs);
  if (!keys) { return XCONF_SUCCESS; }
  Pair *pairs = Array_first_real(__conf->pairs);
  for (uint32_t i = 0; i < *count; i++) {
    keys[i] = Array_virt2real(INSTANCE.context->key_array, pairs[i].key);
  }
  return XCONF_SUCCESS;
}

uint32_t XCONFList_count(XCONFList *__list, uint32_t *count) {
  *count = Array_length(__list);
  return XCONF_SUCCESS;
}

uint32_t XCONF_destroy_object(XCONFObject *object) {
  releaseObject(object, INSTANCE.allocator);
  INSTANCE.allocator->free(object);
  return XCONF_SUCCESS;
}
uint32_t XCONF_destroy_list(XCONFList *list) {
  releasePrimeArray(list);
  return XCONF_SUCCESS;
}
