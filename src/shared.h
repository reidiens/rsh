#ifndef __TSH_SHARED_H__
    #define __TSH_SHARED_H__ 1

#include <stdio.h>

#define EXIT_SUCCESS        0
#define EXIT_RECOVERABLE    1
#define EXIT_UNRECOVERABLE  2

typedef struct Tsh_Buffer {
    char *buf;
    size_t max;
    size_t size;
} tsh_buf_t;

extern tsh_buf_t *inbuf;
extern tsh_buf_t *outbuf;

extern FILE *historyfd;

#endif