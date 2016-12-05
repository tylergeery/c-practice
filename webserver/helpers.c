#include "../csapp/csapp.h"

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
