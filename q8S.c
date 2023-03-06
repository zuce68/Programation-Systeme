#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

int server_pid;

void shutdown_server(int signum) {
    printf("Server shutting down...\n");
    unlink("/tmp/misiuk");
    exit(0);
}

int main()
{
    signal(SIGINT, shutdown_server);

    int server_fd = mkfifo("/tmp/misiuk", 0777);
    if (server_fd != 0 && errno != EEXIST) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    printf("Server is running...\n");

    while (1) {
        int client_pid;
        int server_read_fd = open("/tmp/misiuk", O_RDONLY);
        read(server_read_fd, &client_pid, sizeof(int));
        close(server_read_fd);

        printf("Received connection from client %d\n", client_pid);


        char client_fifo_read[50];
        char client_fifo_write[50];

        
        sprintf(client_fifo_read, "/tmp/fifo%d_2", client_pid);
        sprintf(client_fifo_write, "/tmp/fifo%d_1", client_pid);

        int client_read_fd = mkfifo(client_fifo_read, 0777);
        if (client_read_fd != 0 && errno != EEXIST) {
            perror("mkfifo");
            exit(EXIT_FAILURE);
        }
        int client_write_fd = mkfifo(client_fifo_write, 0777);
        if (client_write_fd != 0 && errno != EEXIST) {
            perror("mkfifo");
            exit(EXIT_FAILURE);
        }

        int client_read = open(client_fifo_read, O_RDONLY);
        int client_write = open(client_fifo_write, O_WRONLY);

       write(client_fifo_write, "You are connected, send a message\n", strlen("You are connected, send a message\n") + 1);

        char buffer;
        while (1) {
            read(client_read, buffer, 1);
            printf("Client %d: %s", client_pid, buffer);

            if (strcmp(buffer, "quit\n") == 0) {
                break;
            }

            printf("Server: ");
            buffer = getchar();
            write(client_write, buffer, strlen(buffer) + 1);
        }

        close(client_read);
        close(client_write);
        unlink(client_fifo_read);
        unlink(client_fifo_write);
        printf("Connection with client %d closed.\n", client_pid);
    }

    return 0;
}
