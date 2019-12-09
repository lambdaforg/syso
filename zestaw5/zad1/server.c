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
#define FAILURE_EXIT(format, ...) { fprintf(stderr, format, ##__VA_ARGS__); exit(-1); }
 
// Kolejki klientow i serwera, licznik klientow
int queue_descriptor = -2;  
int clients_queue[MAX_CLIENTS][2]; 
int counterClient = 0;

void printError(char *text){
	printf("%s", text);
	
	exit(-1);
}
struct Message createMessage(long type, pid_t id){
	Message msg;
	msg.msg_type = type;
	msg.idSender = id;
	return msg;
}
int findClient(int clientID){
	
		for(int i = 0 ; i < MAX_CLIENTS; i++){
				
				if(clients_queue[i][0] == clientID){
					return i;
				}
			
		}
	
	return -1;
}
void handleStop(struct Message *msg){
	printf("DELETING USER QUEUE FROM SERVER, USER WAS STOPPED.\n");
	fflush(stdout);
	
	int clientID = msg->idSender;
	int index = findClient(clientID);
		
	clients_queue[index][0] = -1;
	clients_queue[index][1] = -1;

}
void handleLogin(struct Message *msg){
	
			key_t clientQueueKey;
			//klucz klienta
			if (sscanf(msg->msg_text, "%d", &clientQueueKey) < 0)
			printError("erorr");
			// kolejka klienta
			int clientQueue = msgget(clientQueueKey, 0);
			//ppid klienta
			int clientID = msg->idSender;
		
			
			Message msgo = createMessage(LOGIN, getpid());
	
			if(counterClient < MAX_CLIENTS )
			{
				printf("Adding new client!\n");
				fflush(stdout);
				
				clients_queue[counterClient][0] = clientID;
				clients_queue[counterClient][1] = clientQueue;
				sprintf(msgo.msg_text, "%d", counterClient);
				
				counterClient++;
				
			}

		
		if (msgsnd(clientQueue, &msgo, MSG_SIZE, 0) == -1)
			printError("server: error with client queu");
}
void handleTime(struct Message *msg){

		int index = findClient(msg->idSender);
		if(index == -1)
		{
			printError("not found queue client");
			return;
		}
		Message msgo = createMessage(TIME,getpid());
		char buff[20];
		time_t now = time(NULL);
		strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));	
		sprintf(msgo.msg_text, "%s", buff);
		
	    if(msgsnd(clients_queue[index][1], &msgo, MSG_SIZE, 0) == -1)
				printError("server: Error with time sender\n");
}
void handleEnd(struct Message *msg){
	exit(1);
}
void close_queue(){
		msgctl(queue_descriptor, IPC_RMID, NULL);
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
void close_singal(int signum){
		exit(1);
}
int main(){
		//funkcja wykonywana przy zamknieciu serveruu
		atexit(close_queue);
		signal(SIGINT, close_singal);
		
		
		char *path = getenv("HOME");
		 if(path == NULL)
			printError("server: Error with path\n");
			
		key_t serverKey = ftok(path, PROJECT_ID);
		if(serverKey == -1){
			printError("Erro with serverKey");
		 }	
		queue_descriptor = msgget(serverKey, IPC_CREAT | IPC_EXCL | 0666);
		 if(queue_descriptor == -1){
			printf("Erro with queue_descriptor");
			queue_descriptor = msgget(serverKey, 0);
			close_queue();
			exit(1);
		}
		
		Message msg;
		while(1){
		 if (msgrcv(queue_descriptor, &msg, MSG_SIZE, 0, 0) < 0){
					  printError("server: receiving message failed\n");
		 }
			
			handle_request(&msg);
			
		}
		
}
