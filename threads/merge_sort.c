#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define THREAD_COUNT 3

typedef struct {
    int *arr;
    int count;
    int current;
} ThreadShare;

ThreadShare ThreadShare_New(ThreadShare ts, int length, int* unsorted)
{
    ts.current = 0;
    ts.count = length;
    ts.arr = (int *)malloc(length * sizeof(int));

    memcpy(ts.arr, unsorted, length * sizeof(int));

    return ts;
}

void ThreadShare_Free(ThreadShare* ts)
{
    free(ts->arr);
}

void sort(ThreadShare* ts)
{
    int i, j, tmp;

    for (i = 0; i < ts->count; ++i) {
        for (j = i + 1; j < ts->count; ++j) {
            if (ts->arr[i] > ts->arr[j]) {
                tmp = ts->arr[i];
                ts->arr[i] = ts->arr[j];
                ts->arr[j] = tmp;
            }
        }
    }
}

void *perform(void* arg)
{
    ThreadShare* ts = (ThreadShare *)arg;

    sort(ts);

    return NULL;
}

int getNext(ThreadShare* shared)
{
    int i, thread_id;
    int min = 0xFFFFFF;

    for (i = 0; i < THREAD_COUNT; i++) {
        if (shared[i].count == shared[i].current) {
            continue;
        }

        if (shared[i].arr[shared[i].current] < min) {
            min = shared[i].arr[shared[i].current];
            thread_id = i;
        }
    }

    shared[thread_id].current++;

    return min;
}

int main(int argc, char* argv[])
{
    int i;
    int size = (argc-1);
    int sorted[size];
    int per = (int)(size / THREAD_COUNT);
    int *nums = (int *)malloc(size * sizeof(int));

    ThreadShare *shared = (ThreadShare *)malloc(THREAD_COUNT * sizeof(ThreadShare));
    pthread_t threads[THREAD_COUNT];

    for (i = 1; i < argc; i++) {
        nums[i - 1] = atoi(argv[i]);
    }

    // Kick off a sort for reach individual thread
    for (i = 0; i < THREAD_COUNT; i++) {
        int* start = &nums[i * per];

        if (i + 1 == THREAD_COUNT) {
            per = size - (i * per);
        }

        if (per > 0) {
            shared[i] = ThreadShare_New(shared[i], per, start);
            pthread_create(&threads[i], NULL, perform, (void *)&shared[i]);
        }
    }

    // Wait for each thread to finish
    for (i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    // Merge Results of all threads individual sort
    i = 0;
    while (i < size) {
        printf("%d ", getNext(shared));
        i++;
    }

    printf("\n\n");

    for (i = 0; i < THREAD_COUNT; i++) {
        ThreadShare_Free(&shared[i]);
    }
    free(shared);

    return 0;
}
