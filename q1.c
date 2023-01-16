#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t child1, child2;
    child1 = fork();
    if (child1 != 0) {
        child2 = fork();
        if (child2 != 0) {
            printf("Parent: PID = %d, Child 1 PID = %d, Child 2 PID = %d\n", getpid(), child1, child2);
        }
    }
    return 0;
}

/* Solution Question 1 

#include <stdio.h>

#include <unistd.h>

int main(){

        int pid = fork();

        if (pid>0) pid = fork();

        printf("PID = %d PPID = %d\n",getpid(),getppid());

        return 0;

}

*/ 