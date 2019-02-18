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
main(int argc, char* argv[])
{
    FILE* fp;
    size_t len = 0;
    ssize_t read_line;
    char* line;
    char buf[256];

    int file_input = 0;

    if (argc > 2) {
        exit(1);
    }
    else if (argc == 2) {
        file_input = 1;
        fp = fopen(argv[1], "r");
    }

    while(1) {
        if (!file_input) {
            printf("nush$ ");
            fflush(stdout);
            line = fgets(buf, 256, stdin);

            if (!line) {
                break;
            }
        }
        else {
            line = fgets(buf, 256, fp);
            fflush(fp);

            if (feof(fp) != 0) {
                fclose(fp);
                break;
            }
        }

        svec* tokens = tokenize(line);
        ast* token_ast = parse_cmd(tokens);
        // print_ast(token_ast);
        execute(token_ast);

        free_ast(token_ast);
        free_svec(tokens);
    }

    return 0;
}

