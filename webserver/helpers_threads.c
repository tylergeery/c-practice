#include "../csapp/csapp.h"

/**
 * Write an HTML error & HTTP headers to the client
 */
void clienterror(int fd, char *cause, char *errnum,
                    char *shortmsg, char *longmsg)
{
    char buf[MAXLINE], body[MAXBUF];

    /**
     * Build the HTTP response body
     */
    sprintf(body, "<html><title>Tiny Error</title>");
    sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
    sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
    sprintf(body, "%s<hr><em>The Tiny Web server</em>\r\n", body);

    /**
     * Print the HTTP response
     */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
    Rio_writen(fd, buf, strlen(buf));
    Rio_writen(fd, body, strlen(body)); // write response body finally
}

/**
 * Read the request headers and print to stdout
 * Does not actually use them
 */
void read_request_headers(rio_t *rp)
{
    char buf[MAXLINE];

    Rio_readlineb(rp, buf, MAXLINE);

    /**
     * Headers always terminate with WEB standard empty line `\r\n`
     *
     * Keep reading until then
     */
    while(strcmp(buf, "\r\n")) {
        Rio_readlineb(rp, buf, MAXLINE);
        printf("%s", buf);
    }

    return;
}

/**
 * Finds content to be delivered from uri
 * Returns 1 for static content, 0 for dynamic
 */
int parse_uri(char *uri, char *filename, char *dynamic_args)
{
    char *ptr;

    if (!strstr(uri, "dynamic")) {
        /**
         * Find static content
         */
        strcpy(dynamic_args, "");
        strcpy(filename, "./webserver/static");
        strcat(filename, uri);

        if (uri[strlen(uri)-1] == '/') {
            strcat(filename, "home.html");
        }

        return 1;
    } else {
        /**
         * Find Dynamic content
         */
         ptr = index(uri, '?');

         if (ptr) {
             strcpy(dynamic_args, ptr+1);
             *ptr = '\0';
         } else {
             strcpy(dynamic_args, "");
         }

         strcpy(filename, "./webserver");
         strcat(filename, uri);

         return 0;
    }
}

/**
 * Gets the HTTP mime type for filename
 */
void get_filetype(char *filename, char *filetype)
{
    char *suffix = strdup(filename) + 1; // in case first character is .
    char *filepath = strsep(&suffix, ".");

    if (strcasecmp(suffix,"html") == 0) {
        strcpy(filetype, "text/html");
    } else if (strcasecmp(suffix,"gif") == 0) {
        strcpy(filetype, "image/gif");
    } else if (strcasecmp(suffix,"jpg") == 0 || strcasecmp(suffix, "jpeg") == 0) {
        strcpy(filetype, "image/jpeg");
    } else if (strcasecmp(suffix,"css") == 0) {
        strcpy(filetype, "text/css");
    } else if (strcasecmp(suffix,"js") == 0) {
        strcpy(filetype, "text/javascript");
    } else {
        strcpy(filetype, "text/plain");
    }
}

/**
 * Serve static content
 * Writes HTTP headers for static content
 */
void serve_static(int fd, char *filename, int filesize)
{
    int srcfd;
    char *srcp, filetype[MAXLINE], buf[MAXLINE];

    /**
     * Send response headers to client
     */
    get_filetype(filename, filetype);
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
    sprintf(buf, "%sX-BullShit: %s\r\n", buf, filename);
    sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
    sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, filetype);
    Rio_writen(fd, buf, strlen(buf));

    /**
     * Send response body to client
     */

    // open file and get file descriptor
    srcfd = Open(filename, O_RDONLY, 0);

    // maps requested file to to private, read-only virtual memory
    // memory area starts srcp
    srcp = Mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);

    // closes file descriptor
    Close(srcfd);

    // writes virtual memory to output file descriptor
    Rio_writen(fd, srcp, filesize);

    // frees mapped virtual memory area
    Munmap(srcp, filesize);
}

void serve_dynamic(int fd, char *filename, char *dynamic_args)
{
    char buf[MAXLINE], *emptylist[] = { NULL };

    /**
     * Return the HTTP header
     */
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
    sprintf(buf, "%sX-BS: %s\r\n", buf, filename);
    sprintf(buf, "%sContent-type: text/html\r\n\r\n", buf);
    Rio_writen(fd, buf, strlen(buf));

    /** Render */
    render(fd, filename, dynamic_args);
}
