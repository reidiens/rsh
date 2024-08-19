#ifndef __TSH_CORE__ 
    #define __TSH_CORE__ 1

#ifndef __CLANG_STDINT_H
    #include <stdint.h>
#endif

#ifndef _STDIO_H
    #include <stdio.h>
#endif

#ifndef _UNISTD_H
    #include <unistd.h>
#endif

#ifndef _ERRNO_H
    #include <errno.h>
#endif

#ifndef _PWD_H
    #include <pwd.h>
#endif

#ifndef _STRING_H
    #include <string.h>
#endif

#ifndef _STDLIB_H
    #include <stdlib.h>
#endif

#define EXIT_SUCCESS        0
#define EXIT_RECOVERABLE    1
#define EXIT_UNRECOVERABLE  2

#define LINE_BUFSIZE    2048
#define ARG_BUFSIZE       32

uint8_t tsh_init(void);
void tsh_loop(void);

#endif