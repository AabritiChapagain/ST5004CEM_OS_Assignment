#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include<time.h>
#define PORT 5050
#define BUFFER_SIZE 512


int validate_input(const char *input)
{
    if (input == NULL)
        return 0;

    if (strlen(input) == 0 || strlen(input) >= BUFFER_SIZE)
        return 0;

    return 1;
}

void *handle_client(void *arg)
{
    int client_fd = *(int *)arg;
    free(arg);

    char buffer[BUFFER_SIZE];

int bytes = recv(client_fd, buffer, sizeof(buffer), 0);

if (bytes <= 0)
{
    close(client_fd);
    return NULL;
}

buffer[bytes] = '\0';

    printf("Received: %s\n", buffer);

if (!validate_input(buffer))
{
    strcpy(buffer, "ERR:invalid_input");
    send(client_fd, buffer, strlen(buffer) + 1, 0);
    close(client_fd);
    return NULL;
}
    if (strncmp(buffer, "AUTH ", 5) == 0)
    {
        if (strcmp(buffer + 5, "cw-secret-token") == 0)
        {
            strcpy(buffer, "OK:authenticated");
        }
        else
        {
            strcpy(buffer, "ERR:bad_token");
        }
    }
    else if (strncmp(buffer, "ECHO ", 5) == 0)
    {
        send(client_fd, buffer + 5, strlen(buffer + 5) + 1, 0);
    }
    else if (strcmp(buffer, "TIME") == 0)
    {
        time_t now = time(NULL);
        char *t = ctime(&now);
        send(client_fd, t, strlen(t) + 1, 0);
    }
    else
    {
strcpy(buffer, "ERR:unknown_command");
send(client_fd, buffer, strlen(buffer) + 1, 0);    }

    close(client_fd);
    return NULL;
}
int main()
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    char buffer[BUFFER_SIZE];
    int authenticated = 0;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0)
    {
        perror("Socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

     bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
     listen(server_fd, 5);

    printf("Server listening on port %d...\n", PORT);
while (1)
{
    client_fd = accept(server_fd,
                       (struct sockaddr *)&client_addr,
                       &client_len);

    int *new_sock = malloc(sizeof(int));
    *new_sock = client_fd;

    pthread_t tid;

    pthread_create(&tid, NULL, handle_client, new_sock);
    pthread_detach(tid);
}
   close(server_fd);

    return 0;
}
