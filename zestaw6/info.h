#ifndef INFO_H
#define INFO_H

#define MAX_CLIENTS  10
#define PROJECT_ID 0x033
#define PROJECTPATH getenv("HOME")
#define MAX_CONT_SIZE 4096

typedef enum State {
    SLEEP = 1, SHAVE = 2, AWAKEN = 3, ZERO = 4,
} state;
typedef enum Client {
    INVITED = 1, FINISHED = 2, WAITING = 3
};
typedef struct BarberSem {
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

    if (semop(idSem, &semaphore_request, 1)) // 1 goes for one operation
      //  FAIL("Could not update semaphore\n");
}

void giveSem(int idSem) {
    struct sembuf semaphore_request;
    request.sem_num = 0;
	request.sem_op = 1;
    request.sem_flg = 0;

    if (semop(idSem, &semaphore_request, 1)) // 1 goes for one operation
      //  FAIL("Could not update semaphore\n");
}
int isQueueEmpty(){
	if(countClients > 0)
		return 1;
	
	return 0;
}
int isQueueFull(){
	if(countClients > MAX_CLIENTS)
		return 1;
	
	return 0;
}
#endif