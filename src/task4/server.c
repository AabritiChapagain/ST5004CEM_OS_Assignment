#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5050
#define BUFFER_SIZE 512

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    char buffer[BUFFER_SIZE];

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

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 5) < 0)
    {
        perror("Listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);

    if (client_fd < 0)
    {
        perror("Accept");
        exit(EXIT_FAILURE);
    }

    printf("Client connected.\n");

    recv(client_fd, buffer, sizeof(buffer), 0);

    printf("Received: %s\n", buffer);

    send(client_fd, "Connection successful.", 22, 0);

    close(client_fd);
    close(server_fd);

    return 0;
}
