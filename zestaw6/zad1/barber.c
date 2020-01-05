#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>


#include "info.h"

int idShared;
int semaphoreID;
void initBarber(){
 

	key_t keyBarber = ftok(PROJECTPATH,PROJECT_ID);
		if(keyBarber == -1){
				exit(-1);
		}
		
     idShared = shmget(keyBarber, sizeof(BarberSem) ,S_IRWXU | IPC_CREAT);
		if(idShared == -1){
				exit(-1);
				
		}
		BarberSem = shmat(idShared, 0 ,0);
	 if (BarberSem == (void*)(-1)) {
			//perror("shmat");
			exit(1);
		}
	  semaphoreID = semget(keyBarber, 1, S_IRWXU | IPC_CREAT);
		
		if(semaphoreID == -1){
			exit(0);
		}	
	  semctl(semaphoreID, 0, SETVAL, 0);
	
	 
	  BarberSem->countClients = 0;
	  BarberSem->state = IDLE;
	  BarberSem->currentClient = -1;
	  for(int i = 0 ; i < MAX_CLIENTS ; i++){
			BarberSem->queueClients[i] = -1;
	  }
	  

}	

void takeClient(){
	
	BarberSem->currentClient = BarberSem->queueClients[0];
	BarberSem->state = SHAVE;

	printf("Invite client with pid_t %d", BarberSem->currentClient); 
}
void goToSleep(){
		
	BarberSem->state = SLEEP;
	printf("Going to sleep at %s", currentTime());
}
void shave(){
	
	printf("Start cutting client at %s with pid_t %d",  currentTime(), BarberSem->currentClient);
	/* Sleep albo cos */
	printf("Finished cutting client at %s with pid_t %d",  currentTime(), BarberSem->currentClient);
	
	BarberSem->currentClient = 0;
	BarberSem->state = IDLE;
}
void barberOperations(){
	
	switch(BarberSem->state){
		case SHAVE:{
			shave();
		}
		break;
		case IDLE:{
				if(isQueueEmpty() == 1 && BarberSem->currentClient == -1){
					takeClient();
				}
				else if(isQueueEmpty() == 0){
					goToSleep();
				}
		}break;
		case AWAKEN:
		{
			printf("Awaken by client %s,", currentTime());
			BarberSem->state = IDLE;
			
		}
		break;
		default:
			break;
	}


}
void handleINT(int signum){
		printf("i was killed?");
		exit(1);
}
void removeSemaphore(){
	shmdt(BarberSem);
    if(semaphoreID != 0) {
        semctl(semaphoreID, 0, IPC_RMID);
    }
    if(idShared != 0) {
        shmctl(idShared, IPC_RMID, NULL);
    }
	
}

int main(){
	
	
		signal(SIGINT, handleINT);
		//atexit(removeSemaphore);
		initBarber();
		giveSem(semaphoreID);
	
	
	while(1){
	takeSem(semaphoreID);
	barberOperations();
	giveSem(semaphoreID);
		
	}
	
		return 0;
}



