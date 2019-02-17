
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "ast.h"
#include "parse.h"
#include "svec.h"

int
streq(const char* aa, const char* bb)
{
    return strcmp(aa, bb) == 0;
}

int
find_first_index(svec* toks, const char* tt)
{
    for (int ii = 0; ii < toks->size; ++ii) {
        if (streq(toks->data[ii], tt)) {
            return ii;
        }
    }

    return -1;
}

int
contains(svec* toks, const char* tt)
{
    return find_first_index(toks, tt) >= 0;
}

svec*
slice(svec* xs, int i0, int i1)
{
    svec* ys = make_svec();
    for (int ii = i0; ii < i1; ++ii) {
        svec_push_back(ys, xs->data[ii]);
    }
    return ys;
}
/*
calc_ast*
parse(svec* toks)
{
    if (toks->size == 1) {
        int vv = atoi(toks->data[0]);
        return make_ast_value(vv);
    }

    const char* ops[] = {"+", "-", "*", "/"};

    for (int ii = 0; ii < 4; ++ii) {
        const char* op = ops[ii];

        if (contains(toks, op)) {
            int jj = find_first_index(toks, op);
            svec* xs = slice(toks, 0, jj);
            svec* ys = slice(toks, jj + 1, toks->size);
            calc_ast* ast = make_ast_op(op[0], parse(xs), parse(ys));
            free_svec(xs);
            free_svec(ys);
            return ast;
        }
    }

    fprintf(stderr, "Invalid token stream");
    exit(1);
}*/

ast*
parse_cmd(svec* tokens)
{
    char* ops[] = {"|", ">", "<", "&&", "||", ";", "&"};

    for (int i = 0; i < 7; ++i) {
        char* op = ops[i];

        if (contains(tokens, op)) {
            int op_index = find_first_index(tokens, op);
            svec* left = slice(tokens, 0, op_index);
            svec* right = slice(tokens, op_index + 1, tokens->size); 
          
            //ast* ast = make_ast_op(op, parse_cmd(left), parse_cmd(right));
           
            ast* ast_left = parse_cmd(left);
            printf("ast_left pointer: %p\n", ast_left);

            ast* ast_right = parse_cmd(right);
            printf("ast_right pointer: %p\n", ast_right);

            ast* ast = make_ast_op(op, ast_left, ast_right);
            //print_ast(ast_left);
            //print_ast(ast_right);

            free_svec(left);
            free_svec(right);
            return ast;
        }
    }

    char** cmd = malloc((tokens->size + 1) * sizeof(char*)); //malloc throws error here
    int cmd_len = tokens->size;

    for (int i = 0; i < tokens->size; ++i) {
        int str_len = strlen(tokens->data[i]);
        cmd[i] = malloc(str_len + 1);
        memcpy(cmd[i], tokens->data[i], str_len);
        cmd[i][str_len] = 0;
    }

    cmd[cmd_len] = 0;
   
    ast* cmd_ast = make_ast_cmd(cmd, cmd_len);

    printf("cmd_ast pointer: %p\n", cmd_ast);

    return cmd_ast;
}
