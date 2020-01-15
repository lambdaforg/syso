#ifndef INFO_H
#define INFO_H

#define MAX_CLIENTS  10
#define PROJECT_ID 0x033
#define PROJECTPATH getenv("HOME")
#define MAX_CONT_SIZE 4096

enum State {
    SLEEP = 1, SHAVE = 2, AWAKEN = 3, IDLE = 4,
};
enum Client {
    INVITED = 1, FINISHED = 2, WAITING = 3, 
};
struct BarberSem {
    enum State state;
	//licznik semafora
	int countClients;
    pid_t currentClient;
    pid_t queueClients[MAX_CLIENTS];
} *BarberSem;

void takeSem(int idSem) {
    struct sembuf request;
	request.sem_num = 0;
	request.sem_op = -1;
	request.sem_flg = 0;

    if (semop(idSem, &request, 1)) // 1 goes for one operation
		printf("Error");
      //  FAIL("Could not update semaphore\n");
}

void giveSem(int idSem) {
    struct sembuf request;
    request.sem_num = 0;
	request.sem_op = 1;
    request.sem_flg = 0;

    if (semop(idSem, &request, 1)) // 1 goes for one operatio
			printf("Error");
      //  FAIL("Could not update semaphore\n");
}
char* currentTime(){
		time_t rawtime;
		struct tm * timeinfo;
	
		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
	// printf ( "Current local time and date: %s", asctime (timeinfo) );
	return asctime(timeinfo);
	
}
int isQueueEmpty(){
	if(BarberSem->countClients > 0)
		return 1;
	
	return 0;
}
int isQueueFull(){
	if(BarberSem->countClients > MAX_CLIENTS)
		return 1;
	
	return 0;
}
void exitFromQueue(){
	
	//BarberSem->currentClient = queueClients[0];
	for(int i =0; i < BarberSem->countClients; i++){
		BarberSem->queueClients[i] = BarberSem->queueClients[i+1];
		
	}
	BarberSem->queueClients[BarberSem->countClients] = -1;
	if(BarberSem->countClients > 0)
	BarberSem->countClients--;
	
}
#endif