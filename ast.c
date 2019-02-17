#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"

ast*
make_ast_cmd(char** cmd, int cmd_len)
{
    ast* ast = malloc(sizeof(ast));
    ast->op = "=";
    ast->arg0 = NULL;
    ast->arg1 = NULL;
    ast->cmd = cmd;
    ast->cmd_len = cmd_len;
    return ast;
}

ast*
make_ast_op(char* op, ast* arg0, ast* arg1)
{
    ast* ast = malloc(sizeof(ast));
    ast->op = op;
    ast->arg0 = arg0;
    ast->arg1 = arg1;
    ast->cmd = NULL;
    ast->cmd_len = 0;
    return ast;
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
            for (int i = 0; i < ast->cmd_len; ++i) {
                free(ast->cmd[i]);
            }
            free(ast->cmd);
        }
        
        free(ast);
    }
}

void
print_ast(ast* ast)
{
    if (ast) {
        if (ast->op) {
            printf("%s\n", ast->op);
         }

        if (ast->cmd) {
            printf("size: %d\n", ast->cmd_len);
            
            for (int i = 0; i < ast->cmd_len; ++i) {
                printf("%s ", ast->cmd[i]);
            }
            // printf("%s\n", ast->cmd[0]);

            printf("\n");
        }

        if (ast->arg0) {
            print_ast(ast->arg0);
        }

        if (ast->arg1) {
            print_ast(ast->arg1);
        }
    }
}


/*
int
ast_eval(calc_ast* ast)
{
    switch (ast->op) {
    case '=':
        return ast->value;
    case '+':
        return ast_eval(ast->arg0) + ast_eval(ast->arg1);
    case '-':
        return ast_eval(ast->arg0) - ast_eval(ast->arg1);
    case '*':
        return ast_eval(ast->arg0) * ast_eval(ast->arg1);
    case '/':
        return ast_eval(ast->arg0) / ast_eval(ast->arg1);
    default:
        fprintf(stderr, "Unknown op: %c\n", ast->op);
        exit(1);
    }
}
*/
