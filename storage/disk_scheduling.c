#include <stdlib.h>
#include <stdio.h>
#include "header.h"

void help(char* file_path) {
    printf("%s <start_pos> - Calculate the total cylinder travel length for 5000 disk requests.\n\tstart_pos - number between 0 and 4999\n", file_path);
}

void generate_cylinders(int* cylinders) {
    int i;

    for (i = 0; i < CYLINDER_REQUESTS; i++) {
        cylinders[i] = rand() % CYLINDER_COUNT;
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        help(argv[0]);
        return -1;
    }

    int start_pos = atoi(argv[1]);
    if (start_pos < 0 || start_pos > (CYLINDER_COUNT - 1)) {
        help(argv[0]);
        return -1;
    }

    int requests[CYLINDER_REQUESTS];
    generate_cylinders(requests);

    int fcfs_count = fcfs(start_pos, requests);
    int sstf_count = 0; // TODO
    int scan_count = 0; // TODO
    int cscan_count = 0; // TODO
    int look_count = 0; // TODO
    int clook_count = 0; // TODO

    // Calculate travel for all algorithms
    printf(
        "\nFCFS: %d\nSSTF: %d\nSCAN: %d\nC-SCAN: %d\nLOOK: %d\nC-LOOK%d\n\n",
        fcfs_count, sstf_count, scan_count, cscan_count, look_count, clook_count
    );

    return 0;
}
