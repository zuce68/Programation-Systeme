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
        int server_read_fd_pid = open("/tmp/misiuk", O_RDONLY);
        read(server_read_fd_pid, &client_pid, sizeof(int));
        close(server_read_fd_pid);

        printf("Received connection from client %d\n", client_pid);


        char server_fifo_read[50];
        char server_fifo_write[50];
        int server_read_fd;
        int server_write_fd;
        
        sprintf(server_fifo_read, "/tmp/fifo%d_2", client_pid);
        sprintf(server_fifo_write, "/tmp/fifo%d_1", client_pid);

        printf("%s\n%s\n", server_fifo_write, server_fifo_read);

        while((server_write_fd = open(server_fifo_write, O_WRONLY))==-1){
            fprintf(stderr, "trying to connect write\n");
            sleep(1);
        }   

        while((server_read_fd = open(server_fifo_read, O_RDONLY))==-1){
            fprintf(stderr, "trying to connect read\n");
            sleep(1);
        }   
   

        

        write(server_write_fd, "You are connected, send a message\n", 256);

        char buffer_w[50];
        char buffer_r[50];

        while (1) {
            read(server_read_fd, &buffer_r,256);
            printf("Client %d: %s\n", client_pid, buffer_r);

            if (strcmp(buffer_w,"quit")==0){
                break;
            }

            printf("Server: ");
            fgets(buffer_w, 50, stdin);
            printf("\n");
            write(server_write_fd, &buffer_w, 256);
        }

        close(server_read_fd);
        close(server_write_fd);
        unlink(server_fifo_read);
        unlink(server_fifo_write);
        printf("Connection with client %d closed.\n", client_pid);
    }

    return 0;
}
