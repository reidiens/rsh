#include "rsh_loop.h"

#include "rsh_bufs.h"
#include "rsh_core.h"
#include "rsh_funcs.h"
#include "rsh_init.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define ARG_BUFSIZE 32

uint8_t rsh_readline() {
    char c;

    do {
        if (rsh_inbuf->size >= (int)(rsh_inbuf->max * 0.6)) {
            rsh_inbuf->max += rsh_inbuf->max;
            rsh_inbuf->buf = realloc(rsh_inbuf->buf, rsh_inbuf->max * sizeof(char));

            if (!rsh_inbuf->buf) {
                rsh_err("Error allocating text input buffer");
                rsh_cleanup();
                exit(errno);
            }
        }

        c = getchar();
        if (c == EOF || c == '\n') {
            rsh_inbuf->buf[rsh_inbuf->size] = '\0';
            return EXIT_SUCCESS;
        }

        rsh_inbuf->buf[rsh_inbuf->size] = c;
        rsh_inbuf->size++;

    } while (1);
}

char **parse_input_buf(size_t *argSize) {
    char **args = malloc(*argSize * sizeof(char *));
    if (!args) {
        rsh_err("Error allocating memory for token buffer");
        rsh_cleanup();
        exit(errno);
    }

    char *token;
    int pos = 0;

    token = strtok(rsh_inbuf->buf, " \n\t\a\r");

    while (token != NULL) {
        if (pos >= (int)(*argSize * 0.6)) {
            *argSize += *argSize;
            args = realloc(args, *argSize * sizeof(char));
            if (!args) {
                rsh_err("Error allocating memory for token buffer");
                free(args);
                rsh_cleanup();
                exit(errno);
            }
        }

        args[pos] = token;
        token = strtok(NULL, " \n\t\a\r");
        pos++;
    };

    args[pos] = NULL;

    return args;
}

void rsh_prompt() {
    printf("╭─\033[96m%s\033[92m@\033[96m%s\n\033[0m╰─🞛 ", username, hostname);
}

void rsh_loop(void) {
    size_t argSize = ARG_BUFSIZE;
    char **args;

    gethostname(hostname, HOSTNAME_MAX_LEN);

    while (1) {
        rsh_prompt();
        fprintf(rsh_history_fd, "%s\n", rsh_inbuf->buf);

        if (strcmp(rsh_inbuf->buf, "exit") == 0) {
            rsh_cleanup();
            exit(EXIT_SUCCESS);
        }
    
        args = parse_input_buf(&argSize);
    
        if (strcmp(args[0], "cd") == 0) {
            rsh_cd(args);
            flush_io_bufs();
            memset(args, 0, argSize);
            puts(" ");
            continue;
        }

        if (rsh_exec_command(args) != EXIT_SUCCESS) {
            free(args);
            rsh_cleanup();
            exit(errno);
        }
    
        flush_io_bufs();
        memset(args, 0, argSize);
        puts(" ");
    }

}