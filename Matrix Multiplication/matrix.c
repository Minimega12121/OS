#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h> // Include time.h for clock()

#define SIZE 2500 // Size of the matrices

int A[SIZE][SIZE];
int B[SIZE][SIZE];
int C[SIZE][SIZE];

typedef struct {
    int startRow;
    int endRow;
} ThreadData;

void fillMatrices() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            A[i][j] = rand() % 10; // Fill with random numbers for demonstration
            B[i][j] = rand() % 10;
        }
    }
}

void* multiply(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    for (int i = data->startRow; i < data->endRow; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < SIZE; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    pthread_exit(0);
}

void printMatrixPortion(int C[SIZE][SIZE], int startRow, int endRow, int startCol, int endCol) {
    for (int i = startRow; i < endRow; ++i) {
        for (int j = startCol; j < endCol; ++j) {
            printf("%5d ", C[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int numThreads;
    printf("Enter the number of threads: ");
    scanf("%d", &numThreads);

    fillMatrices(); // Fill A and B with random values

    pthread_t threads[numThreads];
    ThreadData data[numThreads];
    int rowsPerThread = SIZE / numThreads;
    int extraRows = SIZE % numThreads;

    struct timeval start, end;
    double time_taken;

    gettimeofday(&start, NULL);

    for (int i = 0; i < numThreads; ++i) {
        data[i].startRow = i * rowsPerThread;
        data[i].endRow = (i + 1) * rowsPerThread + (i == numThreads - 1 ? extraRows : 0);
        pthread_create(&threads[i], NULL, multiply, (void*)&data[i]);
    }

    for (int i = 0; i < numThreads; ++i) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end, NULL);

    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6;


    printf("Matrix multiplication took %f seconds to execute.\n", time_taken);

    // Optionally print a portion of the result matrix
    // printf("Portion of Matrix C:\n");
    // printMatrixPortion(C, 0, SIZE, 0, SIZE); 

    return 0;
}