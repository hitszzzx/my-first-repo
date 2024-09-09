#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define MATRIX_SIZE 1024

// Function to initialize matrices
void initialize_matrices(int A[MATRIX_SIZE][MATRIX_SIZE], int B[MATRIX_SIZE][MATRIX_SIZE], int C[MATRIX_SIZE][MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
            C[i][j] = 0;
        }
    }
}

// Function to perform naive matrix multiplication
void naive_gemm(int A[MATRIX_SIZE][MATRIX_SIZE], int B[MATRIX_SIZE][MATRIX_SIZE], int C[MATRIX_SIZE][MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            for (int k = 0; k < MATRIX_SIZE; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    int (*A)[MATRIX_SIZE] = malloc(sizeof(int[MATRIX_SIZE][MATRIX_SIZE]));
    int (*B)[MATRIX_SIZE] = malloc(sizeof(int[MATRIX_SIZE][MATRIX_SIZE]));
    int (*C)[MATRIX_SIZE] = malloc(sizeof(int[MATRIX_SIZE][MATRIX_SIZE]));

    if (A == NULL || B == NULL || C == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    // Initialize matrices
    srand(time(NULL));  // Seed for random number generation
    initialize_matrices(A, B, C);

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        free(A);
        free(B);
        free(C);
        exit(EXIT_FAILURE);
    }

    clock_t start, end;
    double cpu_time_used;

    if (pid == 0) {
        // Child process
        start = clock();
        naive_gemm(A, B, C);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        //printf("Child process CPU time: %f seconds\n", cpu_time_used);
        free(A);
        free(B);
        free(C);
        exit(EXIT_SUCCESS);
    } else {
        // Parent process
        wait(NULL); // Wait for child process to complete

        start = clock();
        naive_gemm(A, B, C);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        //printf("Parent process CPU time: %f seconds\n", cpu_time_used);

        // Display process tree
        //printf("Process tree:\n");
        //system("pstree -p");

        // Clean up
        free(A);
        free(B);
        free(C);
        exit(EXIT_SUCCESS);
    }

    return 0;
}
