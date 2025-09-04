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
 * Filename: error.h
 * Creator: Yaokai Liu
 * Create Date: 2025-09-03
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#ifndef XCONF_GRAMMAR_ERROR_INFO_H
#define XCONF_GRAMMAR_ERROR_INFO_H

#include <stdint.h>
typedef struct ErrInfo {
  /**
   * @description offset in src string
   */
  uint32_t offset;
  /**
   * @description line number in src file
   */
  uint32_t lineno;
  /**
   * @description column offset in the line in src file
   */
  uint32_t column;
  /**
   * @description error code
   */
  uint32_t code;
  /**
   * @description   file that error occurs
   */
  const char * file;
  /**
   * @description   the token type
   */
  uint32_t token;
} ErrInfo;


#endif //XCONF_GRAMMAR_ERROR_INFO_H
