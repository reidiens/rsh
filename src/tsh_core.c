#include "tsh_core.h"
#include "tsh_funcs.h"
#include <unistd.h>

FILE *historyfd;
char *homedir;

#define TOK_DELIM " \t\n\r\a"

uint8_t tsh_init(void) {
    char *args[] = {"neofetch", "\0"};
    if (run(args) != EXIT_SUCCESS) exit(errno);

    struct passwd *userinf = getpwuid(getuid());
    homedir = userinf->pw_dir;

    char *historyfile = homedir;
    char *name = "/.tsh_history";    

    strncat(historyfile, name, strlen(name));
    
    historyfd = fopen(historyfile, "a");
    if (historyfd == NULL) {
        perror("\033[31mtsh:\033[0m Could not open history file `.tsh_history`");
        exit(errno);
    }

    return EXIT_SUCCESS;
}

char* read_line(char *buf, size_t *bufSize) {
    char c;
    int pos = 0;

    do {
        if (pos >= (int)(*bufSize * 0.6)) {
            *bufSize *= 2;
            buf = realloc(buf, *bufSize * sizeof(char));

            if (buf == NULL) {
                perror("\033[31mtsh: \033[0m error allocating command buffer memory");
                fclose(historyfd);
                free(buf);
                exit(errno);
            }
        }

        c = getchar();
        if (c == EOF || c == '\n') {
            buf[pos] = '\0';
            return buf;
        } 

        buf[pos] = c;
        pos++;

    } while (1);
}

char** parse_line (char *line, size_t *argSize) {
    char **args = malloc(*argSize * sizeof(char *));

    if (args == NULL) {
        perror("\033[31mtsh: \033[0m error allocating memory for command token array");
        fclose(historyfd);
        free(args);
        free(line);
        exit(errno);
    }

    char *token;
    int pos = 0;

    token = strtok(line, TOK_DELIM);

    while (token != NULL) {
        if (pos >= (int)(*argSize * 0.6)) {
            *argSize *= 2;
            args = realloc(args,*argSize * sizeof(char *));

            if (args == NULL) {
                perror("\033[31mtsh:\033[0m error allocating memory for command token array");
                fclose(historyfd);
                free(args);
                free(line);
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
    size_t bufSize = LINE_BUFSIZE, argSize = ARG_BUFSIZE;
    char *buf = malloc(bufSize * sizeof(char));
    char **args;

    if (buf == NULL) {
        perror("\033[31mtsh:\033[0m error allocating command buffer memory");
        fclose(historyfd);
        exit(errno);
    }    

    char hname[255];
    gethostname(hname, sizeof(hname));

    while (1) {
        printf("╭─\033[96m%s\033[92m@\033[96m%s\n\033[0m╰─🞛 ", getlogin(), hname);

        buf = read_line(buf, &bufSize); 

        fprintf(historyfd, "%s\n", buf);

        if (strcmp(buf, "exit") == 0) {
            free(buf);
            fclose(historyfd);
            exit(EXIT_SUCCESS); 
        }

        args = parse_line(buf, &argSize);

        if (strcmp(args[0], "cd") == 0) {
            change_dir(args);
            memset(buf, 0, bufSize);
            memset(args, 0, argSize);
            puts(" ");
            continue;
        }

        if (run(args) != EXIT_SUCCESS) {
            free(buf);
            free(args);
            fclose(historyfd);
            exit(errno);
        } 

        memset(buf, 0, bufSize);
        memset(args, 0, argSize);
        puts(" ");
    }

}