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

  
int queue_descriptor = -2;  
int clients_queue[MAX_CLIENTS][2]; 
// CLIENT
int main(){
		//funkcja wykonywana przy zamknieciu serveruu
		if (atexit(close_queue) == -1)

		
		
		char* path = getenv("HOME");
		key_t serverKey = ftok(path, PROJECT_ID);
		
		 if(serverKey == -1)
			printf("Erro with serverKey");		
		queue_descriptor = msgget(publicKey, IPC_CREAT | IPC_EXCL | 0666);
	
		 if(queue_descriptor == -1)
			printf("Erro with queue_descriptor");	

		
		Message msg;
		while(1){
		 if (msgrcv(queue_descriptor, &msg, MSG_SIZE, 0, 0) < 0)
					printf("Error with fetching data");
			handle_request(&msg);
		}
		
}
void handle_request(Message *msg){
	switch(msg->msg_type){
		case 1:
			handleTime(msg);
		break;
		case 2:
			handleEnd(msg);
		break;
		case 3:
			handleLogin(msg);
		break;
		case 4:
			handleStop(msg);
		break;
	}
	
}
void handleLogin(Message *msg){
	
		key_t clientQueueKey;
		//klucz klienta
	 if (sscanf(msg->message_text, "%d", &clientQueueKey) < 0)
			printf("erorr");
		
		// kolejka klienta
		int clientQueue = mssget(clientQueueKey, 0);
		
		//ppid klienta
		
		int clientID = msg->idSender;
		  //msg->msg_type = LOGIN;
			msg->idSender = getpid();
			
	
	
			if(counterClient < MAX_CLIENTS )
			{
				clients_queue[counterClient][0] = clientID;
				clients_queue[counterClient][1] = clientQueue;
				
			}
		counterClient++;
		
		if (msgsnd(clientQueueKey, msg, MSG_SIZE, 0) == -1)
			FAILURE_EXIT("server: LOGIN response failed\n");
}
void handleTime(Message *msg){

		int index = findClient(msg->idSender);
		if(index == -1)
		{
			printf("not found queue client");
			break;
		}
		
		msg->idSender = getpid();
		
		time_t timer;
		time(&timer);
		char* timeStr = convert_time(&timer);
		sprintf(msg->msg_text, "%s", timeStr);
		free(timeStr);
		
	    if(msgsnd(clients_queue[index][1], msg, MSG_SIZE, 0) == -1)
			FAILURE_EXIT("server: TIME response failed\n");
}
void handleEnd(Message *msg){
	
	int clientID = msg->idSender;
	int index = findClient(clientID);
		
	clients_queue[i][0] = -1;
	clients_queue[i][1] = -1;
	
	
}
int findClient(int clientID){
	
		for(int i = 0 ; i < MAX_CLIENTS; i++){
				if(clients_queue[i][0] == clientID){
					return i;
				}
			
		}
	
	return -1;
}
void close_queue(){
		msgctl(queue_descriptor, IPC_RMID, NULL);
}