#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "tokens.h"
#include "svec.h"
#include "ast.h"
#include "parse.h"
#include "execute.h"

int
main(int _argc, char* _argv[])
{
    char cmd[256];

    while(1) {
        printf("nush$ ");
        fflush(stdout);
        char* input = fgets(cmd, 256, stdin);

        if (!input) {
            break;
        }

        svec* tokens = tokenize(input);
        ast* token_ast = parse_cmd(tokens);
        print_ast(token_ast);
//      execute(token_ast);
        
        free_ast(token_ast);
        free_svec(tokens);
    }
    return 0;
}

