#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


#define ARRAY_LENGTH(array) (sizeof((array))/sizeof((array)[0]))

int answers[3];
int size;

void max(void *args)
{
    int i, max, num;
    int *nums = (int *)args;

    for (i = 0; i < size; i++) {
        if (nums[i] > max) {
            max = nums[i];
        }
    }

    answers[0] = max;
}

void avg(void *args)
{
    int i, sum, num;
    int *nums = (int *)args;

    for (i = 0; i < size; i++) {
        sum += nums[i];
    }

    answers[1] = (int)(sum / size);
}

void min(void *args)
{
    int i, num;
    int *nums = (int *)args;
    int min = nums[0];

    for (i = 0; i < size; i++) {
        if (nums[i] < min) {
            min = nums[i];
        }
    }

    answers[2] = min;
}

void (*funcs[3]) = {max, avg, min};
pthread_t threads[ARRAY_LENGTH(funcs)];

int main(int argc, char *argv[])
{
    int iter;
    int *nums = (int *)malloc((argc - 1) * sizeof(int));

    size = (argc-1);

    for (iter = 1; iter < argc; iter++) {
        nums[iter - 1] = atoi(argv[iter]);
    }

    for (iter = 0; iter < ARRAY_LENGTH(funcs); iter++) {
        pthread_create(&threads[iter], NULL, funcs[iter], (void *)nums);
    }

    for (iter = 0; iter < ARRAY_LENGTH(funcs); iter++) {
        pthread_join(threads[iter], NULL);
    }

    printf("\nThe average value is %d\nThe minimum value is %d\nThe maximum value is %d\n\n", answers[1], answers[2], answers[0]);

    return 0;
}
