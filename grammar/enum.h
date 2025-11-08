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
 * Module Name: grammar/xCONF
 * Filename: enum.h
 * Creator: Yaokai Liu
 * Create Date: 2025-11-03
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#ifndef XCONF_ENUM_H
#define XCONF_ENUM_H

enum XCONF_UPDATE_METHOD_ENUM: uint32_t {
  XCONF_UM_UPDATE,
  XCONF_UM_APPEND,
  XCONF_UM_REMOVE,
};

enum XCONF_PATH_ACTION_TYPE: uint32_t {
  XCONF_PATH_ACTION_UNSET,
  XCONF_PATH_ACTION_BUILD,
  XCONF_PATH_ACTION_ACCESS,
};

enum XCONF_CONTEXT_ARRAY_AND_DICT_ID: uint32_t {
  XCONF_KEY_ARRAY = 1,
  XCONF_TEXT_ARRAY = 2,
  XCONF_VALUE_ARRAY = 3,
  XCONF_PAIR_ARRAY = 4,
  XCONF_REFER_ARRAY = 5,
  XCONF_REFER_VALUE_ARRAY = 5,
  XCONF_OBJECT_ID = 6,
};

enum XCONF_VALUE_CATEGORY_ENUM: uint32_t {
  XCONF_VAL_CAT_NULL,
  XCONF_VAL_CAT_LIST,
  XCONF_VAL_CAT_TEXT,
  XCONF_VAL_CAT_OBJECT,
  XCONF_VAL_CAT_BOOLEAN,

  XCONF_VAL_CAT_INT,
  XCONF_VAL_CAT_UINT,
  XCONF_VAL_CAT_FLOAT,

  XCONF_VAL_CAT_UNINITIALIZED = UINT32_MAX,
};

#endif //XCONF_ENUM_H