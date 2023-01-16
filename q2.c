#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
    int file_desc, status;
    pid_t child;

    file_desc = open("file.txt", O_RDONLY);
    if (file_desc < 0) {
        printf("Error opening file\n");
        exit(1);
    }

    child = fork();
    if (child == 0) {
        char buffer[100];
        int read_bytes;
        while ((read_bytes = read(file_desc, buffer, 100)) > 0) {
            write(1, buffer, read_bytes);
        }
        sleep(5);
        printf("Child terminating\n");
        sleep(5);
        exit(20);
    } else {
        wait(&status);
        printf("My child has terminated\n");
        printf("Child status value: %d\n", WEXITSTATUS(status));
    }

    close(file_desc);
    return 0;
}