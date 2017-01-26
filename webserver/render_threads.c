#include "../csapp/csapp.h"
#include <stdio.h>

struct Query {
    char *params[10][2];
    int param_count;
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

void logStr(char *desc, char *string) {
    char output[MAXLINE];
    sprintf(output, "%s: %s\n", desc, string);
    Rio_writen(STDOUT_FILENO, output, strlen(output));
}

void logPtr(char *desc, char *ptr) {
    char string[MAXLINE];
    sprintf(string, "%s Address: %p\n", desc, ptr);
    Rio_writen(STDOUT_FILENO, string, strlen(string));
}

void logInt(char *desc, int i) {
    char string[MAXLINE];
    sprintf(string, "%d\n", i);
    Rio_writen(STDOUT_FILENO, string, strlen(string));
}

/**
 * Get the params from a query string
 */
struct Query get_params(char *query_string)
{
    int j;
    char *next;
    char *qs = (char *)malloc(sizeof(query_string));
    char **params = (char **)malloc(0);
    struct Query q;
    q.param_count = 0;

    // dont overwrite query string
    strcpy(qs, query_string);

    // split query string on vars
    next = strtok(qs, "&");

    while (next != NULL) {
        // dynamically allocate space... not efficient
        params = realloc(params, (q.param_count+1) * sizeof(char*));
        params[q.param_count] = (char *)malloc(sizeof(char));

        // copy to params
        strcpy(params[q.param_count], next);

        // get next token from static buffer
        next = strtok(NULL, "&");
        q.param_count++;
    }

    for (j = 0; j < q.param_count; j++) {
        // dont overwrite param
        char *string = (char *)malloc(sizeof(params[j]));
        strcpy(string, params[j]);

        q.params[j][0] = strtok(string, "=");
        q.params[j][1] = strtok(NULL, "=");
    }

    return q;
}

char *replace_var(char *str, char *orig, char *rep)
{
    char *buffer = malloc(MAXBUF);
    char *pos;

    // Is 'orig' even in 'str'?
    if(!(pos = strstr(str, orig))) {
        return str;
    }

    // Copy characters from 'str' start to 'orig' st$
    strncpy(buffer, str, pos - str);
    buffer[pos - str] = '\0';

    sprintf(buffer + (pos - str), "%s%s", rep, pos + strlen(orig));

    return buffer;
}

int render(int fd, char *template, char *query_string)
{
    char dir[MAXLINE];
    struct Query q;

    getcwd(dir, sizeof(dir));

    // Open dynamic template
    int tfd = Open(template, O_RDONLY, 0);
    FILE *template_file = fdopen(tfd, "rb");
    long int filesize = get_filesize(template_file);

    // read template to string buffer
    char *buffer = malloc(MAXBUF);
    fread(buffer, 1, filesize, template_file);

    // get params
    q = get_params(query_string);

    // // loop through params and write to buffer
    for (int i = 0; i < q.param_count; i++) {
        char replace_val[MAXLINE];
        sprintf(replace_val, "{{%s}}", q.params[i][0]);

        // replace all dynamic variables in templates
        if (q.params[i][1] != NULL) {
            char *new_template = malloc(MAXBUF);
            while ((new_template = replace_var(buffer, replace_val, q.params[i][1])) != buffer) {
                buffer = new_template;
            }
        }
    }

    // write to passed fd
    Rio_writen(fd, buffer, strlen(buffer));

    // Clean up
    Close(tfd);

    // return successful
    return 1;
}
