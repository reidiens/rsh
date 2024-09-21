#include "rsh_core.h"

#include "rsh_output.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>

char* rsh_color_command(char *argv[], char *command) {
    if (!command) return NULL;

    uint8_t in_PATH = 0;
    char *cur_dir = strtok(PATH, ":");
    char *fname;

    if (strlen(command) + strlen(cur_dir) > strlen(PATH))
        fname = malloc((strlen(PATH) * 2) * sizeof(char));

    else
        fname = malloc(strlen(PATH) * sizeof(char));

    if (!fname) {
        rsh_err("Couldn't allocate memory for command buffer");
        return NULL;
    }

    while (cur_dir != NULL) {
        strcpy(fname, cur_dir);
        strcat(fname, command);

        if (access(fname, F_OK | X_OK) == -1) {
            if (errno == ENOENT) {
                cur_dir = strtok(NULL, ":");
                continue;
            }
            else {
                rsh_err("Error checking file in PATH");
                perror("");
                return NULL;
            }
        }
        else {
            char *ret = "\033[32m";
            strcat(ret, command);
            return ret;
        }
    };

}