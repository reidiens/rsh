#include "rsh_core.h"

#include "rsh_init.h"
#include "rsh_bufs.h"

#include <string.h>
#include <unistd.h>
#include <errno.h>

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

void rsh_cleanup() {
    fclose(rsh_history_fd);
    free_io_bufs();
    free(username);
    free(hostname);
    free(homedir);
}

uint8_t init_user_info_bufs() {
    username = malloc(USERNAME_MAX_LEN * sizeof(char));
    if (!username) return EXIT_UNRECOVERABLE;

    hostname = malloc(HOSTNAME_MAX_LEN * sizeof(char));
    if (!hostname) {
        free(username);
        return EXIT_UNRECOVERABLE;
    }

    homedir = malloc(HOMEDIR_MAX_LEN * sizeof(char));
    if (!homedir) {
        free(hostname);
        free(username);
        return EXIT_UNRECOVERABLE;
    }

    return EXIT_SUCCESS;
}

uint8_t rsh_init(int argc, char *argv[]) {
    if (init_user_info_bufs() != EXIT_SUCCESS) {
        rsh_err("Couldn't allocate memory for user info");
        exit(1);
    }

    username = getlogin();
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
        strcpy(homedir, "home");
    }

    char *historyfile = homedir;
    char *name = "/.rsh_history";    

    strncat(historyfile, name, strlen(name));
    
    if (init_io_bufs() != EXIT_SUCCESS) {
        rsh_err("Couldn't initialize I/O buffers");
        free(username);
        free(hostname);
        free(homedir);
        exit(1);
    }
    
    rsh_history_fd = fopen(historyfile, "a");
    if (rsh_history_fd == NULL) {
        rsh_err("Couldn't open history file");
        free_io_bufs();
        exit(errno);
    }

    return EXIT_SUCCESS;
}