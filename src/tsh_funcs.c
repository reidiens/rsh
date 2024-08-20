#include "tsh_core.h"
#include "tsh_funcs.h"

uint8_t run(char **args) {
    pid_t pid = fork(), wpid;
    int stat;

    if (pid == 0) {
        if (execvp(args[0], args)) {
            if (errno == ENOENT) {
                printf("\033[31m tsh:\033[0m Command not found: %s\n", args[0]);
                return EXIT_RECOVERABLE;
            }
            perror("\033[31mtsh:\033[0m Error running command");
            return EXIT_UNRECOVERABLE;
        }
    }
    else if (pid < 0) {
        perror("\033[31mtsh:\033[0m Couldn't fork process");
        return EXIT_UNRECOVERABLE;
    }
    else {
        do {
            wpid = waitpid(pid, &stat, WUNTRACED);
            if (wpid < 0) {
                perror("\033[31mtsh:\033[0m Error waiting for process");
                return EXIT_UNRECOVERABLE;
            }
        } while (!WIFEXITED(stat) && !WIFSIGNALED(stat));
    }

    return EXIT_SUCCESS;
}

uint8_t change_dir(char **args) {
    if (args[1] == NULL) {
        if (chdir(homedir) == -1) {
            perror("cd");
            return EXIT_RECOVERABLE;
        } 
    }
    if (chdir(args[1]) == -1) {
        if (errno == ENOENT) printf("cd: No such file or directory\n");
        else perror("cd");
        return EXIT_RECOVERABLE;
    }
    return EXIT_SUCCESS;
}