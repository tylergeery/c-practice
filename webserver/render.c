#include "../csapp/csapp.h"
#include <stdio.h>

#define ARRAY_LENGTH(array) (sizeof((array))/sizeof((array)[0]))

struct Query {
    char *params[10][2];
};
/**
 * Gets the passed file's size
 */
long int get_filesize(FILE *stream)
{
    fseek(stream, 0L, SEEK_END);
    long int filesize = ftell(stream);
    rewind(stream);

    return filesize;
}

void logT(char *desc, char *string) {
    Rio_writen(STDOUT_FILENO, desc, strlen(desc));
    Rio_writen(STDOUT_FILENO, string, strlen(string));
    Rio_writen(STDOUT_FILENO, "\n", strlen("\n"));
}
/**
 * Get the params from a query string
 */
struct Query get_params(char *query_string)
{
    int i = 0;
    int j;
    int length;
    char *next;
    char *dec;
    char *qs = malloc(strlen(query_string) + 1);
    char **params = malloc(0);
    struct Query q;

    // dont overwrite query string
    strcpy(qs, query_string);

    // split query string on vars
    next = strtok(qs, "&");

    while (next != NULL) {
        logT("Param: ", next);

        // dynamically allocate space... not efficient
        ++i;
        params = realloc(params, i * sizeof(char*) + 1);
        params[i] = next;

        // get next token from static buffer
        next = strtok(NULL, "&");
    }

    for (j = 0; j < ARRAY_LENGTH(params); j++) {
        q.params[j][0] = strtok(params[j], "=");
        q.params[j][1] = strtok(NULL, "=");
    }

    return q;
}

int main(int argc, char **argv)
{
    // Get ENV vars
    char dir[MAXLINE];
    char *template = getenv("FILENAME");
    char *query_string = getenv("QUERY_STRING");
    char *param_string;
    struct Query q;

    getcwd(dir, sizeof(dir));

    // Open dynamic template
    int tfd = Open(template, O_RDONLY, 0);
    FILE *template_file = fdopen(tfd, "rb");
    long int filesize = get_filesize(template_file);

    // read template to string buffer
    void *buffer = malloc(filesize);
    if (buffer) {
        fread(buffer, 1, filesize, template_file);
    }

    // replace all dynamic variables in templates
    if (buffer) {
        // get params
        get_params(query_string);

        // // loop through params and write to buffer
        // for (int i = 0; i < ARRAY_LENGTH(q.params); i++) {
        //     sprintf(param_string, "param: %s, value: %s\n", q.params[i][0], q.params[i][1]);
        //     Rio_writen(STDOUT_FILENO, param_string, strlen(param_string));
        // }
    }

    //printf("first param: %s\n\n", params[0]);
    // write to STDOUT_FILENO
    Rio_writen(STDOUT_FILENO, buffer, strlen(buffer));

    // Clean up
    Close(tfd);

    // return successful
    return 1;
}
