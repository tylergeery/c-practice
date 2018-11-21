#include <stdlib.h>
#include "../header.h"

int scan(int start_pos, int* requests) {
    int visited = 0, i = start_pos, travel = 0, dir = 1;

    sort_requests(requests);
    int start = find_starting_bound(requests, start_pos);
    int next = start;

    for (; visited < CYLINDER_REQUESTS; ) {
        while (requests[next] == i) {
            visited++;
            requests[next] = -1;
            next += dir;
        }

        travel += 1;
        i = (i+dir);
        if (i == CYLINDER_COUNT) {
            i--;
            dir *= -1;
            next = start - 1;
        }
    }

    return travel;
}
