#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <time.h>

#define MAX_NAME_LEN 50
#define FIFO_MODE 0666
#define TARGET_SCORE 100

int roll_dice() {
    return (rand() % 6 + 1) + (rand() % 6 + 1);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s player_name\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    char *player_name = argv[1];
    srand(time(NULL) ^ getpid()); // use process ID as seed for random number generator
    int score = 0;

    // Open FIFO for communication with server
    int server_fd = open("/tmp/server", O_WRONLY);
    if (server_fd == -1) {
        printf("Error opening server FIFO\n");
        exit(EXIT_FAILURE);
    }

    // Create FIFO for communication with server
    char player_fifo[MAX_NAME_LEN+5]; // room for "/tmp/" prefix and null terminator
    sprintf(player_fifo, "/tmp/%s", player_name);
    mkfifo(player_fifo, FIFO_MODE);

    // Send player name to server
    write(server_fd, player_name, strlen(player_name)+1);

    // Open FIFO for communication with server
    int player_fd = open(player_fifo, O_RDONLY);
    if (player_fd == -1) {
        printf("Error opening player FIFO\n");
        exit(EXIT_FAILURE);
    }

    printf("%s connected to server!\n", player_name);

    while (score < TARGET_SCORE) {
        // Wait for server to signal it's our turn to play
        char player_turn;
        read(player_fd, &player_turn, sizeof(char));
        if (player_turn != '1') {
            printf("Unexpected turn signal: %c\n", player_turn);
            exit(EXIT_FAILURE);
        }

        // Roll dice and update score
        int roll = roll_dice();
        score += roll;

        // Display roll and score
        printf("%s rolled %d (total score: %d)\n", player_name, roll, score);

        // Check if we won
        if (score >= TARGET_SCORE) {
            printf("%s won!\n", player_name);
            write(server_fd, "0", sizeof(char)); // signal end of game to server
            close(server_fd);
            close(player_fd);
            unlink(player_fifo);
            exit(EXIT_SUCCESS);
        }

        // Signal server it's their turn to play
        write(server_fd, "1", sizeof(char));
    }

    printf("%s lost!\n", player_name);

    close(server_fd);
    close(player_fd);
    unlink(player_fifo);

    return 0;
}
