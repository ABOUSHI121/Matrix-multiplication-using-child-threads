#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define SIZE 100 // size of the two square matrices
#define NUM_THREADS 5// number of threads here we can change number of threads to see the effect

int id = 8742121;//id number flipped
long long int idy = 4343958242;//id * year of birth flipped
int matrix_one[SIZE][SIZE], matrix_two[SIZE][SIZE], matrix_result[SIZE][SIZE];//matrices we will use(global to make it easier)

// Structure to pass arguments to the thread function
struct ThreadData {//here we can make every thread do the multiplication to specified rows
    int start_row;
    int end_row;
};

void fill_matrix_one() {//fill matrix one with id repeatedly until its filled
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

void fill_matrix_two() {//fill matrix one with idy repeatedly until its filled
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

void fill_matrix_result() {//fill matrix one with 0 repeatedly until its filled
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            matrix_result[i][j] = 0;
        }
    }
}

void print_matrix(int matrix[SIZE][SIZE]) {//print the matrix to make sure every thing work right
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            printf("%d  ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}

// Function to calculate the multiplication for a specific range of rows
void* multiply(void* arg) {//this is the function of multiplication that every thread make for the range start row and end row
    struct ThreadData* data = (struct ThreadData*)arg;

    for (int i = data->start_row; i < data->end_row; ++i) {//we devide the matrix_result to row so we do all columns
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

    pthread_t thread1, thread2, thread3;//this threads will fill the 3 matrices individually to make the program faster
    pthread_create(&thread1, NULL, fill_matrix_one, NULL);
    pthread_create(&thread2, NULL, fill_matrix_two, NULL);
    pthread_create(&thread3, NULL, fill_matrix_result, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    pthread_t threads[NUM_THREADS];//array of threads
    struct ThreadData threadData[NUM_THREADS];//array of threads data with the same sizes of threads

    int rows_per_thread = SIZE / NUM_THREADS;//the number of rows to each thread
    for (int i = 0; i < NUM_THREADS; ++i) {//this loop give every thread the start and end rows and create the thread to do the function of multiplication
        threadData[i].start_row = i * rows_per_thread;
        threadData[i].end_row = (i + 1) * rows_per_thread;
        pthread_create(&threads[i], NULL, multiply, (void*)&threadData[i]);
    }

    for (int i = 0; i < NUM_THREADS; ++i) {//this is to join the threads
        pthread_join(threads[i], NULL);
    }

    end_time = clock();
    cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Matrix multiplication completed in %f seconds\n", cpu_time_used);
    // the print function take time, so I commented it to measure the actual time
    //printf("Matrix multiplication completed\n");
   // print_matrix(matrix_result);

    return 0;
}
