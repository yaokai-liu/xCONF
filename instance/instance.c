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
 * Module Name: instance
 * Filename: instance.c
 * Creator: Yaokai Liu
 * Create Date: 2025-06-11
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/
#include "instance.h"
#include "xCONF/parse.h"
#include "xCONF/xCONF.h"
#include "allocator.h"
#include "compose.h"
#include "Path/parse.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stddef.h>

XCONFInstance *XCONF_new(Trie *key_trie, Array *key_array, const Allocator *allocator) {
  XCONFInstance *instance = allocator->calloc(1, sizeof(XCONFInstance));
  instance->allocator = &STDAllocator;
  instance->key_trie = key_trie;
  instance->key_array = key_array;
  instance->context = XCONFContext_new(instance, instance->allocator);
  instance->tokenizer = XCONFTokenizer_new(instance, instance->allocator);
  return instance;
}

uint32_t XCONF_reset(const XCONFInstance *instance) {
  XCONFTokenizer_enter(instance->tokenizer, nullptr);
  XCONFContext_reset(instance->context);
  return XCONF_SUCCESS;
}

uint32_t XCONF_clear(const XCONFInstance *instance) {
  XCONFContext_reset(instance->context);
  XCONFContext_clear(instance->context);
  XCONFTokenizer_enter(instance->tokenizer, nullptr);
  return XCONF_SUCCESS;
}

void XCONF_destroy(XCONFInstance *instance) {
  XCONFContext_destroy(instance->context);
  XCONFTokenizer_destroy(instance->tokenizer);
  instance->allocator->free(instance);
}

uint32_t XCONF_load(XCONFInstance *instance, const char *filepath, XCONFObject **conf) {
  FILE *file = fopen(filepath, "r");
  if (!file) { return XCONF_ERROR_NO_SUCH_FILE; }
  fseek(file, 0, SEEK_END);
  uint32_t size = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *buffer = malloc((size + 1) * sizeof(char));
  fread(buffer, sizeof(char), size, file);
  buffer[size] = '\0';

  uint32_t result = XCONF_parse(instance, buffer, conf);

  free(buffer);
  fclose(file);

  return result;
}

uint32_t XCONF_parse(XCONFInstance *instance, const char *string, XCONFObject **conf) {
  uint32_t path_action = instance->context->path_action;
  XCONFTokenizer_enter(instance->tokenizer, string);
  instance->context->path_action = XCONF_PATH_ACTION_BUILD;
  XCONFContext_enter(instance->context, nullptr);
  Object *object = parse(instance->tokenizer, instance->context, &instance->errInfo, instance->allocator);
  XCONFContext_exit(instance->context);
  instance->context->path_action = path_action;
  XCONFTokenizer_exit(instance->tokenizer);
  if (!object) { return instance->errInfo.code; }
  *conf = object;
  return XCONF_SUCCESS;
}

uint32_t XCONF_dump(const XCONFInstance *instance, XCONFObject *conf, bool compact, const char *filepath) {
  FILE *file = fopen(filepath, "wb");
  writeObject(file, conf, nullptr, compact, compact * 2, instance->context);
  fclose(file);
  return XCONF_SUCCESS;
}

uint32_t XCONF_compose(XCONFInstance *instance, XCONFObject *conf, bool compact, char *buffer, uint32_t buffer_size) {
  FILE *file = tmpfile();
  writeObject(file, conf, nullptr, compact, compact * 2, instance->context);
  fseek(file, 0, SEEK_END);
  uint32_t size = ftell(file);
  fseek(file, 0, SEEK_SET);
  if (size > buffer_size) { fclose(file); return XCONF_ERROR_OVER_BUFFER_SIZE; }
  fread(buffer, sizeof(char), size, file);
  buffer[size] = '\0';
  fclose(file);
  return XCONF_SUCCESS;
}
