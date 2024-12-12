#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 10  // 线程数
#define BLOCK_SIZE 16   // 块大小

typedef struct {
    int thread_id;
    int m;
    int n;
    int k;
    double *A;
    double *B;
    double *C;
} ThreadData;

// 矩阵乘法计算函数
void *pthreads_dgemm(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int m = data->m;
    int n = data->n;
    int k = data->k;
    int thread_id = data->thread_id;

    for (int i = thread_id * (m / NUM_THREADS); i < (thread_id + 1) * (m / NUM_THREADS); i += BLOCK_SIZE) {
        for (int j = 0; j < n; j += BLOCK_SIZE) {
            for (int p = 0; p < k; p += BLOCK_SIZE) {
                for (int ii = i; ii < i + BLOCK_SIZE && ii < m; ii++) {
                    for (int jj = j; jj < j + BLOCK_SIZE && jj < n; jj++) {
                        double sum = 0;
                        for (int pp = p; pp < p + BLOCK_SIZE && pp < k; pp++) {
                            sum += data->A[ii * k + pp] * data->B[pp * n + jj];
                        }
                        data->C[ii * n + jj] += sum;
                    }
                }
            }
        }
    }
    return NULL;
}

// 计算GFLOPS的函数
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

        // 创建线程
        pthread_t threads[NUM_THREADS];
        ThreadData thread_data[NUM_THREADS];

        // 记录开始时间
        clock_t start = clock();

        // 创建线程
        for (int i = 0; i < NUM_THREADS; i++) {
            thread_data[i].thread_id = i;
            thread_data[i].m = m;
            thread_data[i].n = n;
            thread_data[i].k = k;
            thread_data[i].A = A;
            thread_data[i].B = B;
            thread_data[i].C = C;

            pthread_create(&threads[i], NULL, pthreads_dgemm, (void *)&thread_data[i]);
        }

        // 等待所有线程完成
        for (int i = 0; i < NUM_THREADS; i++) {
            pthread_join(threads[i], NULL);
        }

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
