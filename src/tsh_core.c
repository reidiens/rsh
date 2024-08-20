#include "tsh_core.h"

#define TOK_DELIM " \t\n\r\a"

char* getArgvVal(char **argv, char *arg_name) {
    char *p = arg_name;
    for (char **parg = argv + 2; *parg; parg++) {
        for (char *arg_str = *parg; *arg_str && *arg_str == *p; arg_str++) {
            if (*arg_str == '=') return arg_str + 1;
            p++;
        }
    }
    return NULL;
}

void tsh_err(const char *str) {
    fprintf(stderr, "\033[31mtsh: \033[0m%s\n", str);
}

uint8_t tsh_init(int argc, char *argv[]) {
    if (init_io_bufs() != EXIT_SUCCESS) {
        tsh_err("Couldn't initialize I/O buffers");
        exit(1);
    }

    homedir = getArgvVal(argv, "HOME");
    if (!homedir) {
        tsh_err("Couldn't get home directory");
        free_io_bufs();
        exit(2);
    }

    char *historyfile = homedir;
    char *name = "/.tsh_history";    

    strncat(historyfile, name, strlen(name));
    
    historyfd = fopen(historyfile, "a");
    if (historyfd == NULL) {
        tsh_err("Couldn't open history file");
        free_io_bufs();
        exit(errno);
    }

    return EXIT_SUCCESS;
}

void tsh_cleanup() {
    free_io_bufs();
    fclose(historyfd);
}

uint8_t get_input() {
    char c;

    do {
        if (inbuf->size >= (int)(inbuf->max * 0.6)) {
            inbuf->max *= 2;
            inbuf->buf = realloc(inbuf->buf, inbuf->max * sizeof(char));

            if (inbuf->buf == NULL) {
                tsh_err("Error allocating command buffer memory");
                tsh_cleanup();
                exit(errno);
            }
        }

        c = getchar();
        if (c == EOF || c == '\n') {
            inbuf->buf[inbuf->size] = '\0';
            return EXIT_SUCCESS;
        } 

        inbuf->buf[inbuf->size] = c;
        inbuf->size++;

    } while (1);
}

char** parse_input_buf(size_t *argSize) {
    char **args = malloc(*argSize * sizeof(char *));

    if (args == NULL) {
        tsh_err("Error allocating memory for command token array");
        tsh_cleanup();
        free(args);
        exit(errno);
    }

    char *token;
    int pos = 0;

    token = strtok(inbuf->buf, TOK_DELIM);

    while (token != NULL) {
        if (pos >= (int)(*argSize * 0.6)) {
            *argSize *= 2;
            args = realloc(args,*argSize * sizeof(char *));

            if (args == NULL) {
                tsh_err("Error allocating memory for command token array");
                free(args);
                tsh_cleanup();
                exit(errno);
            }
        }

        args[pos] = token;
        token = strtok(NULL, TOK_DELIM);
        pos++;
    };

    args[pos] = NULL;

    return args;
}

void tsh_loop(void) {
    size_t argSize = ARG_BUFSIZE;
    char **args;

    char hname[255];
    gethostname(hname, sizeof(hname));

    while (1) {
        printf("╭─\033[96m%s\033[92m@\033[96m%s\n\033[0m╰─🞛 ", getlogin(), hname);

        get_input(); 

        fprintf(historyfd, "%s\n", inbuf->buf);

        if (strcmp(inbuf->buf, "exit") == 0) {
            tsh_cleanup();
            exit(EXIT_SUCCESS); 
        }

        args = parse_input_buf(&argSize);

        if (strcmp(args[0], "cd") == 0) {
            change_dir(args);
            flush_io_bufs();
            memset(args, 0, argSize);
            puts(" ");
            continue;
        }

        if (run(args) != EXIT_SUCCESS) {
            free(args);
            tsh_cleanup();
            exit(errno);
        } 

        flush_io_bufs();
        memset(args, 0, argSize);
        puts(" ");
    }

}