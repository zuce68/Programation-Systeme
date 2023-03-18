/* Server code */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

int main(int argc, char const *argv[]) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
       
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
       
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
       
    // Binding the socket to the address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
       
    // Listening to incoming connections
    if (listen(server_fd, 1) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
       
    // Accepting a new connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
       
    // Sending a welcome message to the client
    send(new_socket, hello, strlen(hello), 0);
    printf("Hello message sent\n");
       
    // Chatting with the client
    while (1) {
        // Reading the message from the client
        memset(buffer, 0, sizeof(buffer));
        if (read(new_socket, buffer, sizeof(buffer)) < 0) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        printf("Client: %s\n", buffer);

        // Sending a response to the client
        memset(buffer, 0, sizeof(buffer));
        printf("Server: ");
        scanf("%s", buffer);
        if (send(new_socket, buffer, strlen(buffer), 0) < 0) {
            perror("send");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}