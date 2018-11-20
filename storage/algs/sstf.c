#include <stdlib.h>
#include <stdio.h>
#include "../header.h"

int sstf(int start_pos, int* requests) {
    int i, next, travel = 0, current = start_pos;

    sort_requests(requests);

    int upper_bound = find_starting_bound(requests, start_pos);
    int lower_bound = upper_bound - 1;

    for (i = 0; i < CYLINDER_REQUESTS; i++) {
        int to_lower = get_bound(requests, lower_bound, current);
        int to_upper = get_bound(requests, upper_bound, current);

        if (to_lower <= to_upper) {
            next = lower_bound;
            lower_bound--;
        } else {
            next = upper_bound;
            upper_bound++;
        }

        travel += abs(requests[next] - current);
        current = requests[next];
    }

    return travel;
}
