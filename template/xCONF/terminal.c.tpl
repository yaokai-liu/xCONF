${license}

#include "token.h"
#include "generated/tokens.gen.h"
const char_t XCONF_TERMINALS[] = {
  ${terminals}
};

const char_t * XCONF_TERMINAL_STRINGS[] = {
  ${strings}
};

const uint32_t XCONF_TERMINAL_STRING_LENS[] = {
  ${string_lens}
};

const int32_t N_JSON_TERMINAL = sizeof(XCONF_TERMINALS);
