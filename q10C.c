#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024

int main() {
    int shmid, *shm_ptr, result;
    char operator;

    int shmid_car;
    char *shm_ptr_car;
    int shmid_nb, *shm_ptr_nb;
    key_t  key_car, key_nb;

    key_car=0662;
    key_nb=0661;
    // get shared memory segment created by server
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

    printf("Enter two numbers and an operator (+, -, *, /): ");
    scanf("%d %d %c", &shm_ptr_nb[0], &shm_ptr_nb[1],&shm_ptr_car[0]);


    // read input from shared memory segment and print
    printf("Operation: %d %c %d\n", shm_ptr_nb[0], shm_ptr_car[0], shm_ptr_nb[1]);

    shm_ptr_nb[4]=1;

    // read result from shared memory segment and print
    while(shm_ptr_nb[4]==1)
    {
        sleep(1);
    }

    result = shm_ptr_nb[3];

    printf("Result: %d\n", result);

    shm_ptr_nb[3]=-1;
    // detach from shared memory segment
    shmdt(shm_ptr_nb);
    shmdt(shm_ptr_car);

    return 0;
}
