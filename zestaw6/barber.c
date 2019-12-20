#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <ctype.h>
#include <time.h>
#include <signal.h>


#include "info.h"

  
int idSem = -1;  
int idUsingSem = -1;
void initBarber(){
 

	key_t keyBarber = ftok(PROJECTPATH,PROJECT_ID);
		if(keyBarber == -1){
				exit(-1);
		}
		
     idSem = shmget(keyBarber, sizeof(BarberSem) ,S_IRWXU | IPC_CREAT);
		if(idSem == -1){
				exit(-1);
				
		}
		BarberSem = shmat(idSem, 0 ,0);
	 if (BarberSem == (char *)(-1)) {
			//perror("shmat");
			exit(1);
		}
	  idUsingSem = semget(keyBarber, 1, S_IRWXU | IPC_CREAT);
		
		if(idUsingSem == -1){
			exit(0);
		}	
	  semctl(idUsingSem, 0, SETVAL, 0);
	
	 
	  BarberSem->countClients = 0;
	  BarberSem->state = ZERO;
	  BarberSem->currentClient = -1;
	  for(int i = 0 ; i < MAX_CLIENTS ; i++){
			BarberSem->queue_clients[i] = -1;
	  }
	  

}	
void handleINT(int signum){
		printf("i was killed?");
		exit(1);
}
// CLIENT
int main(){
		signal(SIGINT, handleINT);
		//atexit(removeSemaphore);
		
		initBarber()
		giveSem(idUsingSem);
		while(1){
			takeSem(idUsingSem);
				switch(BarberSem->state){
					case ZERO:
						if(!isQueueEmpty()){
							//printf("Trying to invite client");
							inviteClient();
							BarberSem->state = SHAVE;
						}
						else{
							//printf("Barber sleep");
							BarberSem->state = SLEEP;
							
						}
					break;
					case AWAKLEN:
					     printf("Barber woke up");
						 BarberSem->state = SHAVE;
					break;
					case 
				
				}
			
		}

	
		
}
