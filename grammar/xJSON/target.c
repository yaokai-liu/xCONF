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
 * Module Name: grammar
 * Filename: target.c
 * Creator: Yaokai Liu
 * Create Date: 2025-06-07
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include "target.h"


void releaseValue(Value *val, const Allocator *allocator) {
  switch (val->type) {
    case XJSON_VAL_LIST: { return releaseList(val->val.LIST, allocator); }
    case XJSON_VAL_OBJECT: { return releaseObject(val->val.OBJECT, allocator); }
    default:{}
  }
}

void releasePair(Pair *pair, const Allocator *allocator) {
  releaseValue(pair->value, allocator);
}

void releaseText(Text *text, const Allocator *allocator) {
  if (text->content) { allocator->free(text->content); }
  text->content = nullptr;
  text->n_pred = 0;
  text->n_succ = 0;
}

void releaseTexts(Texts *, const Allocator *) {
  /* nothing to do */
}

void releasePathKey(PathKey *, const Allocator *) {
  /* nothing to do */
}

void releaseObject(Object *object, const Allocator *allocator) {
  allocator->free(object->keys);
  allocator->free(object->values);
}

void releaseList(List *list, const Allocator *allocator) {
  allocator->free(list->values);
}
