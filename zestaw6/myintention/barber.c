#include <stdio.h>



void takeClient(){
	
	BarberSem->currentClient = queueClients[0];
	BarberSem->state = SHAVE;

	printf("Cutting client with pid_t %d", BarberSem->currentClient); 
}
void goToSleep(){
		
	BarberSem->state = SLEEP;
	printf("Going to sleep at %lo", current_time());
}
void shave(){
	
	printf("Start cutting client at %lo with pid_t %d",  current_time(), BarberSem->currentClient);
	/* Sleep albo cos */
	printf("Finished cutting client at %lo with pid_t %d",  current_time(), BarberSem->currentClient);
	
	BarberSem->currentClient = -1;
	//BarberSem->state = IDLE;
}
void barberOperations(){
	
	switch(BarberSem->state){
		case SHAVE:{
			shave();
		}
		break;
		case IDLE:{
				if(!isQueueEmpty){
					takeClient();
				}
				else{
					goToSleep();
				}
		}break;
		case AWAKEN:
		{
			printf("Awaken by client %lo," current_time());
			BarberSem->state = SHAVE;
			
		}
		break;
	}


}
int main(){
	
	
	initBarber();
	
	
	while(1){
	takeSem(semaphoreID);
	barberOperations();
	giveSem(semaphoreID);
	
	}
	
		return 0;
}



