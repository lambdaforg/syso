#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include "info.h"

int idShared;
sem_t *semaphoreID;
void initBarber(){
 
     idShared = shm_open( PROJECTPATH ,O_RDWR | O_CREAT | O_EXCL, S_IRWXU | S_IRWXG);
		
		
		if(idShared == -1){
			printf("t");
				exit(-1);
				
		}
		if(ftruncate(idShared, sizeof(*BarberSem)) == -1){
			printf("t2");
			exit(-1);
			}

		BarberSem = mmap( NULL, sizeof(*BarberSem),PROT_READ | PROT_WRITE, MAP_SHARED, idShared, 0);
	 if (BarberSem == (void*)(-1)) {
			//perror("shmat");
			printf("t3");
			exit(1);
		}
	  semaphoreID = sem_open(PROJECTPATH, O_WRONLY | O_CREAT | O_EXCL, S_IRWXU | S_IRWXG, 0);
		
		if(semaphoreID == (void*) -1){
			printf("t4");
			exit(0);
		}	

	 
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

	printf("Invite client with pid_t %d\n", BarberSem->currentClient); 
}
void goToSleep(){
		
	BarberSem->state = SLEEP;
	printf("Going to sleep at %s\n", currentTime());
}
void shave(){
	
	printf("Start cutting client at %s with pid_t %d \n",  currentTime(), BarberSem->currentClient);
	/* Sleep albo cos */
	printf("Finished cutting client at %s with pid_t %d\n",  currentTime(), BarberSem->currentClient);
	
	BarberSem->currentClient = 0;
	BarberSem->state = IDLE;
}
void barberOperations(){
	
	switch(BarberSem->state){
		case SHAVE:{
			if(BarberSem->currentClient != BarberSem->queueClients[0])
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
			printf("Awaken by client %s\n", currentTime());
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
	munmap(BarberSem, sizeof(BarberSem));
	sem_close(semaphoreID);
    if(semaphoreID != 0) {
        sem_unlink(PROJECTPATH);
    }
    if(idShared != 0) {
        shm_unlink(PROJECTPATH);
    }
	
}

int main(){
	
		signal(SIGINT, handleINT);
		atexit(removeSemaphore);
		initBarber();
		giveSem(semaphoreID);
	
	
	while(1){
	takeSem(semaphoreID);
	barberOperations();
	giveSem(semaphoreID);
		
	}
	
		return 0;
}



