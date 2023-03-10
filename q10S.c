#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024

int main() {
    int shmid_car;
    char *shm_ptr_car;
    int shmid_nb, *shm_ptr_nb;
    key_t  key_car, key_nb;

    key_car=0662;
    key_nb=0661;

    if ((shmid_car = shmget(key_car, SHM_SIZE, IPC_CREAT | 0666)) < 0) 
    { 
        perror("shmget");
        exit (1) ;
    }
    //Attach the segment
    if ((shm_ptr_car = shmat(shmid_car, NULL, 0)) == (char *)-1) 
    { 
        perror ("shmat") ;
        exit (1) ;
    }

      if ((shmid_nb = shmget(key_nb, SHM_SIZE, IPC_CREAT | 0666)) < 0) 
    { 
        perror("shmget");
        exit (1) ;
    }
    //Attach the segment
    if ((shm_ptr_nb = shmat(shmid_nb, NULL, 0)) == (int *)-1) 
    { 
        perror ("shmat") ;
        exit (1) ;
    }
while(1){
    // read input from user
   if (shm_ptr_nb[4]==1)
    {
         // perform calculation
        switch (shm_ptr_car[0]) {
        case '+':
            shm_ptr_nb[3] = shm_ptr_nb[0] + shm_ptr_nb[1];
            break;
        case '-':
            shm_ptr_nb[3] = shm_ptr_nb[0] - shm_ptr_nb[1];
            break;
        case '*':
            shm_ptr_nb[3] = shm_ptr_nb[0] * shm_ptr_nb[1];
            break;
        case '/':
            if (shm_ptr_nb[1] == 0) {
                printf("Error: division by zero\n");
                exit(EXIT_FAILURE);
            }
            shm_ptr_nb[3] = shm_ptr_nb[0] / shm_ptr_nb[1];
            break;
        default:
            printf("Error: invalid operator\n");
            exit(EXIT_FAILURE);
    }
        shm_ptr_nb[4]=0;
    } }

    // detach from shared memory segment
    shmdt(shm_ptr_nb);
    shmdt(shm_ptr_car);

    return 0;
}
