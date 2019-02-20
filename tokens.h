#ifndef TOKEN_H
#define TOKEN_H

#include "svec.h"

char* read_token(char* text, int i);
int get_op_size(char* text, int i);
int is_op(char c);
svec* tokenize(char* text);

#endif
