#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

int main(int argc, char const *argv[]) {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    char *hello = "Hello from client";

    // Creating socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
       
    // Converting IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Connecting to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // Sending a welcome message to the server
    send(sock, hello, strlen(hello), 0);
    printf("Hello message sent\n");

    // Chatting with the server
    while (1) {
        // Reading the message from the server
        memset(buffer, 0, sizeof(buffer));
        if (read(sock, buffer, sizeof(buffer)) < 0) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        printf("Server: %s\n", buffer);

        // Sending a message to the server
        memset(buffer, 0, sizeof(buffer));
        printf("Client: ");
        scanf("%s", buffer);
        if (send(sock, buffer, strlen(buffer), 0) < 0) {
            perror("send");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
