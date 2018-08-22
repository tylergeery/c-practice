#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#define STUDENT_COUNT 10
#define HALLWAY_CHAIRS 3

typedef struct {
    enum {SLEEPING, TEACHING} state;
    pthread_cond_t available;
    pthread_cond_t active;
    pthread_mutex_t wake;
    pthread_cond_t done;
    pthread_mutex_t times_up;
} TA;

typedef struct {
    TA *ta;
    int num;
    enum {PENDING, WAITING, LEARNING, HELPED} state;
} Student;

sem_t hallway_chairs;

void *student_get_help(void *arg)
{
    int i = 0;
    Student *student = (Student *)arg;

    while (1) {
        // see if hallway spot is available
        if (student->state == PENDING) {
            int val = sem_wait(&hallway_chairs);
            printf("Student %d is waiting in hallway\n", student->num);

            // wait til TA is available
            pthread_mutex_lock(&student->ta->wake);
            printf("Student %d About to wake TA: %d\n", student->num, student->ta->state);
            while(student->ta->state != SLEEPING)
                pthread_cond_wait(&student->ta->available, &student->ta->wake);

            // wake up TA
            printf("Student %d woke up TA\n", student->num);
            student->ta->state = TEACHING;
            pthread_mutex_unlock(&student->ta->wake);
            printf("Student %d relinquishing hallway spot\n", student->num);

            // release hallway lock
            sem_post(&hallway_chairs);
            student->state = LEARNING;
            printf("Student %d is currently learning\n", student->num);
            pthread_cond_signal(&student->ta->active);

            // signal to those waiting
            pthread_cond_wait(&student->ta->done, &student->ta->times_up);
            printf("Student %d is done learning\n", student->num);
            student->state = HELPED;
            pthread_mutex_unlock(&student->ta->times_up);
            pthread_cond_signal(&student->ta->available);

            break;
        }

        printf("Student %d is waiting for spot in hallway %d\n", student->num, student->state == PENDING);
        sleep(rand() % 10);
    }

    printf("Student %d is finished and going home\n", student->num);
}

void *ta_sleep_and_give_help(void *arg)
{
    int helped = 0;
    TA *ta = (TA *)arg;

    while (helped < STUDENT_COUNT) {
        pthread_mutex_lock(&ta->wake);
        printf("TA waiting for a student\n");
        while (ta->state != TEACHING)
            pthread_cond_wait(&ta->active, &ta->wake);
        printf("TA found a student\n");
        pthread_mutex_unlock(&ta->wake);

        // help current student
        sleep((rand() % 5) + 1);

        printf("TA finishing with student\n");
        pthread_mutex_lock(&ta->wake);
        ta->state = SLEEPING;
        pthread_mutex_unlock(&ta->wake);

        // have student tell the hallway that ta is free
        printf("TA notifying student he is done\n");
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

    ta.state = SLEEPING;
    if (pthread_mutex_init(&ta.wake, NULL) != 0) {
        printf("Error: Wake mutex could not be initialized\n");
        return errno;
    }

    if (pthread_mutex_init(&ta.times_up, NULL) != 0) {
        printf("Error: TA Times up mutex could not be initialized\n");
        return errno;
    }

    if (pthread_cond_init(&ta.available, NULL) != 0) {
        printf("Error: TA Available condition could not be initialized\n");
        return errno;
    }

    if (pthread_cond_init(&ta.active, NULL) != 0) {
        printf("Error: TA active condition could not be initialized\n");
        return errno;
    }

    if (pthread_cond_init(&ta.done, NULL) != 0) {
        printf("Error: TA Done condition could not be initialized\n");
        return errno;
    }

    if (sem_init(&hallway_chairs, 0, HALLWAY_CHAIRS) != 0) {
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
