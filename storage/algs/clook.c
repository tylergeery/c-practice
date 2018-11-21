#include <stdlib.h>
#include "../header.h"

int clook(int start_pos, int* requests) {
    int visited, i, travel = 0, current = start_pos;
    int dir = 1;

    for (i = 0, visited = 0; visited < CYLINDER_REQUESTS; i += 1) {
        if (i >= CYLINDER_REQUESTS) {
            i = -1;
            dir *= -1;
            continue;
        }

        if (should_serve(dir, requests[i], current)) {
            travel += abs(current - requests[i]);
            current = requests[i];
            visited++;
            requests[i] = -1;
        }
    }

    return travel;
}
