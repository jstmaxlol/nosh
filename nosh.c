/*
 * nosh - noshell
 * a 2-hour C shell for POSIX mates
 *
 * czjstmax <jstmaxlol at disroot dot org>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <wordexp.h>
#include <sys/wait.h>

//
void handlecc(int sig);
void freeStuff();

//
char *line = NULL;
size_t len = 0;
wordexp_t p;

int main(void)
{
    signal(SIGINT, handlecc);

    while (true) {
        printf("$ ");
        getline(&line, &len, stdin);

        line[strcspn(line, "\n")] = '\0';

        wordexp(line, &p, 0);

        // debug
//         for (size_t i = 0; i < p.we_wordc; i++) {
//             printf("argv[%zu] = %s\n", i, p.we_wordv[i]);
//         }

        if (strlen(line) == 0)
            continue;               //otherwise.. SIGSEGV!
        if (strcmp(p.we_wordv[0], ":q") == 0 || strcmp(p.we_wordv[0], "exit") == 0) {
            freeStuff();
            return 0;
        }
        else if (strcmp(p.we_wordv[0], "echo") == 0) {
            for (size_t i = 1; i < p.we_wordc; i++)
                printf("%s ", p.we_wordv[i]);
            if (p.we_wordc >= 2) {
                if (strcmp(p.we_wordv[1], "-c") == 0)
                    continue;
                printf("\n");
            }
        }
        else if (strcmp(p.we_wordv[0], "cd") == 0) {
            if (chdir(p.we_wordv[1]))
                perror("nsh! ERROR: failed to change directory");
        }
        else if (strcmp(p.we_wordv[0], "clear") == 0) {
            printf("\033[2J\033[H\033[3J");
            fflush(stdout);
        }
        else {
            pid_t pid = fork();
            char **argv = p.we_wordv; // pray with me now
            if (pid == 0) {
                char path[strlen("/usr/bin/") + strlen(p.we_wordv[0]) + 1];
                sprintf(path, "/usr/bin/%s", p.we_wordv[0]);
                execv(path, argv);
                perror("nsh! ERROR: failed to execv()");
                _exit(1);
            }
            else if (pid > 0) {
                int status;
                waitpid(pid, &status, 0);
            }
            else {
                printf("nsh! couldn't open %s", p.we_wordv[0]);
            }
        }
    }
}

void handlecc(int sig)
{
    printf("\nnsh! %d caught.\nfreeing stuff before quitting.\n", sig);
    freeStuff();
    exit(0);
}

void freeStuff()
{
    wordfree(&p);
    free(line);
}

