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
 * Module Name: test
 * Filename: debug.c
 * Creator: Yaokai Liu
 * Create Date: 2025-06-08
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include <stdio.h>

#include "meman-utils.h"
#include "xCONF/xCONF.h"

#define BUFFER_SIZE 1024

int main () {
  char buffer[BUFFER_SIZE];
  const Allocator *allocator = &STDAllocator;
  Trie *key_trie = Trie_new(sizeof(char), (key_t *) char2u64, allocator);
  Array *key_array = Array_new(sizeof(char),1, allocator);
  XCONFInstance *instance = XCONF_new(key_trie, key_array, allocator);

  // const char *string = "{data[0].a = 0x81363.4p238LL, data[1].b = 3.1415926535L, data[2].what[0].the.heil.is[0].that: ['I don\\'t know.']}";
  // if (XCONF_parse(string, &conf) != XCONF_SUCCESS) { XCONF_finish(); return -1; }
  // if (XCONF_compose(conf, buffer, 512) != XCONF_SUCCESS) { XCONF_finish(); return -2; }
  // printf("%s\n", buffer);

  XCONFObject *conf = nullptr;
  enum XCONF_ERROR_CODE_ENUM result;
  result = XCONF_load(instance, "test.xconf", &conf);
  if (result != XCONF_SUCCESS) {
    XCONF_destroy(instance); return -1;
  }
  result = XCONF_compose(instance, conf, true, buffer, BUFFER_SIZE);
  if (result != XCONF_SUCCESS) { XCONF_destroy(instance); return -2; }
  printf("%s\n", buffer);

  // XCONFObject_destroy(conf);
  XCONF_destroy(instance);
  return 0;
}
