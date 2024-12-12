#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cblas.h>  // 包含 OpenBLAS 的头文件

// 计算 GFLOPS 的函数
double calculate_gflops(int m, int n, int k, double time) {
    return (2.0 * m * n * k) / (time * 1e9); // 计算GFLOPS
}

int main() {
    int sizes[] = {10, 40, 320, 640, 1000, 4000, 8000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int s = 0; s < num_sizes; s++) {
        int m = sizes[s];
        int n = sizes[s];
        int k = sizes[s];

        // 动态分配矩阵
        double *A = (double *)malloc(m * k * sizeof(double));
        double *B = (double *)malloc(k * n * sizeof(double));
        double *C = (double *)malloc(m * n * sizeof(double));

        // 随机初始化矩阵A和B
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < k; j++) {
                A[i * k + j] = (double)(rand() % 10);
            }
        }
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < n; j++) {
                B[i * n + j] = (double)(rand() % 10);
            }
        }
        // 初始化C矩阵
        for (int i = 0; i < m * n; i++) {
            C[i] = 0.0;
        }

        // 记录开始时间
        clock_t start = clock();
        // 执行 OpenBLAS 的 dgemm
        cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, n, k, 1.0, A, k, B, n, 0.0, C, n);
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
