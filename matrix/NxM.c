#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#define THREAD_COUNT 2
#define ARRAY_LENGTH(array) (sizeof((array))/sizeof((array)[0]))

int thread_indexes[THREAD_COUNT];
pthread_t threads[THREAD_COUNT];
sem_t semaphores[3][3];

int n[3][3] = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
};

int m[3][1] = {
    {2},
    {5},
    {3}
};

int result_matrix[ARRAY_LENGTH(n)][ARRAY_LENGTH(m[0])];
// 2*1+5*2+3*3 = 21
// 2*4+5*5+3*6 = 51
// 2*7+5*8+3*9 = 81
/**
 * Prints the usage instructions
 */
void usage(char file[]) {
    printf("Usage: %s <operation>\n", file);
}

void init() {
    for (int i = 0; i < ARRAY_LENGTH(result_matrix); i++) {
        for (int j = 0; j < ARRAY_LENGTH(result_matrix[i]); j++) {
            result_matrix[i][j] = 0;
            sem_init(&semaphores[i][j], 0, 1);
        }
    }
}

/**
 * Threaded function to add appropriate row values
 */
void *add(void *param) {
    int thread_index = *(int *)param;

    for (int i = thread_index; i < ARRAY_LENGTH(n); i += THREAD_COUNT) {
        for (int j = 0; j < ARRAY_LENGTH(n[i]); j++) {
            result_matrix[i][j] = n[i][j] + m[i][j];
        }
    }
}

/**
 * Threaded function to subtract appropriate row values
 */
void *subtract(void *param) {
    int thread_index = *(int *)param;

    for (int i = thread_index; i < ARRAY_LENGTH(n); i += THREAD_COUNT) {
        for (int j = 0; j < ARRAY_LENGTH(n[i]); j++) {
            result_matrix[i][j] = n[i][j] - m[i][j];
        }
    }
}

/**
 * Threaded function to multiply appropriate values
 *
 * Uses semaphores to ensure that multiple threads do not access the
 * same row/column value
 */
void *multiply(void *param) {
    int thread_index = *(int *)param;

    // for each row in result array (that applies to this thread)
    for (int i = thread_index; i < ARRAY_LENGTH(result_matrix); i += THREAD_COUNT) {
        for (int j = 0; j < ARRAY_LENGTH(result_matrix[i]); j++) {
            // lock ij

            // for each column in M, multiply its items by row in N
            int sum = 0;
            for (int k = 0; k < ARRAY_LENGTH(result_matrix); k++) {
                sum += n[i][k] * m[k][j];
            }
            result_matrix[i][j] = sum;
        }
    }
}

/**
 * Prints the result matrix
 */
void output() {
    printf("[ ");
    for(int i = 0, j = ARRAY_LENGTH(result_matrix); i < j; i++) {
        for (int k = 0, l = ARRAY_LENGTH(result_matrix[i]); k < l; k++) {
            printf("%d ", result_matrix[i][k]);
        }

        if (i != (j-1)) {
            printf("\n  ");
        }
    }
    printf("]\n");
}

int main(int argc, char *argv[]) {
    int i;
    void *func;

    if (argc != 2) {
        usage(argv[0]);
        return 0;
    }

    init();
    if (strcmp(argv[1], "add") == 0) {
        func = add;
    } else if (strcmp(argv[1], "subtract") == 0) {
        func = subtract;
    } else if (strcmp(argv[1], "multiply") == 0) {
        func = multiply;
    } else {
        usage(argv[0]);
        return 0;
    }

    for (i = 0; i < THREAD_COUNT; i++) {
        thread_indexes[i] = i;
        pthread_create(&threads[i], NULL, func, &thread_indexes[i]);
    }

    for (i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    output();

    return 0;
}
