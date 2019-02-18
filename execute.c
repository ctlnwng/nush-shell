#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "execute.h"
#include "ast.h"
#include "parse.h"

int
execute_redirect_input(ast* ast)
{
    int cpid;
    int saved_stdin = dup(0);

    char* file = ast->arg1->cmd[0];
    close(0);
    
    open(file, O_RDONLY | O_CREAT);
    int arg0_result = execute(ast->arg0);
    dup2(saved_stdin, 0);
    
    return arg0_result;
}

int
execute_redirect_output(ast* ast)
{
    int cpid;
    int saved_stdout = dup(1);

    char* file = ast->arg1->cmd[0];
    close(1);
        
    open(file, O_CREAT | O_TRUNC | O_WRONLY, 0644);
    int arg0_result = execute(ast->arg0);
    dup2(saved_stdout, 1);

    return arg0_result;
}

int
execute_pipe(ast* ast)
{
    int saved_stdin = dup(0);
    int saved_stdout = dup(1);

    int cpid, rv;
    
    int pipes[2];
    pipe(pipes);

    // pipes[0] is for reading
    // pipes[1] is for writing
    
    if ((cpid = fork())) {
        close(pipes[1]);

        int status;
        waitpid(cpid, &status, 0);
        close(0);
        dup(pipes[0]);

        execute(ast->arg1);
        dup2(saved_stdin, 1);
        return status;
    }
    else {
        close(pipes[0]);
        close(1);
        dup(pipes[1]);

        execute(ast->arg0);
        dup2(saved_stdout, 0);
        exit(0);
    }
}

int
execute_background(ast* ast)
{
    int cpid;
    if ((cpid = fork())) {
        execute(ast->arg1);
    }
    else {
        execute(ast->arg0);
        exit(0);
    }
}

int
execute_and(ast* ast)
{
    int arg0_result = execute(ast->arg0);
    
    if (!arg0_result) {
        return execute(ast->arg1);
    }
    return arg0_result;
}

int
execute_or(ast* ast)
{
    int arg0_result = execute(ast->arg0);  

    if (arg0_result) {
        return execute(ast->arg1);
    }
    return arg0_result;
}

int
execute_semicolon(ast* ast)
{
    execute(ast->arg0);
    return execute(ast->arg1);
}

int
execute(ast* ast)
{
    if (!ast) {
        return 0;
    }

    char* op = ast->op;

    if (streq(op, "<")) {
        return execute_redirect_input(ast);
    } else if (streq(op, ">")) {
        return execute_redirect_output(ast);
    } else if (streq(op, "|")) {
        return execute_pipe(ast);
    } else if (streq(op, "&")) {
        return execute_background(ast);
    } else if (streq(op, "&&")) {
        return execute_and(ast);
    } else if (streq(op, "||")) {
        return execute_or(ast);
    } else if (streq(op, ";")) {
        return execute_semicolon(ast);
    } else {

        if (ast->cmd_len >= 1) {

            if (streq(ast->cmd[0], "cd")) {
                if (ast->cmd[1]) {
                    return chdir(ast->cmd[1]);
                }
                else {
                    return chdir("~");
                }
            }

            if (streq(ast->cmd[0], "exit")) {
                exit(0);
            }
            
            int cpid;

            if ((cpid = fork())) {
                int status;
                waitpid(cpid, &status, 0); 
                return status;
            }
            else {
                char** args = ast->cmd;

                execvp(args[0], args);
                return -1;
            }
        }
    }
}
