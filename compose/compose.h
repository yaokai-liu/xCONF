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
 * Module Name: compose
 * Filename: compose.h
 * Creator: Yaokai Liu
 * Create Date: 2025-06-12
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#ifndef XCONF_COMPOSE_H
#define XCONF_COMPOSE_H

#include <stdint.h>
#include <stdio.h>
#include "xCONF/target.h"
#include "xCONF/context.h"

uint32_t writeValue(FILE *file, const Value *value, REFER(const Value) v_parent, bool paired, uint32_t indent, XCONFContext *context);

uint32_t writeList(FILE *file, const List *list, REFER(const Value) v_current, bool pathed, uint32_t indent,
                   XCONFContext *context);

uint32_t writeObject(FILE *file, Object *object, REFER(const Value) v_current, bool pathed, uint32_t indent,
                     XCONFContext *context);
uint32_t writeCompactPair(FILE *file, const REFER(Value) v_current, const REFER(Value) v_parent, uint32_t indent, XCONFContext *context);
uint32_t writePath(FILE * file, const Value * value, XCONFContext * context);
#endif //XCONF_COMPOSE_H
