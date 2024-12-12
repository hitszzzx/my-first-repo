#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int block_size ;

void reorder_matrix(int rows, int cols, double orig_matrix[rows][cols], double reordered_matrix[rows][cols], int block_size) {
    for (int ii = 0; ii < rows; ii += block_size) {
        for (int jj = 0; jj < cols; jj += block_size) {
            for (int i = ii; i < ii + block_size && i < rows; i++) {
                for (int j = jj; j < jj + block_size && j < cols; j++) {
                    reordered_matrix[i][j] = orig_matrix[i][j];
                }
            }
        }
    }
}

void naive_dgemm(int m, int n, int k, double A[m][k], double B[k][n], double C[m][n]) {
     for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0;
        }
    }

    // 分块乘法
    for (int ii = 0; ii < m; ii += block_size) {
        for (int jj = 0; jj < n; jj += block_size) {
            for (int kk = 0; kk < k; kk += block_size) {

                for (int i = ii; i < ii + block_size && i < m; i++) {
                    for (int j = jj; j < jj + block_size && j < n; j++) {
                        double sum = C[i][j]; 
                        for (int p = kk; p < kk + block_size && p < k; p++) {
                            sum += A[i][p] * B[p][j];
                        }
                        C[i][j] = sum;
                    }
                }

            }
        }
    }
}

double calculate_gflops(int m, int n, int k, double time) {
    return (2.0 * m * n * k) / (time * 1e9); // 计算GFLOPS
}

void print_matrix(int rows, int cols, double matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%lf ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    int sizes[] = { 320, 640, 1000, 4000, 8000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    block_size = 64;

    for (int s = 0; s < num_sizes; s++) {
        int m = sizes[s];
        int n = sizes[s];
        int k = sizes[s];

        // 动态分配矩阵
        double (*A)[k] = malloc(m * sizeof(*A));
        double (*B)[n] = malloc(k * sizeof(*B));
        double (*C)[n] = malloc(m * sizeof(*C));

        double (*A_reordered)[k] = malloc(m * sizeof(*A_reordered));
        double (*B_reordered)[n] = malloc(k * sizeof(*B_reordered));


        // 随机初始化矩阵A和B
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < k; j++) {
                A[i][j] = (double)(rand() % 10);
            }
        }
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < n; j++) {
                B[i][j] = (double)(rand() % 10);
            }
        }
        reorder_matrix(m, k, A, A_reordered, block_size);
        reorder_matrix(k, n, B, B_reordered, block_size);
        // 记录开始时间
        clock_t start = clock();
        // 执行矩阵乘法
        naive_dgemm(m, n, k, A_reordered, B_reordered, C);
        // 记录结束时间
        clock_t end = clock();

        // 计算执行时间
        double time_taken = (double)(end - start) / CLOCKS_PER_SEC;
        // 计算GFLOPS
        double gflops = calculate_gflops(m, n, k, time_taken);

        // 打印结果
        printf("Matrix size: %d x %d\n", m, n);
        printf("Time taken: %f seconds\n", time_taken);
        printf("GFLOPS: %f\n\n", gflops);

        // 释放动态分配的内存
        free(A);
        free(B);
        free(C);
    }

    return 0;
}
