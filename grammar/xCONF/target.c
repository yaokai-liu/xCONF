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
 * Module Name: grammar
 * Filename: target.c
 * Creator: Yaokai Liu
 * Create Date: 2025-06-07
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include "target.h"
#include "context.h"

Object *Object_new(const Allocator *allocator) {
  Object *object = allocator->calloc(1, sizeof(Object));
  object->pairs = Array_new(sizeof(Pair), XCONF_PAIR_ARRAY, allocator);
  object->mapping = AVLTree_new(allocator, nullptr);
  return object;
}

void releaseValue(Value *val, const Allocator *allocator) {
  switch (val->type) {
    case XCONF_VAL_LIST: { return releaseList(val->val.LIST, allocator); }
    case XCONF_VAL_OBJECT: { return releaseObject(val->val.OBJECT, allocator); }
    default:{}
  }
}

void releasePair(Pair *, const Allocator *) {
  /* nothing to do */
}

void releaseWrapperedText(WrapperedText *text, const Allocator *allocator) {
  if (text->content) { allocator->free(text->content); }
  text->content = nullptr;
  text->length = 0;
  text->n_pred = 0;
  text->n_succ = 0;
}

void releaseTexts(Texts *, const Allocator *) {
  /* nothing to do */
}

void releasePath(Path *, const Allocator *) {
  /* nothing to do */
}

void releaseObject(Object *object, const Allocator *) {
  releasePrimeArray(object->pairs);
  AVLTree_destroy(object->mapping, nullptr);
}

void releaseList(List *list, const Allocator *) {
  Array_reset(list, nullptr);
}
