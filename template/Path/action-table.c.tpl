${license}

#include "action.h"
#include "generated/tokens.gen.h"
#include "generated/Path/rules.gen.h"
#include "generated/Path/action-table.gen.h"

struct state {
  const uint16_t ndx_base;
  const uint16_t goto_base;
  const uint16_t token_base;
  const uint16_t n_tokens;
};
struct unit {
  uint8_t type;
  uint8_t offset;
};

const struct grammar_action XCONF_PATH_ACTIONS[];
const uint16_t XCONF_PATH_JUMPS[];
const struct unit XCONF_PATH_UNITS[];
const struct state XCONF_PATH_STATES[];
const uint32_t XCONF_PATH_CURRENT_TOKENS[];

const struct unit *getParsePathUnit(const state *state, uint32_t look);

const struct grammar_action XCONF_PATH_ACTIONS[] = {
  ${actions}
};

const uint16_t XCONF_PATH_JUMPS[] = {
  ${jumps}
};

const struct unit XCONF_PATH_UNITS[] = {
  ${units}
};

const struct state XCONF_PATH_STATES[] = {
  ${states}
};

const uint32_t XCONF_PATH_CURRENT_TOKENS[] = {
  ${currents}
};

inline const struct unit *getParsePathUnit(const state *state, uint32_t look) {
  const struct unit *unit, *base = &XCONF_PATH_UNITS[state->token_base];
  int32_t left = 0, right = state->n_tokens - 1;
  int32_t u_idx = (left + right) / 2;
  unit = &base[u_idx];
  while (unit->type != look && left < right) {
    if (unit->type < look) {
      left = u_idx + 1;
    } else {
      right = u_idx - 1;
    }
    u_idx = (left + right) / 2;
    unit = &base[u_idx];
  }
  if (unit->type != look) { return nullptr; }
  return unit;
}


inline const struct grammar_action *getParsePathAction(uint32_t index, uint32_t ahead) {
    const state *state = &XCONF_PATH_STATES[index];
    const struct unit *unit = getParsePathUnit(state, ahead);
    if (!unit) { return nullptr; }
    const struct grammar_action *act = &XCONF_PATH_ACTIONS[state->ndx_base + unit->offset];
    return act;
}

inline uint32_t parsePathJumpState(uint32_t index, uint32_t current) {
    const state *state = &XCONF_PATH_STATES[index];
    const struct unit *unit = getParsePathUnit(state, current);
    if (!unit) { return XCONF_Path_BAD_STATE; }
    return XCONF_PATH_JUMPS[state->goto_base + unit->offset];
}

inline uint32_t getParsePathStateCurrentTokenType(int32_t state) {
    return XCONF_PATH_CURRENT_TOKENS[state];
}