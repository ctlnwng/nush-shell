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
}

int
execute_background(ast* ast)
{
    int cpid;
    if ((cpid = fork())) {
        return execute(ast->arg1);
    }
    else {
        return execute(ast->arg0);
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

        int cpid;

        if ((cpid = fork())) {

            int status;
            waitpid(cpid, &status, 0);

        // printf("== executed program complete ==\n");
                
            //printf("child returned with wait code %d\n", status);
           // if (WIFEXITED(status)) {
             //   printf("child exited with exit code (or main returned) %d\n", WEXITSTATUS(status));
            
            //}
            return status;
        }
        else {
            char** args = ast->cmd;

            execvp(args[0], args);
            return -1;
        }
    }
}
