#include "../csapp/csapp.h"

/**
 * Write an HTML error & HTTP headers to the client
 */
void clienterror(int fd, char *cause, char *errnum,
                    char *shortmsg, char *longmsg)
{
    char buf[MAXLINE], body[MAXBUF]

    /**
     * Build the HTTP response body
     */
    sprintf(body, "<html><title>Tiny Error</title>");
    sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
    sprintf(body, "%s%s: $s\r\n", body, errnum, shortmsg);
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
        printf(%s, buf);
    }

    return;
}

/**
 * Finds content to be delivered from uri
 * Returns 1 for static content, 0 for dynamic
 */
void parse_uri(char *uri, char *filename, char *cgiargs)
{
    char *ptr;

    if (!strstr(uri, "cgi-bin")) {
        /**
         * Find static content
         */
        strcpy(cgiargs, "");
        strcpy(filename, "./static");
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
             strcpy(cgiargs, ptr+1);
             *ptr = '\0';
         } else {
             strcpy(cgiargs, "");
         }

         strcpy(filename, ".");
         strcat(filename, uri);

         return 0;
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
    get_filetype(filename, filetype) ;
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
    sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
    sprintf(buf, "%sContent-type: %d\r\n\r\n", buf, filetype);
    Rio_writen(fd, buf, strlen(buf));

    /**
     * Send response body to client
     */
    srcfd = Open(filename, O_RDONLY, 0);
    srcp = MMap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0); //?? TODO
    Close(srcfd);
    Rio_writen(fd, srcp, filesize);
    Munmap(srcp, filesize); //?? TODO
}

void serve_dynamic(int fd, char *filename, char *cgiargs)
{
    char buf[MAXLINE], *emptylist[] = { NULL };

    /**
     * Return first part of HTTP response
     */
}

/**
 * Gets the HTTP mime type for filename
 */
void get_filetype(char *filename, char *filetype)
{
    char *suffix = strfup(filename);
    char *filepath = strsep(&suffix, ".");

    switch (suffix) {
        case "html":
            strcpy(filetype, "text/html");
            break;
        case "gif":
            strcpy(filetype, "image/gif");
            break;
        case "jpg":
        case "jpeg":
            strcpy(filetype, "image/jpeg");
            break;
        case "css":
            strcpy(filetype, "text/css");
            break;
        case "js":
            strcpy(filetype, "text/javascript");
            break;
        default:
            strcpy(filetype, "text/plain");
    }
}
