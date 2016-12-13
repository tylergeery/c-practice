#include "../csapp/csapp.h"
#include <stdio.h>


int main(int argc, char **argv)
{
    // Get ENV vars
    char *template = getenv("FILENAME");
    char *query_string = gentenv("QUERY_STRING");
    char *tmp;
    char **params;

    // Open dynamic template
    int tfd = Open(template, O_RDONLY, 0)
    FILE *template_string = fdopen(tfd, "rb");
    long int filesize = get_filesize(template_string);

    // read template to string buffer
    buffer = malloc(filesize);
    if (buffer) {
        fread(buffer, 1, filesize, template_string);
    }

    // replace all dynamic variables in templates
    if (buffer) {
        // copy over query string to be tokenized
        strcpy(tmp, query_string);

        // get params
        params = get_params(tmp);

        // loop through params and write to buffer
    }

    // write to STDOUT_FILENO
    Rio_writen(STDOUT_FILENO, buffer, strlen(buffer));

    // Clean up
    Close(tfd);

    // return successful
    return 1;
}

/**
 * Gets the passed file's size
 */
long int get_filesize(FILE *stream)
{
    fseek(stream, 0L, SEEK_END);
    long int filesize = ftell(stream);
    rewind(fp);

    return filesize;
}

/**
 * Get the params from a query string
 */
char ** get_params(char * query_string)
{
    int i = 0;
    char *p, next;
    char **params;

    // split query string on vars
    while ((next = strtok(query_string, "&")) != NULL) {
        int j = 0;
        char *values[2];

        while ((p = strtok(next, "=")) != NULL) {
            values[j++] = p;
        }

        params[i++] = values;
    }

    return params;
}
