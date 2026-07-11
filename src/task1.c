#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

#define NUM_THREADS 5
#define ITERATIONS 200000

long shared_counter = 0;

pthread_mutex_t counter_lock;
void *safe_increment(void *arg)
{
    (void)arg;

    for (int i = 0; i < ITERATIONS; i++)
    {
        pthread_mutex_lock(&counter_lock);

        shared_counter++;

        pthread_mutex_unlock(&counter_lock);
    }

    return NULL;
}
void run_mutex_demo()
{
    printf("\n========== Mutex Synchronization ==========\n");

    shared_counter = 0;

    pthread_mutex_init(&counter_lock, NULL);

    pthread_t threads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, safe_increment, NULL);
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    long expected = NUM_THREADS * ITERATIONS;

    printf("Expected Counter Value : %ld\n", expected);
    printf("Actual Counter Value   : %ld\n", shared_counter);

    if (shared_counter == expected)
    {
        printf("Mutex successfully prevented the race condition.\n");
    }
    else
    {
        printf("Unexpected error occurred.\n");
    }

    pthread_mutex_destroy(&counter_lock);
}

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
void *unsafe_increment(void *arg)
{
    (void)arg;

    for (int i = 0; i < ITERATIONS; i++)
    {
        shared_counter++;
    }

    return NULL;
}
void run_race_condition()
{
    printf("\n========== Race Condition Demonstration ==========\n");

    shared_counter = 0;

    pthread_t threads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, unsafe_increment, NULL);
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    long expected = NUM_THREADS * ITERATIONS;

    printf("Expected Counter Value : %ld\n", expected);
    printf("Actual Counter Value   : %ld\n", shared_counter);

    if (shared_counter != expected)
    {
        printf("Race condition detected!\n");
        printf("%ld updates were lost.\n", expected - shared_counter);
    }
    else
    {
        printf("No race condition observed this run.\n");
        printf("Try running the program again.\n");
    }
}

int main()
{
    printf("ST5004CEM Task 1\n");

    run_process_creation();
 
    run_basic_threads();   
  
    run_race_condition();

   run_mutex_demo();
   
    return 0;
}

