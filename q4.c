#include <unistd.h> 
#include <stdio.h> 
#include <string.h>
#include <sys/wait.h> 
#include <stdlib.h> 
int main(int argc, char* argv[]) { 
int pid,status; 
char cmd[100];
int background = 0;
char* arg1;
char* arg2;

while(1){
    printf("> ");
    fgets(cmd, 100, stdin);

    if (cmd[strlen(cmd) - 2] == '&') {
        cmd[strlen(cmd) - 2] = '\0';
        background = 1;
    }

    if (cmd=="exit") exit(0);
    
    int init_size = strlen(cmd);
	char delim[] = " ";

	char *ptr = strtok(cmd, delim);

    arg1=ptr;
    ptr = strtok(NULL, delim);
    arg2=ptr;

    char* arg_list[3] = {arg1, arg2, (char *)0};

     pid = fork();
    if (pid == 0) {
        // child process
        execvp(cmd, arg_list);
        printf("Error: Command not found\n");
        exit(1);
    } else if (pid > 0) {
        // parent process
        if (!background)
            waitpid(pid, &status, 0);
    } else {
        printf("Error: Failed to fork child process\n");
        exit(1);
    }

    background = 0;
}
return 0;

}
