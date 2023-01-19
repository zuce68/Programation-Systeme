#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
    int file, status;
    pid_t child;

    file = open("file.txt", O_RDONLY);
    if (file < 0) {
        printf("Error opening file\n");
        exit(1);
    }

    child = fork();
    if (child == 0) {
        char buffer[10000];
        int rd_bytes;
        while ((rd_bytes = read(file, buffer, 10000)) > 0) {
            write(1, buffer, rd_bytes);
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

    close(file);
    return 0;
}