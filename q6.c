#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


int main() {    
    int pipe_fd[2]; //pipe[0] : READ pipe [1] : WRITE
    pid_t pid; //process id
    char buffer[100]; //buffer to store the result
    int num1, num2; //numbers to be operated
    char operator; //operator
    int result;     //result of the operation

    while (1) { //infinite loop
        printf("Enter two numbers and an operator : "); //prompt
        scanf("%d %d %c", &num1, &num2, &operator);     //read input

        if (pipe(pipe_fd) == -1) { //create pipe
            printf("Pipe creation failed"); //error
            return 1; //exit
        }

        pid = fork(); //create child process

        if (pid < 0) { //error
            printf("Fork failed"); //error
            return 1;   //exit
        } else if (pid > 0) { //parent process
            close(pipe_fd[1]); //close write end
            read(pipe_fd[0], buffer, 100); //read from pipe
            printf("Result: %s\n", buffer);     //print result
            close(pipe_fd[0]); //close read end
        } else {
            close(pipe_fd[0]); //close read end
            switch (operator) { //perform operation
                case '+': //addition
                    result = num1 + num2;
                    break;
                case '-': //subtraction
                    result = num1 - num2;
                    break;
                case '*': //multiplication
                    result = num1 * num2;
                    break;
                case '/': //division
                    result = num1 / num2;
                    break;
                default: 
                    printf("Invalid operator\n");   
                    exit(1);
            }
            sprintf(buffer, "%d", result); //convert result to string
            write(pipe_fd[1], buffer, strlen(buffer)); //write to pipe
            close(pipe_fd[1]); //close write end
            exit(0); //exit child process
        }
    }

    return 0;
}