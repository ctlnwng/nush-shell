// NOTE: All svec functions besides 'reverse_svec' are from Lecture 9's 'calc' program

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "svec.h"

svec*
make_svec()
{
    svec* sv = malloc(sizeof(svec));
    sv->size = 0;
    sv->cap = 4;
    sv->data = malloc(4 * sizeof(char*));
    memset(sv->data, 0, 4 * sizeof(char*));
    return sv;
}

void free_svec(svec* sv)
{
    for (int i = 0; i < sv->size; ++i) {
        if (sv->data[i] != 0) {
            free(sv->data[i]);
        }
    }

    free(sv->data);
    free(sv);
}

void svec_put(svec* sv, int i, char* item)
{
    assert(i >= 0 && i < sv->size);
    sv->data[i] = strdup(item);
}

void svec_push_back(svec* sv, char* item)
{
    int i = sv->size;

    if (i >= sv->cap) {
        sv->cap *= 2;
        sv->data = (char**) realloc(sv->data, sv->cap * sizeof(char*));
    }

    sv->size = i + 1;
    svec_put(sv, i, item);
}

void reverse_svec(svec* sv)
{
    int front = 0;
    int end = sv->size - 1;
    
    // keep swapping values on opposite ends of the array
    while (front < end) {
        char* temp = sv->data[front];
        sv->data[front] = sv->data[end];
        sv->data[end] = temp;

        ++front;
        --end;
    }
}
