#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <signal.h>
#include <semaphore.h>
#include <unistd.h>


#include "info.h"
enum Client status;
sem_t *semaphoreID;

void initClient(){

	int idSem = shm_open(PROJECTPATH, O_RDWR, S_IRWXU | S_IRWXG);

    if (idSem == -1)
		exit(0);

    BarberSem = mmap(NULL,sizeof(*BarberSem),PROT_READ | PROT_WRITE,MAP_SHARED,idSem,0);
    if (BarberSem == (void*) -1)
     	exit(0);
	
    semaphoreID = sem_open(PROJECTPATH, O_WRONLY);
    if (semaphoreID == (void*) -1)
        exit(0);
}
void addToQueue(){
	
		pid_t clientPID = getpid();
		
		
		if(BarberSem->state == SLEEP){
			
			BarberSem->state = AWAKEN;
			BarberSem->queueClients[BarberSem->countClients] = clientPID;
			BarberSem->countClients++;
			printf("I woke up a barber");
			fflush(stdout);
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
			fflush(stdout);
	
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
						fflush(stdout);
						return;
					}
					else{
						printf("test6");
						fflush(stdout);
						addToQueue();
						status = WAITING;
					}
				giveSem(semaphoreID);
				
					while(1){
							takeSem(semaphoreID);
					
								if(BarberSem->currentClient == currentClient){
									status = INVITED;
									printf("%s #%i: invited\n", currentTime(), currentClient);	
									getBarberChair();
									fflush(stdout);
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
							fflush(stdout);
							BarberSem->state = IDLE;
							BarberSem->currentClient = -1;
							giveSem(semaphoreID);
							break;

						}
						giveSem(semaphoreID);
					}
						
							
					if(status == FINISHED){
						
						//printf("%s #%i: Left the barbershop\n", currentTime(), currentClient);
						//fflush(stdout);
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
