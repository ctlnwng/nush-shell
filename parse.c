// NOTE: Most functions from the parse.c file in Lecture 9's 'calc' program

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

ast*
parse_cmd(svec* tokens)
{
    // ops in reverse precedence, since the first operator to
    // get added to the AST will be evaluated last
    char* ops[] = {"&", ";", "||", "&&", ">", "<", "|"};

    for (int i = 0; i < 7; ++i) {
        char* op = ops[i];

        if (contains(tokens, op)) {
            int op_index = find_first_index(tokens, op);
            svec* left = slice(tokens, 0, op_index);
            svec* right = slice(tokens, op_index + 1, tokens->size); 
          
            ast* op_ast = make_ast_op(op, parse_cmd(left), parse_cmd(right));
           
            free_svec(left);
            free_svec(right);
            return op_ast;
        }
    }

    char** cmd = malloc((tokens->size + 1) * sizeof(char*));
    int cmd_len = tokens->size;

    for (int i = 0; i < tokens->size; ++i) {
        int str_len = strlen(tokens->data[i]);
        cmd[i] = malloc(str_len + 1);
        memcpy(cmd[i], tokens->data[i], str_len);
        cmd[i][str_len] = 0;
    }

    cmd[cmd_len] = 0;
   
    return make_ast_cmd(cmd, cmd_len);
}
