#ifndef CALC_PARSE_H
#define CALC_PARSE_H

#include "ast.h"
#include "svec.h"

int streq(const char* aa, const char* bb);
ast* parse_cmd(svec* tokens);

#endif

