${license}

#ifndef XCONF_PATH_RULES_H
#define XCONF_PATH_RULES_H

#include "xCONF/target.h"
#include "xCONF/context.h"

typedef void *fn_xconf_path_reduce(Token argv[], XCONFContext *, ErrInfo *, const Allocator *);

enum XCONF_PATH_PRODUCT_RULE_ENUM {
  ${enum_reduces}
};

extern fn_xconf_path_reduce * const XCONF_PATH_PRODUCTS[];

${reduces}

#endif  // XCONF_PATH_RULES_H
