${license}

#ifndef XCONF_RULES_H
#define XCONF_RULES_H

#include "xCONF/target.h"
#include "xCONF/context.h"

typedef void *fn_xconf_reduce(Token argv[], XCONFContext *, ErrInfo *, const Allocator *);

enum XCONF_PRODUCT_RULE_ENUM {
  ${enum_reduces}
};

extern fn_xconf_reduce * const XCONF_PRODUCTS[];

${reduces}

#endif  // XCONF_RULES_H
