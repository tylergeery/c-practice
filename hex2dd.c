/**
 * hex2dd
 *
 * Takes hex value and converts to decimal dotted string
 * Build: gcc hex2dd.c -o exec/hex2dd
 * A&W: 11.2
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
         printf("\nInvalid arguments --- ./hex2dd <hex-value>\n\n");
         exit(0);
     }

     struct in_addr ip_obj;
     ip_obj.s_addr = strtol(argv[1], NULL, 16);
     printf("0x%x as ip: %s\n", ip_obj.s_addr, inet_ntoa(ip_obj));
 }
