#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define BUFFER_SIZE 5
#define NUM_PRODUCERS 3
#define NUM_CONSUMERS 3

int buffer[BUFFER_SIZE];
int count = 0; // Number of items in the buffer

sem_t mutex;
sem_t empty;
sem_t full;

// Function to print the buffer
void printBuffer() {
    printf("[");
    for (int i = 0; i < count; i++) {
        printf("%d", buffer[i]);
        if (i < count - 1) printf(", ");
    }
    printf("]\n");
}

void* producer(void* arg) {

  

    int id = (int)arg;
    while (1) {
        int item = rand() % 100; // Produce an item: a random integer between 0 and 99

        sem_wait(&empty);
        sem_wait(&mutex);

        // Add item to buffer
        buffer[count++] = item;
        printf("\nProducer %d produced: %d\n", id, item);
        printBuffer();

        sem_post(&mutex);
        sem_post(&full);

        // Sleep for a random time up to 3 seconds
        sleep(rand() % 4);

        if(count == BUFFER_SIZE){
            printf("BUFFER IS FULL...\n\n");
        }

    }
    return NULL;
}

void* consumer(void* arg) {

    int id = (int)arg;
    while (1) {
        int item;

        sem_wait(&full);
        sem_wait(&mutex);

        // Remove the last item from the buffer
        item = buffer[--count];
        printf("\nConsumer %d consumed: %d\n", id, item);
        printBuffer();

        sem_post(&mutex);
        sem_post(&empty);

        // Sleep for a random time up to 3 seconds
        sleep(rand() % 4);

        if(count == 0){
        printf("BUFFER IS EMPTY...\n\n");
    }
    }
    return NULL;
}

int main() {
    pthread_t producers[NUM_PRODUCERS], consumers[NUM_CONSUMERS];
    int ids[NUM_PRODUCERS > NUM_CONSUMERS ? NUM_PRODUCERS : NUM_CONSUMERS];

    // Initialize the random number generator
    srand(time(NULL));

    // Initialize semaphores
    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    // Create producer and consumer threads
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        ids[i] = i + 1;
        pthread_create(&producers[i], NULL, producer, (void*)&ids[i]);
    }
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        ids[i] = i + 1; // Reusing the same ids array for simplicity
        pthread_create(&consumers[i], NULL, consumer, (void*)&ids[i]);
    }

    // Wait for the producer and consumer threads to finish
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(producers[i], NULL);
    }
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(consumers[i], NULL);
    }

    // Cleanup semaphores
    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}