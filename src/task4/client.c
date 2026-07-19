#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5050
#define BUFFER_SIZE 512

int main()
{
    int sockfd;

    struct sockaddr_in server_addr;

    char buffer[BUFFER_SIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        perror("Socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    inet_pton(AF_INET,
              "127.0.0.1",
              &server_addr.sin_addr);

    if (connect(sockfd,
                (struct sockaddr *)&server_addr,
                sizeof(server_addr)) < 0)
    {
        perror("Connect");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server.\n");

    printf("Enter command:\n");
    printf("1. ECHO Hello\n");
    printf("2. TIME\n\n");

    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    send(sockfd, buffer, strlen(buffer) + 1, 0);

    recv(sockfd, buffer, sizeof(buffer), 0);

    printf("Server Response: %s\n", buffer);

    close(sockfd);

    return 0;
}
