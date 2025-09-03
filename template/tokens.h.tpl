${license}

#ifndef XCONF_TOKENS_H
#define XCONF_TOKENS_H

#include "char_t.h"
#include <stdint.h>

enum TOKEN_TYPE_ENUM: uint32_t {
  ${enums},
  XCONF_TOKEN_PLACE_HOLDER,
  XCONF_MAX_TOTAL_TOKEN,
  XCONF_TOKEN_BAD_TOKEN = UINT32_MAX
};
extern const char_t *TOKEN_NAMES[];
#endif  // XCONF_TOKENS_H
