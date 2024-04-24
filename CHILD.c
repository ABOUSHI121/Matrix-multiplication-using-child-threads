#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

#define SIZE 100 // size of the two square matrices
#define NUM_PROCESSES 5 // number of processes(child)

int id = 8742121; // id number flipped
long long int idy = 4343958242; // id * year of birth flipped
int matrix_one[SIZE][SIZE], matrix_two[SIZE][SIZE], matrix_result[SIZE][SIZE]; // matrices we will use (global to make it easier)

// Function to fill matrix one with id repeatedly until it's filled
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

// Function to fill matrix two with idy repeatedly until it's filled
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

// Function to fill matrix result with 0 repeatedly until it's filled
void fill_matrix_result() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            matrix_result[i][j] = 0;
        }
    }
}

// Function to print the matrix to make sure everything works right
void print_matrix(int matrix[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            printf("%d  ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}

// Function to calculate the multiplication for a specific range of rows
void multiply(int start_row, int end_row) {
    for (int i = start_row; i < end_row; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            for (int k = 0; k < SIZE; ++k) {
                matrix_result[i][j] += matrix_one[i][k] * matrix_two[k][j];
            }
        }
    }
}

int main() {
    clock_t start_time, end_time;
    double cpu_time_used;
    start_time = clock();

    pid_t pid;

    // create child processes to fill matrices
    if ((pid = fork()) == 0) {
        fill_matrix_one();
        _exit(0);
    } else {
        waitpid(pid, NULL, 0);
    }

    if ((pid = fork()) == 0) {
        fill_matrix_two();
        _exit(0);
    } else {
        waitpid(pid, NULL, 0);
    }

    if ((pid = fork()) == 0) {
        fill_matrix_result();
        _exit(0);
    } else {
        waitpid(pid, NULL, 0);
    }

    // create child processes to perform multiplication
    for (int i = 0; i < NUM_PROCESSES; ++i) {
        if ((pid = fork()) == 0) {
            int rows_per_process = SIZE / NUM_PROCESSES;
            int start_row = i * rows_per_process;
            int end_row = (i + 1) * rows_per_process;
            multiply(start_row, end_row);
            _exit(0);
        }
    }

    // Wait for all child processes to complete
    for (int i = 0; i < NUM_PROCESSES + 3; ++i) {
        wait(NULL);
    }

    end_time = clock();
    cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Matrix multiplication completed in %f seconds\n", cpu_time_used);

    // the print function takes time, so I commented it to measure the actual time
    // printf("Matrix multiplication completed\n");
    // print_matrix(matrix_result);

    return 0;
}
