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

int roll_dice() {
    return (rand() % 6 + 1) + (rand() % 6 + 1);
}

void child(int server_read_fd_pid, int player, char player_fifo[MAX_PLAYERS][MAX_NAME_LEN+5], int num_players)
{
    char player_name[50];
    int score = 0;
    int roll;
    while (score < 100) {
        // Wait for server to signal it's our turn to play
        char player_turn;
        write(player, "1", strlen("1")+1);
        read(server_read_fd_pid, &player_turn, sizeof(char));
        if (strcmp(&player_turn,"1")==0) {
            roll = roll_dice();
            score += roll;
            // Display roll and score
            printf("Mr Dice rolled %d (total score: %d)\n", roll, score);
            // Send player name to server
            write(server_read_fd_pid, "1", strlen("1")+1);
        }
        else if (strcmp(&player_turn,"0")==0)
        {
            printf("Mr.Dice : This is the end of the game.");
           // break;
        }
        // Roll dice and update score
        
    }
    printf("Mr Dice : This is the end of the game.");
    write(server_read_fd_pid, "0", strlen("0")+1);
    exit(20);
}

int main() {
    char player_name[50];
    int num_players = 0;
    pid_t pid;
    int score;
    int player;
    int roll;
    char player_fifo[MAX_PLAYERS][MAX_NAME_LEN+5]; // room for "/tmp/" prefix and null terminator

    unlink("/tmp/server");

    int server_fd = mkfifo("/tmp/server", 0777);
    if (server_fd != 0 && errno != EEXIST) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }


    printf("Server is waiting for players...\n");
    
    int server_read_fd_pid = open("/tmp/server", O_RDONLY);
    if (server_read_fd_pid == -1) {
        printf("Error opening server FIFO\n");
        exit(EXIT_FAILURE);
    }


    // Wait for players to connect and spawn a child process for each one
   //while (num_players < MAX_PLAYERS) {
        read(server_read_fd_pid, &player_name, sizeof(char[50]));
        sprintf(player_fifo[num_players], "/tmp/%s", player_name);
        player = open(player_fifo[num_players], O_WRONLY);
        if (player == -1) {
            printf("Error opening FIFO %s\n", player_fifo[num_players]);
            exit(EXIT_FAILURE);
        }
        printf("Mr Dice : Player %d connected!\n", num_players+1);
        num_players++;
        pid = fork();
        if (pid == -1) {
            printf("Error forking child process\n");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { // child process
            child(server_read_fd_pid, player, player_fifo, num_players);
        } else { // parent process
            close(player);
        }
     //}

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

    close(server_read_fd_pid);
    printf("Server exiting...\n");

    return 0;
}


