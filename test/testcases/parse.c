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
 * Filename: parse.c
 * Creator: Yaokai Liu
 * Create Date: 2025-09-04
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include "xCONF/xCONF.h"
#include <check.h>

START_TEST(test_NUMBER_I32) {
    XCONFObject *conf = nullptr;
    char *string = "{number: 0x1008611}";
    uint32_t error_code = XCONF_ERROR_UNRECOGNIZED_SYMBOL;

    XCONF_reset();
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
    error_code = XCONFObject_getValueType(conf, "number", &value_type);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_uint_eq(value_type, XCONF_VAL_I32);
    int32_t value = 0;
    XCONFObject_get_int32(conf, "number", &value);
    ck_assert_int_eq(value, 0x1008611);
}
END_TEST

START_TEST(test_NUMBER_I64) {
    XCONFObject *conf = nullptr;
    char *string = "{number = 0x1008611L}";
    uint32_t error_code = XCONF_ERROR_UNRECOGNIZED_SYMBOL;

    XCONF_reset();
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
    error_code = XCONFObject_getValueType(conf, "number", &value_type);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_uint_eq(value_type, XCONF_VAL_I64);
    int64_t value = 0;
    XCONFObject_get_int64(conf, "number", &value);
    ck_assert_int_eq(value, 0x1008611);
}
END_TEST

START_TEST(test_NUMBER_I128) {
    XCONFObject *conf = nullptr;
    char *string = "{[number] = 0x1008611LL}";
    uint32_t error_code = XCONF_ERROR_UNRECOGNIZED_SYMBOL;

    XCONF_reset();
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
    error_code = XCONFObject_getValueType(conf, "number", &value_type);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_uint_eq(value_type, XCONF_VAL_I128);
    int128_t value = 0;
    XCONFObject_get_int128(conf, "number", &value);
    ck_assert_int_eq(value, 0x1008611);
}
END_TEST

START_TEST(test_NUMBER_U32) {
    XCONFObject *conf = nullptr;
    char *string = "{[number]: 0x1008611U}";
    uint32_t error_code = XCONF_ERROR_UNRECOGNIZED_SYMBOL;

    XCONF_reset();
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
    error_code = XCONFObject_getValueType(conf, "number", &value_type);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_uint_eq(value_type, XCONF_VAL_U32);
    uint32_t value = 0;
    XCONFObject_get_uint32(conf, "number", &value);
    ck_assert_int_eq(value, 0x1008611);
}
END_TEST

START_TEST(test_NUMBER_U64) {
    XCONFObject *conf = nullptr;
    char *string = "{[number] 0x1008611LU}";
    uint32_t error_code = XCONF_ERROR_UNRECOGNIZED_SYMBOL;

    XCONF_reset();
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
    error_code = XCONFObject_getValueType(conf, "number", &value_type);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_uint_eq(value_type, XCONF_VAL_U64);
    uint64_t value = 0;
    XCONFObject_get_uint64(conf, "number", &value);
    ck_assert_int_eq(value, 0x1008611);
}
END_TEST

START_TEST(test_NUMBER_U128) {
    XCONFObject *conf = nullptr;
    char *string = "{.number = 0x1008611LLU}";
    uint32_t error_code = XCONF_ERROR_UNRECOGNIZED_SYMBOL;

    XCONF_reset();
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
    error_code = XCONFObject_getValueType(conf, "number", &value_type);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_uint_eq(value_type, XCONF_VAL_U128);
    uint128_t value = 0;
    XCONFObject_get_uint128(conf, "number", &value);
    ck_assert_int_eq(value, 0x1008611);
}
END_TEST

