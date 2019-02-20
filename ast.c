// NOTE: Code based on ast.c from Lecture 9's 'cal'c program

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"

ast*
make_ast_cmd(char** cmd, int cmd_len)
{
    ast* new_ast = malloc(sizeof(ast));
    new_ast->op = "="; // dummy operator for ASTs with no ops
    new_ast->arg0 = NULL;
    new_ast->arg1 = NULL;
    new_ast->cmd = cmd;
    new_ast->cmd_len = cmd_len;
    return new_ast;
}

ast*
make_ast_op(char* op, ast* arg0, ast* arg1)
{
    ast* new_ast = malloc(sizeof(ast));
    new_ast->op = op;
    new_ast->arg0 = arg0;
    new_ast->arg1 = arg1;
    new_ast->cmd = NULL;
    new_ast->cmd_len = 0;
    return new_ast;
}

void
free_ast(ast* ast)
{
    if (ast) {
        if (ast->arg0) {
            free_ast(ast->arg0);
        }

        if (ast->arg1) {
            free_ast(ast->arg1);
        }

        if (ast->cmd) {
            // free all strings in ast->cmd array
            for (int i = 0; i < ast->cmd_len; ++i) {
                free(ast->cmd[i]);
            }
            free(ast->cmd);
        }
        
        free(ast);
    }
}
