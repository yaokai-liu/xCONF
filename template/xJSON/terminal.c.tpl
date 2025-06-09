${license}

#include "xJSON/token.h"
#include "generated/tokens.gen.h"
const char_t XJSON_TERMINALS[] = {
  ${terminals}
};

const char_t * XJSON_TERMINAL_STRINGS[] = {
  ${strings}
};

const uint32_t XJSON_TERMINAL_STRING_LENS[] = {
  ${string_lens}
};

const int32_t N_JSON_TERMINAL = sizeof(XJSON_TERMINALS);
