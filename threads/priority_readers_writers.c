#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#define READER_COUNT 5
#define WRITER_COUNT 5
#define WORK_COUNT 20

int reader_count;
int value;

pthread_mutex_t reader_count_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t value_lock = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t reader_ready = PTHREAD_COND_INITIALIZER;
pthread_cond_t writer_ready = PTHREAD_COND_INITIALIZER;

typedef struct {
    int num;
    int value;
} writer_args;

typedef struct {
    int num;
} reader_args;

void *read_job(void *param)
{
    int i;
    reader_args *args = (reader_args *)param;

    for (i=0; i < WORK_COUNT; i++) {
        sleep(rand() % 5);
        printf("Reader %d ready for work\n", args->num);

        pthread_mutex_lock(&reader_count_lock);
        reader_count++;
        pthread_mutex_unlock(&reader_count_lock);

        pthread_mutex_lock(&value_lock);
        while (value == 0) {
            pthread_cond_wait(&reader_ready, &value_lock);
        }
        printf("Reader %d reading value = %d, reader count = %d\n", args->num, value, reader_count);
        pthread_mutex_unlock(&value_lock);

        pthread_mutex_lock(&reader_count_lock);
        reader_count--;
        pthread_mutex_unlock(&reader_count_lock);
    }

    printf("Reader %d quitting\n", args->num);
    return NULL;
}

void *write_job(void *param)
{
    int i;
    writer_args *args = (writer_args *)param;

    for (i=0; i < WORK_COUNT; i++) {
        sleep(rand() % 5);
        printf("Writer %d ready for work\n", args->num);

        pthread_mutex_lock(&reader_count_lock);
        while (reader_count > 0) {
            pthread_cond_wait(&writer_ready, &reader_count_lock);
        }

        pthread_mutex_lock(&value_lock);
        value = args->value;
        printf("Writer %d Setting value = %d, reader count = %d\n", args->num, args->value, reader_count);
        pthread_mutex_lock(&value_lock);

        pthread_mutex_unlock(&reader_count_lock);
    }

    printf("Writer %d quitting\n", args->num);
    return NULL;
}

int main()
{
    int i;
    writer_args writer_arg_list[WRITER_COUNT];
    reader_args reader_arg_list[WRITER_COUNT];
    pthread_t writer_threads[WRITER_COUNT];
    pthread_t reader_threads[READER_COUNT];

    for (i = 0; i < WRITER_COUNT; i++) {
        writer_arg_list[i].num = i;
        writer_arg_list[i].value = rand();

        if (writer_arg_list[i].value == 0) {
            writer_arg_list[i].value = 1;
        }
    }

    for (i = 0; i < READER_COUNT; i++) {
        reader_arg_list[i].num = i;
    }

    for (i = 0; i < WRITER_COUNT; i++) {
        pthread_create(&writer_threads[i], NULL, write_job, &writer_arg_list[i]);
    }

    for (i = 0; i < READER_COUNT; i++) {
        pthread_create(&reader_threads[i], NULL, read_job, &reader_arg_list[i]);
    }

    for (i = 0; i < READER_COUNT; i++) {
        pthread_join(reader_threads[i], NULL);
    }

    for (i = 0; i < WRITER_COUNT; i++) {
        pthread_join(writer_threads[i], NULL);
    }

    return 0;
}
