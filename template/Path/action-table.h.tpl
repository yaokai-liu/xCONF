${license}

#ifndef XCONF_PATH_ACTION_TABLE_GEN_H
#define XCONF_PATH_ACTION_TABLE_GEN_H

#include <stdint.h>

enum XCONF_PATH_STATE_ENUM {
  XCONF_Path_BAD_STATE = 0,
  ${state_enum}
};

const struct grammar_action *getParsePathAction(uint32_t index, uint32_t ahead);

uint32_t parsePathJumpState(uint32_t index, uint32_t current);

uint32_t getParsePathStateCurrentTokenType(int32_t state);

#endif  // XCONF_PATH_ACTION_TABLE_GEN_H