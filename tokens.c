// NOTE: Code from HW05

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "svec.h"
#include "tokens.h"

svec*
tokenize(char* text)
{
    svec* tokens = make_svec();

    int textLen = strlen(text);
    int i = 0;

    while (i < textLen) {

        if (isspace(text[i])) {
            ++i;
            continue;
        }

        char* tkn = read_token(text, i);
        svec_push_back(tokens, tkn);
        i += strlen(tkn);
        
        // tkn is a malloc'd char*
        free(tkn);
    }

    return tokens;
}

char*
read_token(char* text, int i)
{
    int n = 0;
    char* tkn = malloc(3); // max. necessary size for operator

    // count size of string with no operators/spaces
    while (!is_op(text[i+n]) && !isspace(text[i+n])) {
        ++n;
    }

    // if no operators exist, realloc string
    if (n > 0) {
        tkn = realloc(tkn, n + 1);
    }
    else {
        n = get_op_size(text, i);
    }

    memcpy(tkn, text + i, n); // copying string will let us free it easily later
    tkn[n] = 0;

    return tkn;
}

// gets string length of operator
// sequences of '&' and '|' are grouped by 2
int
get_op_size(char* text, int i)
{
    int n = 0;
    int textLen = strlen(text);
    char c = text[i + n];

    if (is_op(c)) {
        ++n;

        if (textLen - i > 1) {
            if (c == '&' && text[i + n] == '&') {
                ++n;
            }

            if (c == '|' && text[i + n] == '|') {
                ++n;
            }
        }
    }

    return n; 
}

// does given char match an operator char?
int
is_op(char c)
{
    switch (c) {
        case '<':
            return 1;
        case '>':
            return 1;
        case '|':
            return 1;
        case '&':
            return 1;
        case ';':
            return 1;
        default:
            return 0;
    }
}
