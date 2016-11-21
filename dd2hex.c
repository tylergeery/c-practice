/**
 * hex2dd
 *
 * Takes decimal-dotted argument and prints its hex value
 * Build: gcc dd2hex.c -o exec/dd2hex
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <sys/stat.h>
 #include <arpa/inet.h>

 /**
  * Note: Output varies based on endianess of machine
  */
 int main (int argc, char **argv)
 {
     if (argc <= 1) {
         printf("\nInvalid arguments --- ./dd2hex <decimal-dotted-string>\n\n");
         exit(0);
     }

     struct in_addr ip_obj;
     int success = inet_aton(argv[1], &ip_obj);

    if (success) {
        printf("%s as hex value: 0x%x\n", argv[1], ip_obj.s_addr);
    }
 }
