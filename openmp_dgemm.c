#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

// 计算GFLOPS的函数
double calculate_gflops(int m, int n, int k, double time) {
    return (2.0 * m * n * k) / (time * 1e9); // 计算GFLOPS
}

int main() {
    int sizes[] = {10, 40, 320, 640, 1000, 4000, 8000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    int num_threads = 10; // 设置线程数为10

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
        double start_time = omp_get_wtime();

        // 使用OpenMP进行矩阵乘法
        #pragma omp parallel for num_threads(num_threads) collapse(2)
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                for (int p = 0; p < k; p++) {
                    C[i * n + j] += A[i * k + p] * B[p * n + j];
                }
            }
        }

        // 记录结束时间
        double end_time = omp_get_wtime();

        // 计算执行时间
        double time_taken = end_time - start_time;
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
