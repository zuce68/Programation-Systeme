#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#define MAX_PLAYERS 10
#define MAX_NAME_LEN 50
#define FIFO_MODE 0666

int main() {
    int num_players = 0;
    pid_t pid;
    char player_fifo[MAX_PLAYERS][MAX_NAME_LEN+5]; // room for "/tmp/" prefix and null terminator

    // Create FIFOs for each player
    for (int i = 0; i < MAX_PLAYERS; i++) {
        sprintf(player_fifo[i], "/tmp/player%d", i+1);
        mkfifo(player_fifo[i], FIFO_MODE);
    }

    printf("Server is waiting for players...\n");

    // Wait for players to connect and spawn a child process for each one
    while (num_players < MAX_PLAYERS) {
        int fd = open(player_fifo[num_players], O_RDONLY);
        if (fd == -1) {
            printf("Error opening FIFO %s\n", player_fifo[num_players]);
            exit(EXIT_FAILURE);
        }
        printf("Player %d connected!\n", num_players+1);

        pid = fork();
        if (pid == -1) {
            printf("Error forking child process\n");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { // child process
            char name[MAX_NAME_LEN];
            read(fd, name, MAX_NAME_LEN);
            close(fd);
            char *args[] = {"./client", name, NULL};
            execv(args[0], args);
            exit(EXIT_SUCCESS);
        } else { // parent process
            close(fd);
            num_players++;
        }
    }

    printf("All players connected!\n");

    // Wait for all child processes to terminate
    for (int i = 0; i < MAX_PLAYERS; i++) {
        int status;
        wait(&status);
    }

    // Remove FIFOs
    for (int i = 0; i < MAX_PLAYERS; i++) {
        unlink(player_fifo[i]);
    }

    printf("Server exiting...\n");

    return 0;
}
