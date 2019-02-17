#ifndef EXECUTE_H
#define EXECUTE_H

#include "ast.h"

void execute(ast* ast);
void execute_redirect_input(ast* ast);
void execute_redirect_output(ast* ast);
void execute_pipe(ast* ast);
void execute_background(ast* ast);
void execute_and(ast* ast);
void execute_or(ast* ast);
void execute_semicolon(ast* ast);

#endif
