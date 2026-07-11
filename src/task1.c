#include <stdio.h>
#include <pthread.h>

void* task(void* arg)
{
    int id = *(int*)arg;
    printf("Thread %d is running\n", id);
    return NULL;
}

int main()
{
    pthread_t threads[5];
    int ids[5] = {1, 2, 3, 4, 5};

    // Create 5 threads
    for (int i = 0; i < 5; i++)
    {
        pthread_create(&threads[i], NULL, task, &ids[i]);
    }

    // Wait for all 5 threads to finish
    for (int i = 0; i < 5; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("All threads finished.\n");

    return 0;
}
