#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Array to store Fibonacci numbers
long long *fibonacci;

// Function prototype for the thread
void *calculate_fibonacci(void *arg);

int main() {
    int n, i;

    printf("Enter the number of Fibonacci terms: ");
    scanf("%d", &n);

    // Allocate memory to store Fibonacci numbers
    fibonacci = (long long *)malloc(n * sizeof(long long));
    if (fibonacci == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    // Initialize the first two Fibonacci numbers
    fibonacci[0] = 0;
    if (n > 1) {
        fibonacci[1] = 1;
    }

    // Create thread identifiers
    pthread_t *threads = (pthread_t *)malloc((n - 2) * sizeof(pthread_t));
    if (threads == NULL) {
        perror("Failed to allocate memory for threads");
        free(fibonacci);
        exit(EXIT_FAILURE);
    }

    // Create threads to calculate Fibonacci numbers from the 3rd term
    for (i = 0; i < n - 2; i++) {
        int *arg = malloc(sizeof(*arg));
        if (arg == NULL) {
            perror("Failed to allocate memory for thread argument");
            free(threads);
            free(fibonacci);
            exit(EXIT_FAILURE);
        }
        *arg = i + 2;

        if (pthread_create(&threads[i], NULL, calculate_fibonacci, arg) != 0) {
            perror("Failed to create thread");
            free(arg);
            continue;
        }
    }

    // Wait for threads to complete
    for (i = 0; i < n - 2; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print the Fibonacci series
    printf("Fibonacci Series:\n");
    for (i = 0; i < n; i++) {
        printf("%lld ", fibonacci[i]);
    }
    printf("\n");

    // Clean up
    free(threads);
    free(fibonacci);

    return 0;
}

void *calculate_fibonacci(void *arg) {
    int n = *((int *)arg);
    free(arg);

    fibonacci[n] = fibonacci[n - 1] + fibonacci[n - 2];

    pthread_exit(NULL);
}