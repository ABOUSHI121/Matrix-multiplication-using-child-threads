#include <stdio.h>

#define SIZE 100 //size of the two square matrices

void fill_matrix(int matrix[SIZE][SIZE],long long int number)//this function take number and enter it respectively until the matrix finish
{
    long long int temp=number;

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            matrix[i][j]=temp%10;
            temp/=10;
            if (temp==0)temp=number;
        }
    }
}

void print_matrix(int  matrix [SIZE][SIZE])//function to print the matrix we give to it
{
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            printf("%d  ",matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}

void mull(int matrix_one[SIZE][SIZE],int matrix_two[SIZE][SIZE],int matrix_result[SIZE][SIZE])//function to calculate the mul result between two matrices
{
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            for (int k = 0; k < SIZE; ++k) {
                matrix_result[i][j]+=matrix_one[i][k]*matrix_two[k][j];
            }
        }
    }
}

int main() {

    int matrix_one [SIZE][SIZE],
        matrix_two[SIZE][SIZE],
        matrix_result[SIZE][SIZE];

    int id=8742121;
    long long int idy=4343958242;

    fill_matrix(matrix_one,id);
    fill_matrix(matrix_two,idy);
    fill_matrix(matrix_result,0);

    print_matrix(matrix_one);
    print_matrix(matrix_two);

    mull(matrix_one,matrix_two,matrix_result);

    print_matrix(matrix_result);

    return 0;
}
