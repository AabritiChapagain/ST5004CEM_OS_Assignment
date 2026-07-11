#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

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

int main()
{
    printf("ST5004CEM Task 1\n");

    run_process_creation();

    return 0;
}
