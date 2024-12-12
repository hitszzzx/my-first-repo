#define NUM_REPEATS 10 // 重复执行的次数

int main() {
    int sizes[] = {10, 40, 320, 640, 1000, 4000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int s = 0; s < num_sizes; s++) {
        int m = sizes[s];
        int n = sizes[s];
        int k = sizes[s];

        // 动态分配矩阵
        double (*A)[k] = malloc(m * sizeof(*A));
        double (*B)[n] = malloc(k * sizeof(*B));
        double (*C)[n] = malloc(m * sizeof(*C));

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

        // 重复执行多次，并累积时间
        double total_time = 0.0;
        for (int repeat = 0; repeat < NUM_REPEATS; repeat++) {
            // 初始化C为0
            for (int i = 0; i < m; i++) {
                for (int j = 0; j < n; j++) {
                    C[i][j] = 0.0;
                }
            }

            // 记录开始时间
            clock_t start = clock();
            // 执行矩阵乘法
            naive_dgemm(m, n, k, A, B, C);
            // 记录结束时间
            clock_t end = clock();

            // 累积执行时间
            total_time += (double)(end - start) / CLOCKS_PER_SEC;
        }

        // 计算平均执行时间
        double average_time = total_time / NUM_REPEATS;
        // 计算GFLOPS
        double gflops = calculate_gflops(m, n, k, average_time);

        // 打印结果
        printf("Matrix size: %d x %d\n", m, n);
        printf("Average time taken (over %d repeats): %f seconds\n", NUM_REPEATS, average_time);
        printf("GFLOPS: %f\n\n", gflops);

        // 释放动态分配的内存
        free(A);
        free(B);
        free(C);
    }

    return 0;
}
