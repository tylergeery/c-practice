#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#define STUDENT_COUNT 10
#define HALLWAY_CHAIRS 3

typedef struct {
    int count;
    enum {SLEEPING, TEACHING} state;
    pthread_cond_t available;
    pthread_cond_t done;
    pthread_mutex_t wake;
} TA;

typedef struct {
    TA *ta;
    int num;
    enum {PENDING, WAITING, LEARNING, HELPED} state;
} Student;

sem_t *hallway_chairs;

void *student_get_help(void *arg)
{
    int i = 0;
    int hallway_count;
    Student *student = (Student *)arg;

    // printf("Student %d spawned\n", student->num);
    while (1) {
        sem_getvalue(hallway_chairs, &hallway_count);

        // see if hallway spot is available
        if (student->state == PENDING && hallway_count < HALLWAY_CHAIRS) {
            int val = sem_wait(hallway_chairs);
            printf("Student %d is waiting in hallway\n", student->num);

            // wait til TA is available
            pthread_mutex_lock(&student->ta->wake);
            printf("TA State: %d %d %d\n", SLEEPING, TEACHING, student->ta->state);
            while(student->ta->state != SLEEPING)
                pthread_cond_wait(&student->ta->available, &student->ta->wake);
            pthread_mutex_unlock(&student->ta->wake);
            printf("Student %d is meeting with TA\n", student->num);

            // release hallway lock
            sem_post(hallway_chairs);
            student->state = LEARNING;

            // signal to those waiting
            pthread_cond_wait(&student->ta->done, NULL);
            student->state = HELPED;
            pthread_cond_broadcast(&student->ta->available);

            break;
        }

        printf("Student %d is for spot in hallway\n", student->num);
        sleep(rand() % 10);
    }

    printf("Student %d is finished and going home\n", student->num);
}

void *ta_sleep_and_give_help(void *arg)
{
    int helped = 0;
    TA *ta = (TA *)arg;

    while (helped < STUDENT_COUNT) {
        printf("TA Count(%d) waiting for a student\n", ta->count);

        // pthread_mutex_lock(&ta->wake);
        // pthread_cond_wait(&ta->available, &ta->wake);
        printf("TA Count(%d) found a student\n", ta->count);
        ta->state = TEACHING;
        // pthread_mutex_unlock(&ta->wake);

        // help current student
        sleep(rand() % 5);

        // pthread_mutex_lock(&ta->wake);
        ta->state = SLEEPING;
        // pthread_mutex_unlock(&ta->wake);

        // have student tell the hallway that ta is free
        pthread_cond_signal(&ta->done);

        helped++;
    }
}

int main(void)
{
    int i;
    TA ta;
    Student students[STUDENT_COUNT];

    pthread_t ta_thread;
    pthread_t student_threads[STUDENT_COUNT];

    ta.count = 10;
    ta.state = SLEEPING;
    if (pthread_mutex_init(&ta.wake, NULL) != 0) {
        printf("Error: Wake mutex could not be initialized\n");
        return errno;
    }

    if (pthread_cond_init(&ta.available, NULL) != 0) {
        printf("Error: TA Available condition could not be initialized\n");
        return errno;
    }

    if (pthread_cond_init(&ta.done, NULL) != 0) {
        printf("Error: TA Done condition could not be initialized\n");
        return errno;
    }

    hallway_chairs = sem_open("hallway_chairs", O_CREAT, S_IRUSR | S_IWUSR, HALLWAY_CHAIRS);
    if (errno != 0) {
        printf("Error: Hallway chairs semaphore could not be initialized\n");
        return errno;
    }

    pthread_create(&ta_thread, NULL, ta_sleep_and_give_help, (void *)&ta);
    for (i = 0; i < STUDENT_COUNT; i++) {
        students[i].state = PENDING;
        students[i].ta = &ta;
        students[i].num = i;

        pthread_create(&student_threads[i], NULL, student_get_help, (void *)&students[i]);
    }

    for (i = 0; i < STUDENT_COUNT; i++) {
        pthread_join(student_threads[i], NULL);
    }
    pthread_join(ta_thread, NULL);
}
