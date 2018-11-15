#include <stdlib.h>
#include "header.h"

int fcfs(int start_pos, int* requests) {
    int i, travel, current = start_pos;

    for (i = 0; i < CYLINDER_REQUESTS; i++) {
        travel += abs(current - requests[i]);
        current = requests[i];
    }

    return travel;
}
