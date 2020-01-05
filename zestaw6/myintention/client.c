#include <stdio.h>


enum Client status;

Customer {
    while(1) {
        sem_wait(accessSeats) // protects seats so only 1 thread tries to sit in a chair if that's the case
        if(NumberOfFreeSeats > 0) {
            NumberOfFreeSeats-- // sitting down
            sem_post(Customers) // notify the barber
            sem_post(accessSeats) // release the lock
            sem_wait(Barber) // wait in the waiting room if barber is busy
            // customer is having hair cut
        } else {
            sem_post(accessSeats) // release the lock
            // customer leaves
        }
   }
   	int countClients;
    pid_t currentClient;
    pid_t queueClients[MAX_CLIENTS];
   
}
void addToQueue(){
	
		pid_t clientPID = getpid();
		
		
		if(BarberSem->status == SLEEP){
			
			BarberSem->status = AWAKEN;
			BarberSem->currentClient = clientPID;
			printf("I woke up a barber");
			while(BarberSem->status != SHAVE){
					giveSem(semaphoreID);
					takeSem(semaphoreID);
			}
		}
		else{
			// add to queueuue 
			BarberSem->queueClients[countClients] = clientPid;
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
			while(status != FINISHED)
			{
				takeSem(semaphoreID);
					if(isQueueFull){
						printf("Fuck full barbershop.. %lo", current_time());
						return;
					}
					else{
						addToQueue();
						status = waiting;
					}
				teraz pedal musi czekac bo jest waiting to musi miec loopa raczej :OO 
				giveSem(semaphoreID);
				
					while(1){
							takeSem(semaphoreID);
								if(BarberSem->currentClient == currentClient){
									status = INVITED;
									getBarberChair();
									
								}
							giveSem(semaphoreID);
						
					}
					 while(status != FINISHED) {
						takeSem(semaphoreID);
						if (BarberSem->currentClient != currentClient) {
							status = FINISHED;
							printf("%lo #%i: shaved\n", current_time(), currentClient);
							BarberSem->state = IDLE;

						}
						giveSem(semaphoreID);
					}
				
			}
	
}


int main(){
	
	
	
	
	
	return 0;
}
