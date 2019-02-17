#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "execute.h"
#include "ast.h"
#include "parse.h"

void
execute_redirect_input(ast* ast)
{
}

void
execute_redirect_output(ast* ast)
{
}

void
execute_pipe(ast* ast)
{
}

void
execute_background(ast* ast)
{
    int cpid;
    if ((cpid = fork())) {
        execute(ast->arg1);
    }
    else {
        execute(ast->arg0);
    }
}

void
execute_and(ast* ast)
{
    int cpid;
    if ((cpid = fork())) {
        int status;
        waitpid(cpid, &status, 0);
    }
    else {
        if (!execute(ast->arg0)) {
            execute(ast->arg1);
        }
    }
}

void
execute_or(ast* ast)
{
    int cpid;
    if ((cpid = fork())) {
        int status;
        waitpid(cpid, &status, 0);

        if (WIFEXITED(status)) {
            execute(ast->arg1);
        }
    }
    else {
        if (execute(ast->arg0)) {
            execute(ast->arg1);
        };
    }
}

void
execute_semicolon(ast* ast)
{
    int cpid;

    if ((cpid = fork())) {
        int status;
        waitpid(cpid, &status, 0);
    }
    else {
        execute(ast->arg0);
        execute(ast->arg1);
    }
}

int
execute(ast* ast)
{
    char* op = ast->op;

    if (streq(op, "<")) {
        execute_redirect_input(ast);
    } else if (streq(op, ">")) {
        execute_redirect_output(ast);
    } else if (streq(op, "|")) {
        execute_pipe(ast);
    } else if (streq(op, "&")) {
        execute_background(ast);
    } else if (streq(op, "&&")) {
        execute_and(ast);
    } else if (streq(op, "||")) {
        execute_or(ast);
    } else if (streq(op, ";")) {
        execute_semicolon(ast);
    } else {

        int cpid;

        if ((cpid = fork())) {

        // Child may still be running until we wait.

            int status;
            waitpid(cpid, &status, 0);

        // printf("== executed program complete ==\n");
                
            //printf("child returned with wait code %d\n", status);
           // if (WIFEXITED(status)) {
             //   printf("child exited with exit code (or main returned) %d\n", WEXITSTATUS(status));
            
            //}
            //
            return status;
        }
        else {
            char** args = ast->cmd;

            // printf("== executed program's output: ==\n");

            execvp(args[0], args);
            return -1;
        }
    }
}
