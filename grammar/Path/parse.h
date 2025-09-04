/* License
 *
 * ${PROJECT_DESCRIPTION}
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
 * Module Name: grammar/Path
 * Filename: parse.h
 * Creator: Yaokai Liu
 * Create Date: 2025-09-04
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#ifndef XCONF_GRAMMAR_PATH_PARSE_H
#define XCONF_GRAMMAR_PATH_PARSE_H

#include "tokenize/tokenizer.h"
#include "xCONF/context.h"
#include "xCONF/target.h"

Path *parsePath(XCONFTokenizer *tokenizer, XCONFContext *context, ErrInfo *errInfo, const Allocator *allocator);


#endif //XCONF_GRAMMAR_PATH_PARSE_H
