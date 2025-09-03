/* License
 *
 * xCONF - C Library to Parse xCONF to C
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
 * Module Name: driver
 * Filename: driver.c
 * Creator: Yaokai Liu
 * Create Date: 2025-06-11
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include "driver.h"
#include "xCONF/context.h"
#include "xCONF/xCONF.h"
#include "allocator.h"
#include "tokenize/tokenizer.h"
#include "xCONF/parse.h"
#include "compose.h"
#include <stdio.h>
#include <malloc.h>

static struct XCONFInstance {
  const Allocator *allocator;
  XCONFTokenizer *tokenizer;
  XCONFContext *context;
  ErrInfo errInfo;
} INSTANCE = {.context = nullptr};

uint32_t XCONF_init() {
  INSTANCE.tokenizer = XCONFTokenizer_new(nullptr, INSTANCE.context->key_array,
                                          INSTANCE.context->key_trie, INSTANCE.context->allocator);
  INSTANCE.context = XCONFContext_new(&STDAllocator);
  return XCONF_SUCCESS;
}
uint32_t XCONF_finish(XCONFInstance *instance) {
  XCONFContext_destroy(instance->context);
  XCONFTokenizer_destroy(instance->tokenizer);
  return XCONF_SUCCESS;
}

uint32_t XCONF_load(const char *__filepath, XCONF **__json) {
  FILE *file = fopen(__filepath, "r");
  if (!file) { return XCONF_ERROR_NO_SUCH_FILE; }
  fseek(file, 0, SEEK_END);
  uint32_t size = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *buffer = malloc(size * sizeof(char));
  fread(buffer, sizeof(char), size, file);

  uint32_t result = XCONF_parse(buffer, __json);

  free(buffer);
  fclose(file);

  return result;
}

uint32_t XCONF_parse(const char *__string, XCONF **__json) {
  XCONFTokenizer_setSrc(INSTANCE.tokenizer, __string);
  XCONF *json = parse(INSTANCE.tokenizer, INSTANCE.context, &INSTANCE.errInfo, INSTANCE.allocator);
  if (!json) { return INSTANCE.errInfo.code; }
  *__json = json;
  return XCONF_SUCCESS;
}

uint32_t XCONF_dump(XCONF *__json, const char *__filepath) {
  FILE *file = fopen(__filepath, "wb");
  uint32_t result = writeObject(file, __json, 0, INSTANCE.context);
  fclose(file);
  return result;
}

uint32_t XCONF_compose(XCONF *__json, char *buffer, uint32_t buffer_size) {
  FILE *file = tmpfile();
  uint32_t result = writeObject(file, __json, 0, INSTANCE.context);
  if (result != XCONF_SUCCESS) { return result; }
  fseek(file, 0, SEEK_END);
  uint32_t size = ftell(file);
  if (size > buffer_size) { return XCONF_ERROR_OVER_BUFFER_SIZE; }
  fread(buffer, sizeof(char), size, file);
  fclose(file);
  return XCONF_SUCCESS;
}
