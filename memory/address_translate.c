#include <math.h>
#include <stdio.h>
#include <string.h>

#define PAGE_SIZE 4096

void help(char **argv) {
    printf("%s <hex> - Accepts a single valid hex value as a parameter and translates to page number and offset. Assumes 4-KB page size.\n", argv[0]);
}

int parseHex(char *hex, unsigned int *value) {
    int i; int ascii;
    int s = strlen(hex) - 1;

    for (i = s; i >= 0; i--) {
        ascii = hex[i] - '0';

        if (ascii > 48 && ascii < 55) {
            ascii -= 39;
        }

        if (ascii < 0 || ascii > 15) {
            *value = -1;
            return -1;
        }

        *value += (pow(16, s-i) * ascii);
    }

    return 0;
}

void output(unsigned int value) {
    int page = value / PAGE_SIZE;
    int offset = value % PAGE_SIZE;

    printf("The address %d contains:\npage number = %d\noffset = %d\n", value, page, offset);
}

int main(int argc, char **argv) {
    unsigned int address;
    int success;

    if (argc < 2) {
        help(argv);
        return 1;
    }

    success = parseHex(argv[1], &address);

    if (success < 0) {
        help(argv);
        return success;
    }

    output(address);

    return 0;
}
