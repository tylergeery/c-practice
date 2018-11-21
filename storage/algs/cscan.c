#include <stdlib.h>
#include "../header.h"

int cscan(int start_pos, int* requests) {
    int visited = 0, i = start_pos, travel = 0;

    sort_requests(requests);
    int next = find_starting_bound(requests, start_pos);

    for (; visited < CYLINDER_REQUESTS; ) {
        while (requests[next] == i) {
            visited++;
            requests[next] = -1;
            next = (next+1) % CYLINDER_REQUESTS;
        }

        travel += 1;
        i = (i+1) % CYLINDER_COUNT;
        if (i == 0) {
            travel += CYLINDER_COUNT;
        }
    }

    return travel;
}
