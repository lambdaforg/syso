#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

#include "info.h"
enum Client status;
int semaphoreID;

void initClient(){
	//dodaje sie do kolejki
    key_t keyBarber = ftok(PROJECTPATH, PROJECT_ID);
	
	int idSem = shmget(keyBarber, sizeof(BarberSem), 0);
    if (idSem == -1)
		exit(0);

    BarberSem = shmat(idSem, 0, 0);
    if (BarberSem == (void*) -1)
     	exit(0);
	
    semaphoreID = semget(keyBarber, 0, 0);
    if (semaphoreID == -1)
        exit(0);
}
void addToQueue(){
	
		pid_t clientPID = getpid();
		
		
		if(BarberSem->state == SLEEP){
			
			BarberSem->state = AWAKEN;
			BarberSem->queueClients[BarberSem->countClients] = clientPID;
			BarberSem->countClients++;
			printf("I woke up a barber");
			//while(BarberSem->state != SHAVE){
			//		giveSem(semaphoreID);
			//		takeSem(semaphoreID);
			//}
		}
		else if(BarberSem->state != SLEEP){
			// add to queueuue 
			BarberSem->queueClients[BarberSem->countClients] = clientPID;
			BarberSem->countClients++;
		}
	
}
void getBarberChair(){
	
		if(status == INVITED){
				exitFromQueue();
				
		}
			printf("Sit on the chair!!");
	
	
}

void getFancyHair(){
			pid_t currentClient = getpid();
			status = WAITING;
			printf("test3");
			fflush(stdout);
			while(1)
			{
				printf("test5");
					fflush(stdout);
				takeSem(semaphoreID);
					if(isQueueFull() == 1){
						printf("Fuck full barbershop.. %s", currentTime());
						return;
					}
					else{
						addToQueue();
						status = WAITING;
					}
				giveSem(semaphoreID);
				
					while(1){
							takeSem(semaphoreID);
								if(BarberSem->currentClient == currentClient){
									status = INVITED;
									getBarberChair();
									printf("%s #%i: invited\n", currentTime(), currentClient);
									giveSem(semaphoreID);
									break;
								}
							giveSem(semaphoreID);
						
					}
					 while(1) {
						takeSem(semaphoreID);
						if (BarberSem->currentClient == 0) {
							status = FINISHED;
							printf("%s #%i: shaved\n", currentTime(), currentClient);
							BarberSem->state = IDLE;
							BarberSem->currentClient = -1;
							giveSem(semaphoreID);
							break;

						}
						giveSem(semaphoreID);
					}
						
							
					if(status == FINISHED){
						printf("%s #%i: Left the barbershop\n", currentTime(), currentClient);
							break;
					}
					
				
			}
	
}


int main(){
	
	initClient();
		
		for(int i = 0; i <3 ;i++){
			pid_t childID = fork();
				if(childID == 0){
					printf("test");
					fflush(stdout);
					getFancyHair();
					exit(0);
				}
			
		}
		
		while(1){
			wait(0);
		}
	
	
	
	return 0;
}
