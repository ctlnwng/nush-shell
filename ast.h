#ifndef AST_H
#define AST_H

#include <stdlib.h>

typedef struct ast {
    char* op; // op is one of: >, <, ;, &&, ||, &, |
    struct ast* arg0;
    struct ast* arg1;
    char** cmd;
    int cmd_len;
} ast;

ast* make_ast_cmd(char** cmd, int cmd_len);
ast* make_ast_op(char* op, ast* arg0, ast* arg1);
void free_ast(ast* ast);

#endif

