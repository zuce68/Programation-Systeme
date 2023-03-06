#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

void shutdown_client(int signum) {
    printf("Client shutting down...\n");
    exit(0);
}

int main()
{
    signal(SIGINT, shutdown_client);

    int pid = getpid();

    printf("Client %d is running...\n", pid);

    int server_write_fd = open("/tmp/misiuk", O_WRONLY);

    if (server_write_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    write(server_write_fd, &pid, sizeof(pid));
    close(server_write_fd);

    char client_fifo_read[50];
    char client_fifo_write[50];

    sprintf(client_fifo_read, "/tmp/fifo%d_1", pid);
    sprintf(client_fifo_write, "/tmp/fifo%d_2", pid);

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

    char buffer;
   
    while(1){
        printf("Client: ");
        buffer = getchar();
        write(client_write, buffer, strlen(buffer) + 1);

        if (strcmp(buffer, "quit\n") == 0) {
            break;
        }

        read(client_read, buffer, strlen(buffer) + 1);
        printf("Server: %s", buffer);
    }

    close(client_read);
    close(client_write);
    unlink(client_fifo_read);
    unlink(client_fifo_write);

    return 0;
}
