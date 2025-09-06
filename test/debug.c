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
#include "xCONF/xCONF.h"

int main () {
  XCONF_init();

  const char *string = "{data[0].a[0].b = 0x81363.4p238LL}";
  XCONF *conf = nullptr;
  if (XCONF_parse(string, &conf) != XCONF_SUCCESS) { XCONF_finish(); return -1; }

  char buffer[256];
  XCONF_compose(conf, buffer, 256);

  printf("%s\n", buffer);

  XCONF_finish();
  return 0;
}
