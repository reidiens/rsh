#ifndef __TSH_FUNCS_H__
    #define __TSH_FUNCS_H__ 1

#ifndef _STDIO_H
    #include <stdio.h>
#endif

#ifndef _UNISTD_H
    #include <unistd.h>
#endif

#ifndef _ERRNO_H
    #include <errno.h>
#endif

#ifndef __CLANG_STDINT_H
    #include <stdint.h>
#endif

#ifndef _SYS_WAIT_H
    #include <sys/wait.h>
#endif

uint8_t change_dir(char **args);
uint8_t run(char **args);

#endif