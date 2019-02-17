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
}

void
execute_and(ast* ast)
{
}

void
execute_or(ast* ast)
{
}

void
execute_semicolon(ast* ast)
{
    printf("%s\n", "in execute_semicolon");

    int cpid;

    if ((cpid = fork())) {
        int status;
        waitpid(cpid, &status, 0);

        printf("child returned with wait code %d\n", status);
        if (WIFEXITED(status)) {
            printf("child exited with exit code (or main returned) %d\n", WEXITSTATUS(status));
        }
    }
    else {
        execute(ast->arg0);
        execute(ast->arg1);
    }
}

void
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
        // parent process
        // printf("Parent pid: %d\n", getpid());
        // printf("Parent knows child pid: %d\n", cpid);

        // Child may still be running until we wait.

            int status;
            waitpid(cpid, &status, 0);

        // printf("== executed program complete ==\n");

            printf("child returned with wait code %d\n", status);
            if (WIFEXITED(status)) {
                printf("child exited with exit code (or main returned) %d\n", WEXITSTATUS(status));
            }
        }
        else {
        // child process
        //  printf("Child pid: %d\n", getpid());
        // printf("Child knows parent pid: %d\n", getppid());

         char** args = ast->cmd;

        // The argv array for the child.
        // Terminated by a null pointer.
        // char* args[] = {cmd, "one", 0};
            printf("first arg %s\n", args[0]);
            printf("== executed program's output: ==\n");

            execvp(args[0], args);
            printf("Can't get here, exec only returns on error.");
        }
    }
}
