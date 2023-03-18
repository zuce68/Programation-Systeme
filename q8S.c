#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
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

        char buffer_w[256];
        char buffer_r[256];

        while (1) {
                // Réception du message
            read(server_read_fd, buffer_r, 100);
            printf("Client : %s", buffer_r);

            // Envoi de la réponse
            printf("Entrez une réponse : ");
            fgets(buffer_w, 100, stdin);
            write(server_write_fd, buffer_w, strlen(buffer_w) + 1);

            // // Affichage du message
            // printf("Message reçu : %s\n", buf);
            // printf("%d\n",server_read_fd);
            // int n = read(server_read_fd, buffer_r, 256);

            // printf("nnnn %d\n",n);
            // if ( n!= 0) {
            //     buffer_r[n]='\0';
            //     printf("Client: %s\n", buffer_r);
            // }

           /* printf("Server: \n");
            fgets(buffer_w, 256, stdin);
            printf("\n");
            if (strcmp(buffer_w, "") != 0) {
                write(server_write_fd, buffer_w, strlen(buffer_w));
            }*/
        }


        close(server_read_fd);
        close(server_write_fd);
        unlink(server_fifo_read);
        unlink(server_fifo_write);
        printf("Connection with client %d closed.\n", client_pid);
    }

    return 0;
}
