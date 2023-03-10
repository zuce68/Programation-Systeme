#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

int main()
{
    int server_write_fd;
    int pid = getpid();
    char client_fifo_read[50];
    char client_fifo_write[50];

    printf("Client %d is running...\n", pid);
    
    while((server_write_fd = open("/tmp/misiuk", O_WRONLY))==-1){
        fprintf(stderr, "trying to connect\n");
        sleep(1);
    }
    printf("Connected to server\n");
    write(server_write_fd, &pid, sizeof(int));
    close(server_write_fd);

    sprintf(client_fifo_read, "/tmp/fifo%d_1", pid);
    sprintf(client_fifo_write, "/tmp/fifo%d_2", pid);

    //unlink(client_fifo_read);
    //unlink(client_fifo_write);
   

    printf("%s\n%s\n", client_fifo_read, client_fifo_write);


    if (mkfifo(client_fifo_read, 0777)!=0)
    exit(1);
    if (mkfifo(client_fifo_write, 0777)!=0)
    exit(1);

    printf("FIFOs created\n");
    
    int client_read = open(client_fifo_read, O_RDONLY);
    int client_write = open(client_fifo_write, O_WRONLY);

    

    char buffer_w[50];
    char buffer_r[50];
   
    read(client_read, &buffer_r, 256);
    printf("Server: %s\n", buffer_r);

    while(1){

        read(client_read, &buffer_r, 256);
        printf("Server: %s\n", buffer_r);


        printf("Client: ");
        fgets(buffer_w, 50, stdin);
        printf("\n");
        write(client_write, &buffer_w, 256);    

     
        if (strcmp(buffer_w,"quit")==0) {
            break;
        }
    }

    close(client_read);
    close(client_write);
 
    printf("Connection with server closed.\n");

    return 0;
}
