${license}

#ifndef XJSON_RULES_H
#define XJSON_RULES_H

#include "xJSON/target.h"
#include "xJSON/context.h"

typedef void *fn_xjson_reduce(Token argv[], XJSONContext *, ErrInfo *, const Allocator *);

enum XJSON_PRODUCT_RULE_ENUM {
  ${enum_reduces}
};

extern fn_xjson_reduce * const XJSON_PRODUCTS[];

${reduces}

#endif  // XJSON_RULES_H
