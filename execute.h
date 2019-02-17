#ifndef EXECUTE_H
#define EXECUTE_H

#include "ast.h"

int execute(ast* ast);
int execute_redirect_input(ast* ast);
int execute_redirect_output(ast* ast);
int execute_pipe(ast* ast);
int execute_background(ast* ast);
int execute_and(ast* ast);
int execute_or(ast* ast);
int execute_semicolon(ast* ast);

#endif
