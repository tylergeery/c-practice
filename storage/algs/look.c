#include <stdlib.h>
#include "../header.h"

int look(int start_pos, int* requests) {
    int visited, i, travel = 0, current = start_pos;
    int dir = 1;

    for (i = 0, visited = 0; visited < CYLINDER_REQUESTS; ) {
        i += dir;

        if (i < 0 || i >= CYLINDER_REQUESTS) {
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
