#include <unistd.h> 
#include <stdio.h> 
#include <string.h>
#include <sys/wait.h> 
#include <stdlib.h> 
int main(int argc, char* argv[]) { 
int pid; 
char cmd[100];
char* arg1;
char* arg2;
if ((pid = fork())==0){ 
    printf("> ");
    fgets(cmd, 100, stdin);

	int init_size = strlen(cmd);
	char delim[] = " ";

	char *ptr = strtok(cmd, delim);

    arg1=ptr;
    ptr = strtok(NULL, delim);
    arg2=ptr;

    char* arg_list[3] = {arg1, arg2, (char *)0};

    execvp(cmd,arg_list); 
} 
else{ 
    int status; 
    int term_pid = wait(&status); 
    exit(1); } 
} 