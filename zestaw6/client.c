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

state status;
int idSem;
int idUsingSem;
// CLIENT

//Queue handlers//
void addToQueue();
void deleteFromQueue();

/////////////////

void initClient(){
	//dodaje sie do kolejki
    key_t keyBarber = ftok(PROJETHPATH, PROJECT_ID);
	
	idSem = shmget(keyBarber, sizeof(struct Barber_info), 0);
    if (idSem == -1)
		exit(0);

    BarberSem = shmat(idSem, 0, 0);
    if (BarberSem == (void*) -1)
     	exit(0);
	
    idUsingSem = semget(keyBarber, 0, 0);
    if (sem_id == -1)
        exit(0);
}
void getChair(){
			if(status == INVITED){
				deleteFromQueue();
			}else if(status == WAITING){
					while(1){
						giveSem(idSem);
						takeSem(idSem);
						if(BarberSem->state == SHAVE) 
							break;
					}
				status == INVITED:
					
			}
	 BarberSem->currentClient = getpid();
	 printf("i get a chair", client_pid);
}
void getStaffFromBarber(){
		pid_t idClient = getpid();
		
		while(i < 5){
		take_sem(idUsingSem);
		
		if(BarberSem->state == SLEEP){
			printf("I am trying wake up barber");
			getChair();
			BarberSem->state = AWAKEN;
		}else if(isQueueFull){
			printf("Room is full");
		}else{
			 addToQueue();
		}
	 
	    release_sem(idUsingSem);
		 
		while(status != INVITED){
			takeSem(idSem);
			
			if(BarberSem->currentClient == idClient){
				status = INVITED;
				getChair();
				BarberSem->state = SHAVE;
			}
			
			giveSem(idSem);
		
		}
		
		while(status != FINISHED){
			takeSem(idSem);
			if(BarberSem->currentClient == idClient){
				status = FINISHED;
				printf("Client shaved");
				BarberSem->state = ZERO;
				i++;
			}
			
			giveSem(idSem);
			
		}
		}
		
}
int main(){
		initClient();
		
		for(int i = 0; i <3 ;i++){
			pid_t childID = fork();
				if(childID == 0){
					getStaffFromBarber();
					exit(0);
				}
			
		}
		
		while(1){
			wait(0);
		}
		
}
void addToQueue(){
	BarberSem->queueClients[BarberSem->countClients] = getpid();
	
	BarberSem->countClients = BarberSem->countClients + 1;
	
}
void deleteFromQueue(){
	for(int i = 0 ; i < BarberSem->countClients; i++){
		BarberSem->queueClients[i] = BarberSem->queueClients[i+1];
	}
	BarberSem->queueClients[i-1] = -1
	BarberSem->countClients = BarberSem->countClients - 1;
}