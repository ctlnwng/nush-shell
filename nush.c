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
    char* line;
    char buf[256];

    int file_input = 0; // boolean flag for if there is a file input

    if (argc > 2) {
        exit(1);
    }
    else if (argc == 2) {
        file_input = 1;
        fp = fopen(argv[1], "r"); // open given shell script
    }

    while(1) {
        // interactive, read from stdin
        if (!file_input) {
            printf("nush$ ");
            fflush(stdout);

            line = fgets(buf, 256, stdin); 

            if (!line) {
                break;
            }
        }
        // read from file
        else {
            
            fflush(stdout);
            line = fgets(buf, 256, fp);
            fflush(fp);

            if (line) {
                int len = strlen(line);
                char concat_line[256];

                // if current line ends with '\', concat next line
                while (line[len - 2] == 92) {
                    strncpy(concat_line, line, len - 2); // ignore '\' and '\n' characters
                    
                    char* next_line = fgets(buf, 256, fp);
                    strcat(concat_line, next_line);
                    line = concat_line;
                }
            }

            if (feof(fp) != 0) {
                fclose(fp);
                break;
            }

        }

        svec* tokens = tokenize(line);
        ast* token_ast = parse_cmd(tokens);
        execute(token_ast);

        free_ast(token_ast);
        free_svec(tokens);
    }

    return 0;
}

