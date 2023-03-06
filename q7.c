#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>


void player(char *s, int *fd1, int *fd2){
    int points=0; 
    int dice;
    long int ss=0; 
    char turn; 
    close(fd1[1]); 
    close(fd2[0]); 
    while(1){
        read(fd1[0],&turn,1);
        printf("%s: playing my dice\n", s);
        dice =(int) time(&ss)%10 + 1;
        printf("%s: got %d points\n", s, dice); points+=dice;
        printf("%s: Total so far %d\n\n", s, points); 
        if(points >= 50){
            printf("%s: game over I won\n", s);
            kill(0, SIGTERM); 
        }
        sleep(5); // to slow down the execution
        write(fd2[1], &turn, 1); 
    }
}


int main(int argc, char *argv[]){
int fd1[2], fd2[2], fd3[2], fd4[2], fd5[2], fd6[2], fd7[2], fd8[2]; 
char turn="T";
printf("This is a 4-player game with a referee\n");
pipe(fd1);
pipe(fd2);
pipe(fd5);
pipe(fd6);
if(!fork())
player("TOTO",fd1,fd2);
close(fd1[0]); 
close(fd2[1]);
pipe(fd3); pipe(fd4); 
if(!fork())
player("TITI",fd3,fd4);
close(fd3[0]); 
close(fd4[1]);
if(!fork())
player("THREE",fd5,fd6);
close(fd5[0]); 
close(fd6[1]);
if(!fork())
player("FOR",fd7,fd8);
close(fd7[0]); 
close(fd8[1]);
// parent only write to pipe 3 // parent only reads from pipe 4
while(1){
printf("\nReferee: TOTO plays\n\n"); 
write(fd1[1], &turn, 1); 
read(fd2[0], &turn, 1);
printf("\nReferee: TITI plays\n\n"); 
write(fd3[1], &turn, 1); 
read(fd4[0], &turn, 1);
printf("\nReferee: THREE plays\n\n"); 
write(fd5[1], &turn, 1); 
read(fd6[0], &turn, 1);
printf("\nReferee: FOR plays\n\n"); 
write(fd7[1], &turn, 1); 
read(fd8[0], &turn, 1);
} }


