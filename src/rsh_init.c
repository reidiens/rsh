#include "rsh_core.h"

#include "rsh_init.h"
#include "rsh_bufs.h"

#include <string.h>
#include <unistd.h>
#include <errno.h>

uint8_t hostname_alloc_fail = 0;

char* getArgVal(char *argv[], char *str) {
    char *string = str;
    uint8_t found = 0;

    for (char **a_p = argv + 2; *a_p; a_p++) {
        for (char *p = *a_p, *string = str; *p && *p == *string; p++, string++) {
            if (*(p + 1) == '=') return p + 2;               
        }
    }
    return NULL;
}

uint8_t rsh_open_hist_file() {
    char *hist_name = "/.rsh_history";
    char *hist_file_path = malloc((strlen(homedir) * strlen(hist_name)) * sizeof(char));
    if (!hist_file_path) {
        rsh_err("Couldn't alloc history file path memory");
        return EXIT_UNRECOVERABLE;
    }

    strcpy(hist_file_path, homedir);
    strncat(hist_file_path, hist_name, strlen(hist_name));

    rsh_history_fd = fopen(hist_file_path, "a");
    if (!rsh_history_fd) return EXIT_UNRECOVERABLE;

    free(hist_file_path);
    return EXIT_SUCCESS;
}

uint8_t rsh_open_rc_file() {
    char *rc_name = "/.rshrc";
    char *rc_file_path = malloc((strlen(homedir) + strlen(rc_name)) * sizeof(char));
    if (!rc_file_path) {
        rsh_err("Couldn't alloc rc file path memory");
        return EXIT_UNRECOVERABLE;
    }

    strcpy(rc_file_path, homedir);
    strncat(rc_file_path, rc_name, strlen(rc_name));

    rsh_rc_fd = fopen(rc_file_path, "r");
    if (!rsh_rc_fd) return EXIT_UNRECOVERABLE;

    free(rc_file_path);
    return EXIT_SUCCESS;
}

uint8_t rsh_init(int argc, char *argv[]) {
    hostname = malloc(HOSTNAME_MAX_LEN * sizeof(char));
    if (!hostname) {
        hostname_alloc_fail = 1; 
        rsh_err("Couldn't allocate hostname buffer memory. Using default (\"host\")");
        hostname = "host";
    }

    username = getArgVal(argv, "LOGNAME");
    if (!username) {
        rsh_err("Couldn't get username. Using default: 'user'");
        strcpy(username, "user");
    }

    if (gethostname(hostname, HOSTNAME_MAX_LEN) == -1) {
        rsh_err("Couldn't get hostname");
        strcpy(hostname, "machine");
    }

    homedir = getArgVal(argv, "HOME");
    if (!homedir) {
        rsh_err("Couldn't find home directory");
        free(hostname);
        exit(2); 
    }

    if (init_io_bufs() != EXIT_SUCCESS) {
        rsh_err("Couldn't initialize I/O buffers");
        if (hostname_alloc_fail == 0)
            free(hostname);
        exit(1);
    }

    if (rsh_open_rc_file() != EXIT_SUCCESS) {
        rsh_err("Couldn't open rshrc");
        if (hostname_alloc_fail == 0)
            free(hostname);
        free_io_bufs();
        exit(errno);
    }

    // rshrc processing goes here

    if (rsh_open_hist_file() != EXIT_SUCCESS) {
        rsh_err("Couldn't open history file");
        if (hostname_alloc_fail == 0)
            free(hostname);
        fclose(rsh_rc_fd);
        free_io_bufs();
        exit(errno);
    }
    return EXIT_SUCCESS;
}

void rsh_cleanup() {
    fclose(rsh_history_fd);
    fclose(rsh_rc_fd);
    free_io_bufs();
    if (hostname_alloc_fail == 0)
        free(hostname);
}