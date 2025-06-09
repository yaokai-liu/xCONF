${license}

#ifndef XJSON_TOKENS_H
#define XJSON_TOKENS_H

#include "xJSON/char_t.h"
#include <stdint.h>

enum TOKEN_TYPE_ENUM: uint32_t {
  ${enums},
  XJSON_TOKEN_PLACE_HOLDER,
  XJSON_MAX_TOTAL_TOKEN,
  XJSON_TOKEN_BAD_TOKEN = UINT32_MAX
};
extern const char_t *TOKEN_NAMES[];
#endif  // XJSON_TOKENS_H
