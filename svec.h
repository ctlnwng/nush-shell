// NOTE: svec structure from lecture notes (calc program)

#ifndef SVEC_H
#define SVEC_H

typedef struct svec {
    int size;
    int cap;
    char** data;
} svec;

svec* make_svec();
void free_svec(svec* sv);

void svec_put(svec* sv, int ii, char* item);

void svec_push_back(svec* sv, char* item);
void reverse_svec(svec* sv);

#endif
