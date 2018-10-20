#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define PAGE_REF_STR_LENGTH 30

typedef struct {
    char value;
    int last_use;
} Entry;

void usage(char* file, char* message) {
    fprintf(stderr, "Invalid: %s\n", message);
    printf("usage: %s <frame_count>\n", file);
}

void generateReferenceString(char* str, int l) {
    int i;

    for (i = 0; i <= l; i++) {
        str[i] = (rand() % 10) + '0';
    }
}

int findNextUse(char *str, char c, int i, int l) {
    for (int j = i+1; j < l; j++) {
        if (c == str[j]) {
            return j;
        }
    }

    return l; // not used again
}

/**
 * First In, First Out Page Replacement
 *
 * This will cycle through page frames and replacing the oldest existing
 * frame whenever it requires a page replacement
 * meaning in the case of 34563122 with frames (4)
 * '3' should be removed from frames before '4' since '3' was the first frame put in
 * by the time we get to '1'
 */
int fifo(char* str, int l, int frames) {
    int i, j, iter = 0, misses = 0;
    int frameList[frames];

    for (i = 0; i < l; i++) {
        int found = 0;

        // check if value is in frameList
        for (j = 0; j < frames; j++) {
            if (frameList[j] == str[i]) {
                // in cache, we can continue
                found = 1;
                break;
            }
        }

        // check if page replacement necessary
        if (!found) {
            frameList[iter] = str[i];
            iter = (iter+1) % frames;
            misses++;
        }
    }

    return misses;
}

/**
 * Least Recently Used Page Replacement
 *
 * This should replace the page that is most recently used,
 * meaning in the case of 34563122 with frames (4)
 * '4' should be removed from frames before '3' since '3' was more recently used
 * by the time we get to '1'
 */
int lru(char *str, int l, int frames) {
    int i, j, last, recent, misses = 0;
    Entry frameList[frames];

    for (i = 0; i < frames; i++) {
        Entry ent = {'\0', 0};
        frameList[i] = ent;
    }

    for (i = 0; i < l; i++) {
        int found = 0;

        // check if value is in frameList
        for (j = 0; j < frames; j++) {
            if (frameList[j].value == str[i]) {
                // in cache, we can continue
                found = 1;
                frameList[j].last_use = i;
                break;
            }
        }

        // check if page replacement necessary
        if (!found) {
            last = 0, recent = l - 1;

            for (j = 0; j < frames; j++) {
                if (frameList[j].value == '\0') {
                    last = j;
                    break;
                }

                if (frameList[j].last_use < recent) {
                    last = j;
                    recent = frameList[j].last_use;
                }
            }

            Entry entry = {str[i], i};
            frameList[last] = entry;
            misses++;
        }
    }

    return misses;
}

/**
 * Optimal Page Replacement
 *
 * This looks forward to see which page will be used
 * and using this as a heuristic for which frame to replace
 *
 * This is, of course, not realistic but it does give us a bound for best case
 */
int optimal(char *str, int l, int frames) {
    int i, j, last, optimal, misses = 0;
    Entry frameList[frames];

    for (i = 0; i < frames; i++) {
        Entry ent = {'\0', 0};
        frameList[i] = ent;
    }

    for (i = 0; i < l; i++) {
        int found = 0;

        // check if value is in frameList
        for (j = 0; j < frames; j++) {
            if (frameList[j].value == str[i]) {
                // in cache, we can continue
                found = 1;
                frameList[j].last_use = i;
                break;
            }
        }

        // check if page replacement necessary
        if (!found) {
            last = 0, optimal = 0;

            for (j = 0; j < frames; j++) {
                if (frameList[j].value == '\0') {
                    last = j;
                    break;
                }

                int next = findNextUse(str, frameList[j].value, i, l);
                if (next > optimal) {
                    last = j;
                    optimal = next;
                }
            }

            Entry entry = {str[i], i};
            frameList[last] = entry;
            misses++;
        }
    }

    return misses;
}

int main(int argc, char** argv) {
    int frames;

    if (argc < 2) {
        usage(argv[0], "Must supply frame count");
        return -1;
    }

    frames = atoi(argv[1]);
    if (frames <= 0 || frames > 7) {
        usage(argv[0], "Frame count must be between 1-7");
        return -1;
    }

    // seed random
    srand(time(NULL));

    char ref_string[PAGE_REF_STR_LENGTH] = "";
    generateReferenceString(ref_string, PAGE_REF_STR_LENGTH);

    int fifo_faults = fifo(ref_string, PAGE_REF_STR_LENGTH, frames);
    int lru_faults = lru(ref_string, PAGE_REF_STR_LENGTH, frames);
    int optimal_faults = optimal(ref_string, PAGE_REF_STR_LENGTH, frames);

    printf("\nReference string: %s\n\nPage replacement faults:\n\tFIFO: %d\n\tLRU: %d\n\toptimal: %d\n\n\n", ref_string, fifo_faults, lru_faults, optimal_faults);
}
