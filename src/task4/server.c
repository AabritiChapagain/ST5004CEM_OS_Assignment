#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include<time.h>
#define PORT 5050
#define BUFFER_SIZE 512

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

    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
recv(client_fd, buffer, sizeof(buffer), 0);
  printf("Received: %s\n", buffer);
if (strncmp(buffer, "AUTH ", 5) == 0)
{
    if (strcmp(buffer + 5, "cw-secret-token") == 0)
    {
        authenticated = 1;
        strcpy(buffer, "OK:authenticated");
    }
    else
    {
        strcpy(buffer, "ERR:bad_token");
    }
}
else if (!authenticated)
{
    strcpy(buffer, "ERR:not_authenticated");
}
else if (strncmp(buffer, "ECHO ", 5) == 0)
{
    memmove(buffer, buffer + 5, strlen(buffer + 5) + 1);
}
else if (strcmp(buffer, "TIME") == 0)
{
    time_t now = time(NULL);
    strcpy(buffer, ctime(&now));
}
else
{
    strcpy(buffer, "Unknown Command");
}

send(client_fd, buffer, strlen(buffer) + 1, 0);
    close(client_fd);
    close(server_fd);

    return 0;
}
