#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <arpa/inet.h>

/**
 * Note: Output varies based on endianess of machine
 */
int main (int argc, char **argv)
{
    struct in_addr ip_obj;
    int nums[] = {0x0, 0xffffffff, 0x0100007f};
    char *strings[] = {"205.188.160.121", "64.12.149.13", "205.188.146.23"};
    int num_loops = sizeof(nums)/sizeof(nums[0]);
    int string_loops = sizeof(strings)/sizeof(strings[0]);

    for (int i = 0; i < num_loops; i++) {
        ip_obj.s_addr = nums[i];
        printf("%x as ip: %s\n", nums[i], inet_ntoa(ip_obj));
    }

    for (int j = 0; j < string_loops; j++) {
        int add_as_int = inet_aton(strings[j], &ip_obj);

        if (add_as_int) {
            printf("%s as int ip: %x\n", strings[j], ip_obj.s_addr);
        }
    }
}
