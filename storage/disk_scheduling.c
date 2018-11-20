#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "header.h"

void help(char* file_path) {
    printf("%s <start_pos> - Calculate the total cylinder travel length for 5000 disk requests.\n\tstart_pos - number between 0 and 4999\n", file_path);
}

void sort_requests(int *requests) {
    int i, j;

    for (i = 0; i < CYLINDER_REQUESTS; i++) {
        for (j = i; j < CYLINDER_REQUESTS; j++) {
            if (requests[i] > requests[j]) {
                int tmp = requests[i];
                requests[i] = requests[j];
                requests[j] = tmp;
            }
        }
    }
}

int find_starting_bound(int* requests, int start_pos) {
    for (int i = 0; i < CYLINDER_REQUESTS; i++) {
        if (requests[i] >= start_pos) {
            return i;
        }
    }

    return 1;
}

int get_bound(int* requests, int index, int value) {
    if (index < 0 || index >= CYLINDER_REQUESTS) {
        return CYLINDER_COUNT + 1;
    }

    return abs(requests[index] - value);
}

void generate_cylinders(int* cylinders) {
    int i;

    int seed = time(NULL);
    srand(seed);

    for (i = 0; i < CYLINDER_REQUESTS; i++) {
        cylinders[i] = rand() % CYLINDER_COUNT;
    }
}

void copy_cylinders(int* source, int* dest) {
    int i;

    for (i = 0; i < CYLINDER_REQUESTS; i++) {
        dest[i] = source[i];
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

    int base_requests[CYLINDER_REQUESTS];
    int requests[CYLINDER_REQUESTS];

    generate_cylinders(base_requests);

    copy_cylinders(base_requests, requests);
    int fcfs_count = fcfs(start_pos, requests);
    copy_cylinders(base_requests, requests);
    int sstf_count = sstf(start_pos, requests);
    copy_cylinders(base_requests, requests);
    int scan_count = scan(start_pos, requests);
    copy_cylinders(base_requests, requests);
    int cscan_count = 0; // TODO
    copy_cylinders(base_requests, requests);
    int look_count = 0; // TODO
    copy_cylinders(base_requests, requests);
    int clook_count = 0; // TODO

    // Calculate travel for all algorithms
    printf(
        "\nFCFS: %d\nSSTF: %d\nSCAN: %d\nC-SCAN: %d\nLOOK: %d\nC-LOOK: %d\n\n",
        fcfs_count, sstf_count, scan_count, cscan_count, look_count, clook_count
    );

    return 0;
}