START_TEST(test_KEYWORD_null) {
    XCONFObject *conf = nullptr;
    char *string = "{keyword-null = null, keyword-NULL: NULL}";
    uint32_t error_code = XCONF_ERROR_UNRECOGNIZED_SYMBOL;
    enum XCONF_VALUE_TYPE_ENUM value_type = XCONF_VAL_UNINITIALIZED;

    XCONF_reset();
    error_code = XCONF_parse(string, &conf);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_ptr_ne(conf, nullptr);
    uint32_t count = 0;
    error_code = XCONF_keys(conf, nullptr, &count);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_uint_eq(count, 2);
    const char *keys[2] = {nullptr, nullptr};
    error_code = XCONF_keys(conf, keys, &count);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_str_eq(keys[0], "keyword-null");
    ck_assert_str_eq(keys[1], "keyword-NULL");
    value_type = XCONF_VAL_UNINITIALIZED;
    error_code = XCONFObject_getValueType(conf, "keyword-null", &value_type);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_uint_eq(value_type, XCONF_VAL_NULL);
    value_type = XCONF_VAL_UNINITIALIZED;
    error_code = XCONFObject_getValueType(conf, "keyword-NULL", &value_type);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_uint_eq(value_type, XCONF_VAL_NULL);
}
END_TEST


START_TEST(test_KEYWORD_boolean) {
    XCONFObject *conf = nullptr;
    char *string = "{keyword_true = true, keyword-TRUE: TRUE, [keyword-false] false, [keyword_FALSE] = FALSE}";
    uint32_t error_code = XCONF_ERROR_UNRECOGNIZED_SYMBOL;
    enum XCONF_VALUE_TYPE_ENUM value_type = XCONF_VAL_UNINITIALIZED;
    bool value;

    XCONF_reset();
    error_code = XCONF_parse(string, &conf);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_ptr_ne(conf, nullptr);
    uint32_t count = 0;
    error_code = XCONF_keys(conf, nullptr, &count);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_uint_eq(count, 4);
    const char *keys[4] = {nullptr, nullptr, nullptr, nullptr};
    error_code = XCONF_keys(conf, keys, &count);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_str_eq(keys[0], "keyword_true");
    ck_assert_str_eq(keys[1], "keyword-TRUE");
    ck_assert_str_eq(keys[2], "keyword-false");
    ck_assert_str_eq(keys[3], "keyword_FALSE");

    value_type = XCONF_VAL_UNINITIALIZED;
    error_code = XCONFObject_getValueType(conf, "keyword_true", &value_type);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_uint_eq(value_type, XCONF_VAL_BOOLEAN);
    value = 0;
    error_code = XCONFObject_get_bool(conf, "keyword_true", &value);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_int_eq(value, true);

    value_type = XCONF_VAL_UNINITIALIZED;
    error_code = XCONFObject_getValueType(conf, "keyword-TRUE", &value_type);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_uint_eq(value_type, XCONF_VAL_BOOLEAN);
    value = 0;
    error_code = XCONFObject_get_bool(conf, "keyword-TRUE", &value);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_int_eq(value, true);

    value_type = XCONF_VAL_UNINITIALIZED;
    error_code = XCONFObject_getValueType(conf, "keyword-false", &value_type);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_uint_eq(value_type, XCONF_VAL_BOOLEAN);
    value = 1;
    error_code = XCONFObject_get_bool(conf, "keyword-false", &value);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_int_eq(value, false);

    value_type = XCONF_VAL_UNINITIALIZED;
    error_code = XCONFObject_getValueType(conf, "keyword_FALSE", &value_type);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_uint_eq(value_type, XCONF_VAL_BOOLEAN);
    value = 1;
    error_code = XCONFObject_get_bool(conf, "keyword_FALSE", &value);
    ck_assert_uint_eq(error_code, XCONF_SUCCESS);
    ck_assert_int_eq(value, false);
}
END_TEST


Suite *parse_suite() {
    Suite *suite = suite_create("test-parse");
    TCase *tc_number = tcase_create("number");
    tcase_add_test(tc_number, test_NUMBER_I32);
    tcase_add_test(tc_number, test_NUMBER_I64);
    tcase_add_test(tc_number, test_NUMBER_I128);
    tcase_add_test(tc_number, test_NUMBER_U32);
    tcase_add_test(tc_number, test_NUMBER_U64);
    tcase_add_test(tc_number, test_NUMBER_U128);
    tcase_add_test(tc_number, test_KEYWORD_null);
    tcase_add_test(tc_number, test_KEYWORD_boolean);
    suite_add_tcase(suite, tc_number);
    return suite;
}

