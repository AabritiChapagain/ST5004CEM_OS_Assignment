#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

#define NUM_THREADS 5
#define ITERATIONS 200000

long shared_counter = 0;
pthread_mutex_t counter_lock;
typedef struct
{
    int pid;
    int burst_time;
    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
} Process;

void run_round_robin()
{
    printf("\n========== Round Robin Scheduling ==========\n");

    const int quantum = 4;
    const int num_processes = 5;

    Process processes[5] =
    {
        {1, 10, 10, 0, 0, 0},
        {2, 5, 5, 0, 0, 0},
        {3, 8, 8, 0, 0, 0},
        {4, 3, 3, 0, 0, 0},
        {5, 6, 6, 0, 0, 0}
    };

    int completed = 0;
    int current_time = 0;

    printf("Time Quantum = %d ms\n\n", quantum);

    while (completed < num_processes)
    {
        for (int i = 0; i < num_processes; i++)
        {
            if (processes[i].remaining_time > 0)
            {
                int run_time;

                if (processes[i].remaining_time > quantum)
                    run_time = quantum;
                else
                    run_time = processes[i].remaining_time;

                printf("Time %2d -> %2d : Process P%d executes for %d ms\n",
                       current_time,
                       current_time + run_time,
                       processes[i].pid,
                       run_time);

                current_time += run_time;
                processes[i].remaining_time -= run_time;

                if (processes[i].remaining_time == 0)
                {
                    completed++;

                    processes[i].completion_time = current_time;
                    processes[i].turnaround_time = current_time;
                    processes[i].waiting_time =
                        processes[i].turnaround_time -
                        processes[i].burst_time;

                    printf("Process P%d completed.\n",
                           processes[i].pid);
                }
            }
        }
    }

    double total_wait = 0;
    double total_turnaround = 0;

    printf("\n-------------------------------------------------------------\n");
    printf("PID\tBurst\tCompletion\tTurnaround\tWaiting\n");
    printf("-------------------------------------------------------------\n");

    for (int i = 0; i < num_processes; i++)
    {
        printf("%d\t%d\t%d\t\t%d\t\t%d\n",
               processes[i].pid,
               processes[i].burst_time,
               processes[i].completion_time,
               processes[i].turnaround_time,
               processes[i].waiting_time);

        total_wait += processes[i].waiting_time;
        total_turnaround += processes[i].turnaround_time;
    }

    printf("\nAverage Waiting Time    : %.2f ms\n",
           total_wait / num_processes);

    printf("Average Turnaround Time : %.2f ms\n",
           total_turnaround / num_processes);
}

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

    run_round_robin();

    return 0;
}
