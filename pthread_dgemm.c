#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 1024              // 定义矩阵大小
#define BLOCK_SIZE 16         // 定义分块大小
#define NUM_THREADS 10        // 设置线程数量为10

double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE]; // 矩阵声明

// 线程数据结构，包含线程ID和总线程数
typedef struct {
    int thread_id;
    int num_threads;
} ThreadData;

// 矩阵乘法的线程函数
void* dgemm_block(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int tid = data->thread_id; // 获取当前线程ID
    int n = SIZE;

    // 根据线程ID划分行范围
    for (int i = tid * (n / NUM_THREADS); i < (tid + 1) * (n / NUM_THREADS); i += BLOCK_SIZE) {
        for (int j = 0; j < n; j += BLOCK_SIZE) {
            for (int k = 0; k < n; k += BLOCK_SIZE) {
                // 对每个块进行计算
                for (int ii = i; ii < i + BLOCK_SIZE && ii < n; ii++) {
                    for (int jj = j; jj < j + BLOCK_SIZE && jj < n; jj++) {
                        double sum = 0;
                        for (int kk = k; kk < k + BLOCK_SIZE && kk < n; kk++) {
                            sum += A[ii][kk] * B[kk][jj]; 
                        }
                        C[ii][jj] += sum; 
                    }
                }
            }
        }
    }
    return NULL; 
}

int main() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            A[i][j] = rand() % 10; 
            B[i][j] = rand() % 10; 
            C[i][j] = 0;           
        }
    }

    pthread_t threads[NUM_THREADS]; 
    ThreadData threadData[NUM_THREADS]; 

    for (int i = 0; i < NUM_THREADS; i++) {
        threadData[i].thread_id = i; 
        threadData[i].num_threads = NUM_THREADS; 
        pthread_create(&threads[i], NULL, dgemm_block, &threadData[i]); 
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL); 
    }

    return 0;
}
