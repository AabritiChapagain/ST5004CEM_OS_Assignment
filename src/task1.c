#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#define NUM_THREADS 5
void run_process_creation()
{
    pid_t pid = fork();

    if(pid == 0)
    {
        printf("\nChild Process\n");
        printf("Child PID: %d\n", getpid());
        printf("Parent PID: %d\n", getppid());
        exit(0);
    }
    else
    {
        wait(NULL);

        printf("\nParent Process\n");
        printf("Parent PID: %d\n", getpid());
        printf("Child PID: %d\n", pid);
    }
}

void *task(void *arg)
{
    int id = *(int *)arg;

    printf("Thread %d started (Thread ID: %lu)\n",
           id,
           (unsigned long)pthread_self());

    usleep(100000 * id);   // Simulate work

    printf("Thread %d finished\n", id);

    return NULL;
}
void run_basic_threads()
{
    printf("\n========== Basic Multi-threading ==========\n");

    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS] = {1, 2, 3, 4, 5};

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, task, &ids[i]);
    }

    // Wait for all threads
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("All threads have completed.\n");
}
int main()
{
    printf("ST5004CEM Task 1\n");

    run_process_creation();

    run_basic_threads();   

    return 0;
}

