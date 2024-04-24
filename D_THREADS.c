#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define SIZE 100
#define NUM_THREADS 4

int id = 8742121;
long long int idy = 4343958242;
int matrix_one[SIZE][SIZE], matrix_two[SIZE][SIZE], matrix_result[SIZE][SIZE];

struct ThreadData {
    int start_row;
    int end_row;
};

void fill_matrix_one() {
    int temp = id;

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            matrix_one[i][j] = temp % 10;
            temp /= 10;
            if (temp == 0)
                temp = id;
        }
    }
}

void fill_matrix_two() {
    long long int temp = idy;

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            matrix_two[i][j] = temp % 10;
            temp /= 10;
            if (temp == 0)
                temp = idy;
        }
    }
}

void fill_matrix_result() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            matrix_result[i][j] = 0;
        }
    }
}

void print_matrix(int matrix[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            printf("%d  ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}

void* multiply(void* arg) {
    struct ThreadData* data = (struct ThreadData*)arg;

    for (int i = data->start_row; i < data->end_row; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            for (int k = 0; k < SIZE; ++k) {
                matrix_result[i][j] += matrix_one[i][k] * matrix_two[k][j];
            }
        }
    }

    pthread_exit(NULL);
}

int main() {
    clock_t start_time, end_time;
    double cpu_time_used;
    start_time = clock();

    pthread_t thread1, thread2, thread3;
    pthread_create(&thread1, NULL, fill_matrix_one, NULL);
    pthread_create(&thread2, NULL, fill_matrix_two, NULL);
    pthread_create(&thread3, NULL, fill_matrix_result, NULL);

    // Detach threads that fill matrices, as they don't need to be explicitly joined
    pthread_detach(thread1);
    pthread_detach(thread2);
    pthread_detach(thread3);

    pthread_t threads[NUM_THREADS];
    struct ThreadData threadData[NUM_THREADS];

    int rows_per_thread = SIZE / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; ++i) {
        threadData[i].start_row = i * rows_per_thread;
        threadData[i].end_row = (i + 1) * rows_per_thread;
        pthread_create(&threads[i], NULL, multiply, (void*)&threadData[i]);
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_detach(threads[i]);  // Detach threads for matrix multiplication
    }

    end_time = clock();
    cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Matrix multiplication completed in %f seconds\n", cpu_time_used);

    // The print function takes time, so it's commented to measure the actual time
    //printf("Matrix multiplication completed\n");
    print_matrix(matrix_result);

    return 0;
}
