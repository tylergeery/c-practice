#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>

#define MAX_LINE 80
#define MAX_HISTORY 100

typedef struct {
    int current;
    int count;
    char commands[MAX_HISTORY][MAX_LINE];
} History;

void getCommand(char *command)
{
    printf("tsh> ");
    fflush(stdout);

    fgets(command, MAX_LINE, stdin);
    command[strcspn(command, "\n")] = 0;

    return command;
}

void addHistory(History *h, char *command)
{
    if (h->current == (MAX_HISTORY - 1)) {
        h->current = 0;
    }

    &(h->commands[h->current]) = command;

    if (h->count < MAX_HISTORY) {
        h->count++;
    }
}

void getArgs(char **args, char *command)
{
    int count = 0;
    char * pch;

    printf ("Splitting command \"%s\" into args:\n", command);
    pch = strtok (command," ");
    while (pch != NULL) {
        printf ("arg %d: %s\n", count, pch);
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
    History h;

    while (1) {
        // get command
        getCommand(command);

        // parse command to make args
        getArgs(args, command);

        if (strcmp(args[0], "exit") == 0) {
            return 0;
        }

        if (strcmp(args[0], "history") == 0) {
            // TODO: print history
        }

        pid_t pid = fork();

        if (pid > 0) {
            if (strpbrk(args, "&") == NULL) {
                printf("Parent Waiting\n");
                wait(NULL);
            }

            printf("Parent continuing\n");
        } else {
            printf("Child running command: %s\n", args[0]);
            // execvp(args[0], args);
            return 0;
        }

    }
}
