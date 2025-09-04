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
 * Filename: test-parse.c
 * Creator: Yaokai Liu
 * Create Date: 2025-09-04
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include "xCONF/xCONF.h"
#include <check.h>

START_TEST(test_NUMBER_I32) {
    XCONF *conf = nullptr;
    char *string = "{number: 0x1008611}";
    uint32_t error_code = XCONF_ERROR_UNRECOGNIZED_SYMBOL;

    XCONF_init();

    error_code = XCONF_parse(string, &conf);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_ptr_ne(conf, nullptr);
    uint32_t count = 0;
    error_code = XCONF_keys(conf, nullptr, &count);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_uint_eq(count, 1);
    const char *keys[1] = {nullptr};
    error_code = XCONF_keys(conf, keys, &count);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_str_eq(keys[0], "number");
    enum XCONF_VALUE_TYPE_ENUM value_type = XCONF_VAL_UNINITIALIZED;
    error_code = XCONF_getValueType(conf, "number", &value_type);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_uint_eq(value_type, XCONF_VAL_I32);
    int32_t value = 0;
    XCONF_get_int32(conf, "number", &value);
    ck_assert_int_eq(value, 0x1008611);

    XCONF_finish();
}
END_TEST

START_TEST(test_NUMBER_I64) {
    XCONF *conf = nullptr;
    char *string = "{number = 0x1008611L}";
    uint32_t error_code = XCONF_ERROR_UNRECOGNIZED_SYMBOL;

    XCONF_init();

    error_code = XCONF_parse(string, &conf);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_ptr_ne(conf, nullptr);
    uint32_t count = 0;
    error_code = XCONF_keys(conf, nullptr, &count);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_uint_eq(count, 1);
    const char *keys[1] = {nullptr};
    error_code = XCONF_keys(conf, keys, &count);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_str_eq(keys[0], "number");
    enum XCONF_VALUE_TYPE_ENUM value_type = XCONF_VAL_UNINITIALIZED;
    error_code = XCONF_getValueType(conf, "number", &value_type);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_uint_eq(value_type, XCONF_VAL_I64);
    int64_t value = 0;
    XCONF_get_int64(conf, "number", &value);
    ck_assert_int_eq(value, 0x1008611);

    XCONF_finish();
}
END_TEST

START_TEST(test_NUMBER_I128) {
    XCONF *conf = nullptr;
    char *string = "{[number] = 0x1008611LL}";
    uint32_t error_code = XCONF_ERROR_UNRECOGNIZED_SYMBOL;

    XCONF_init();

    error_code = XCONF_parse(string, &conf);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_ptr_ne(conf, nullptr);
    uint32_t count = 0;
    error_code = XCONF_keys(conf, nullptr, &count);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_uint_eq(count, 1);
    const char *keys[1] = {nullptr};
    error_code = XCONF_keys(conf, keys, &count);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_str_eq(keys[0], "number");
    enum XCONF_VALUE_TYPE_ENUM value_type = XCONF_VAL_UNINITIALIZED;
    error_code = XCONF_getValueType(conf, "number", &value_type);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_uint_eq(value_type, XCONF_VAL_I128);
    int128_t value = 0;
    XCONF_get_int128(conf, "number", &value);
    ck_assert_int_eq(value, 0x1008611);

    XCONF_finish();
}
END_TEST

START_TEST(test_NUMBER_U32) {
    XCONF *conf = nullptr;
    char *string = "{[number]: 0x1008611U}";
    uint32_t error_code = XCONF_ERROR_UNRECOGNIZED_SYMBOL;

    XCONF_init();

    error_code = XCONF_parse(string, &conf);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_ptr_ne(conf, nullptr);
    uint32_t count = 0;
    error_code = XCONF_keys(conf, nullptr, &count);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_uint_eq(count, 1);
    const char *keys[1] = {nullptr};
    error_code = XCONF_keys(conf, keys, &count);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_str_eq(keys[0], "number");
    enum XCONF_VALUE_TYPE_ENUM value_type = XCONF_VAL_UNINITIALIZED;
    error_code = XCONF_getValueType(conf, "number", &value_type);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_uint_eq(value_type, XCONF_VAL_U32);
    uint32_t value = 0;
    XCONF_get_uint32(conf, "number", &value);
    ck_assert_int_eq(value, 0x1008611);

    XCONF_finish();
}
END_TEST

START_TEST(test_NUMBER_U64) {
    XCONF *conf = nullptr;
    char *string = "{[number] 0x1008611LU}";
    uint32_t error_code = XCONF_ERROR_UNRECOGNIZED_SYMBOL;

    XCONF_init();

    error_code = XCONF_parse(string, &conf);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_ptr_ne(conf, nullptr);
    uint32_t count = 0;
    error_code = XCONF_keys(conf, nullptr, &count);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_uint_eq(count, 1);
    const char *keys[1] = {nullptr};
    error_code = XCONF_keys(conf, keys, &count);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_str_eq(keys[0], "number");
    enum XCONF_VALUE_TYPE_ENUM value_type = XCONF_VAL_UNINITIALIZED;
    error_code = XCONF_getValueType(conf, "number", &value_type);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_uint_eq(value_type, XCONF_VAL_U64);
    uint64_t value = 0;
    XCONF_get_uint64(conf, "number", &value);
    ck_assert_int_eq(value, 0x1008611);

    XCONF_finish();
}
END_TEST

START_TEST(test_NUMBER_U128) {
    XCONF *conf = nullptr;
    char *string = "{.number = 0x1008611LLU}";
    uint32_t error_code = XCONF_ERROR_UNRECOGNIZED_SYMBOL;

    XCONF_init();

    error_code = XCONF_parse(string, &conf);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_ptr_ne(conf, nullptr);
    uint32_t count = 0;
    error_code = XCONF_keys(conf, nullptr, &count);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_uint_eq(count, 1);
    const char *keys[1] = {nullptr};
    error_code = XCONF_keys(conf, keys, &count);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_str_eq(keys[0], "number");
    enum XCONF_VALUE_TYPE_ENUM value_type = XCONF_VAL_UNINITIALIZED;
    error_code = XCONF_getValueType(conf, "number", &value_type);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_uint_eq(value_type, XCONF_VAL_U128);
    uint128_t value = 0;
    XCONF_get_uint128(conf, "number", &value);
    ck_assert_int_eq(value, 0x1008611);

    XCONF_finish();
}
END_TEST

Suite *number_suite() {
    Suite *suite = suite_create("test-parse");
    TCase *tc_number = tcase_create("number");
    tcase_add_test(tc_number, test_NUMBER_I32);
    tcase_add_test(tc_number, test_NUMBER_I64);
    tcase_add_test(tc_number, test_NUMBER_I128);
    tcase_add_test(tc_number, test_NUMBER_U32);
    tcase_add_test(tc_number, test_NUMBER_U64);
    tcase_add_test(tc_number, test_NUMBER_U128);
    suite_add_tcase(suite, tc_number);
    return suite;
}

