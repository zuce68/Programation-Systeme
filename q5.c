#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void myAlarmHandler(int);
void myAlarmHandler(int dummy){}; // to avoid quittinge

int main() {
    pid_t child;
    float a, b;
    char op;
    FILE *f;

    child = fork();
    if (child == 0) {
            signal(SIGALRM, myAlarmHandler);
            //I have to wait for my parent
            pause();
            //OK, now I can do my job
            while (!(f = fopen("temp.txt", "r"))) {
                sleep(1);
            }
            fscanf(f, "%f %f %c", &a, &b, &op);
            fclose(f);
            unlink("temp.txt");
            switch (op) {
                case '+':
                    printf("%.2f\n", a + b);
                    break;
                case '-':
                    printf("%.2f\n", a - b);
                    break;
                case '*':
                    printf("%.2f\n", a * b);
                    break;
                case '/':
                    printf("%.2f\n", a / b);
                    break;
                default:
                    printf("Invalid operator\n");
                    break;
            }
            sleep(1);
            //Child exit
    } else {
            while ((f = fopen("temp.txt", "r"))) {
                fclose(f);
                sleep(1);
            }
            printf("Enter two numbers and an operator (+, -, *, /): ");
            scanf("%f %f %c", &a, &b, &op);
            f = fopen("temp.txt", "w");
            fprintf(f, "%f %f %c\n", a, b, op);
            fclose(f);
            sleep(3);
            kill(child, SIGALRM); // let the child wake up
            //Parent Exiting
    }
    return 0;
}
