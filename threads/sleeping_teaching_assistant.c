#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define STUDENT_COUNT = 10

typedef struct {
    enum state {SLEEPING, TEACHING};
    pthread_mutex_t wake;
    pthread_cond_t done;
} TA;

typedef struct {
    TA *ta;
    int num;
    enum state {PENDING, WAITING, LEARNING, HELPED};
} Student;

sem_t hallway_chairs;
Student* hallway[3];

void *student_get_help(void *arg)
{
    int hallway_count;
    Student *student = (Student *)arg;

    while (true) {
        sem_getvalue(&hallway_chairs, &hallway_count);

        // see if hallway spot is available
        if (student->state == PENDING && hallway_count < 3) {
            sem_wait(&hallway_chairs);
            student->state = WAITING;

            // wait til TA is available
            // acquire in-office lock
            pthread_mutex_lock(&student->ta->wake);

            while(student->ta->state != SLEEPING)
                pthread_cond_wait(&student->ta->available, &student->ta->wake);

            student->state = LEARNING;
            student->ta->state = TEACHING;

            pthread_mutex_unlock(&student->ta->wake);

            // release hallway lock
            sem_post(&hallway_chairs);

            // release ta lock
            // signal to those waiting
            pthread_cond_wait(&student->ta->done);
            student->state = HELPED;

            break;
        }

        printf("Student %d is waiting in state %s\n", student->num, student->state);
        sleep(rand() % 10);
    }

    printf("Student %d is finished and going home\n", student->num);
}

void *ta_sleep_and_give_help(void *arg)
{
    int helped = 0;
    TA *ta = (TA *)arg;

    while (helped < STUDENT_COUNT) {
        printf("TA waiting for a student\n");

        pthread_cond_wait(ta->available);

        pthread_mutex_lock(&ta->wake);
        ta->state = HELPING;
        pthread_mutex_unlock(&student->ta->wake);

        sleep(rand() % 5);

        // wait til TA is available
        // acquire in-office lock
        pthread_mutex_lock(&ta->wake);
        ta->state = SLEEPING;
        pthread_mutex_unlock(&student->ta->wake);
        pthread_cond_signal(&student->ta->done);

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

    pthread_mutex_init(&ta.wake, NULL);
    pthread_cond_init(&ta.cond, NULL);
    sem_init(&hallway_chairs, 0, 3);

    pthread_create(&ta_thread, NULL, ta_sleep_and_give_help, (void *)&ta);
    for (i = 0; i < STUDENT_COUNT; i++) {
        student[i].state = PENDING;
        student[i].ta = &ta;
        student[i].num = i;

        pthread_create(&student_threads[i], NULL, student_get_help, (void *)&students[i]);
    }

    for (i = 0; i < STUDENT_COUNT; i++) {
        pthread_join(student_threads[i], NULL);
    }
    pthread_join(ta_thread, NULL);
}
