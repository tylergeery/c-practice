#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_LINE 80
#define MAX_HISTORY 100

typedef struct {
    int current;
    int count;
    char commands[MAX_HISTORY][MAX_LINE];
} History;

void getCommand(char *command, History *h)
{
    printf("tsh> ");
    fflush(stdout);

    fgets(command, MAX_LINE, stdin);
    command[strcspn(command, "\n")] = 0;

    if (strcmp(command, "!!") == 0) {
        strcpy(command, h->commands[h->current]);
    }
}

void addHistory(History *h, char *command)
{
    h->current++;
    if (h->current == MAX_HISTORY) {
        h->current = 0;
    }

    strcpy(h->commands[h->current], command);

    if (h->count < MAX_HISTORY) {
        h->count++;
    }
}

void getArgs(char **args, char *command)
{
    int count = 0;
    char * pch;

    for (int i = 0; i < MAX_LINE/2; i++) {
        args[i] = NULL;
    }

    pch = strtok (command," ");
    while (pch != NULL) {
        args[count] = pch;
        count++;
        pch = strtok (NULL, " ");
    }
}

int main(void)
{
    // assuming every other character is a space
    char *args[MAX_LINE/2 + 1];
    char command[MAX_LINE];
    char command_for_history[MAX_LINE];
    History h;
    h.current = 0;
    h.count = 0;

    while (1) {
        // get command
        getCommand(command, &h);

        if (strcmp(command, "") == 0) {
            continue;
        }

        strcpy(command_for_history, command);

        // parse command to make args
        getArgs(args, command);

        if (strcmp(args[0], "exit") == 0) {
            return 0;
        }

        if (strcmp(args[0], "history") == 0) {
            for (int i = 0; i < h.count; i++) {
                int indy = (h.current - i);
                if (indy < 0) {
                    indy = indy % h.count;
                }

                printf("%d %s\n", h.count - i, h.commands[indy]);
            }
        } else {
            pid_t pid = fork();

            if (pid > 0) {
                if (strpbrk(args[0], "&") == NULL) {
                    wait(NULL);
                }
            } else {
                execvp(args[0], args);
                return 0;
            }
        }

        // add commnd to history
        addHistory(&h, command_for_history);
    }
}
