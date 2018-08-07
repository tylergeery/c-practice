#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define THREAD_COUNT 100

pthread_t threads[THREAD_COUNT];
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
int sleep_total = 0;

void *perform()
{
    pthread_mutex_lock(&mtx);

    int sleep_time = rand() % 5;
    sleep(sleep_time);
    sleep_total += sleep_time;

    printf("Thread at location (#%lu), slept for %d seconds\n", (unsigned long)pthread_self(), sleep_time);

    pthread_mutex_unlock(&mtx);

    return NULL;
}

int main(void)
{
    int iter;

    printf("Creating threads\n");
    pthread_mutex_lock(&mtx);
    for (iter = 0; iter < THREAD_COUNT; iter++) {
        pthread_create(&threads[iter], NULL, perform, NULL);
    }
    printf("All %d threads created. Let them loose\n", THREAD_COUNT);
    pthread_mutex_unlock(&mtx);

    for (iter = 0; iter < THREAD_COUNT; iter++) {
        pthread_join(threads[iter], NULL);
    }

    printf("Slept for a total of %d seconds\n", sleep_total);

    return 0;
}
