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
 * Filename: test.c
 * Creator: Yaokai Liu
 * Create Date: 2025-09-04
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include "testcases/testcases.h"
#include "xCONF/xCONF.h"
#include <check.h>

int main() {
    XCONF_init();
    SRunner *srunner = srunner_create(nullptr);
    srunner_add_suite(srunner, parse_suite());
    srunner_set_fork_status(srunner, CK_NOFORK);
    srunner_run_all(srunner, CK_NORMAL);
    int n = srunner_ntests_failed(srunner);
    srunner_free(srunner);
    XCONF_finish();
    return n ? -1 : 0;
}
