#include "rsh_init.h"
#include "rsh_bufs.h"

#include <string.h>
#include <unistd.h>
#include <errno.h>

char* getArgVal(char *argv[], char *str) {
    for (char **a_p = argv + 2; *a_p; a_p++) {
        for (char *p = *a_p, *string = str; *p && *p == *string; p++, string++) {
            if (*p == '=') return ++p;
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

uint8_t rsh_init(int argc, char *argv[]) {
    username = malloc(USERNAME_MAX_LEN * sizeof(char));
    if (!username) {
        rsh_err("Couldn't allocate memory for username");
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