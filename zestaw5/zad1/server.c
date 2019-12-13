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
int Clients_queue[MAX_CLIENTS][2]; 
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
int findClient(int ClientID){
	
		for(int i = 0 ; i < MAX_CLIENTS; i++){
				
				if(Clients_queue[i][0] == ClientID){
					return i;
				}
			
		}
	
	return -1;
}
void handleStop(struct Message *msg){
	printf("DELETING USER QUEUE FROM Server, USER WAS STOPPED.\n");
	fflush(stdout);
	
	int ClientID = msg->idSender;
	int index = findClient(ClientID);
		
	Clients_queue[index][0] = -1;
	Clients_queue[index][1] = -1;

}
void handleLogin(struct Message *msg){
	
			key_t ClientQueueKey;
			//klucz klienta
			if (sscanf(msg->msg_text, "%d", &ClientQueueKey) < 0)
			printError("erorr");
			// kolejka klienta
			int ClientQueue = msgget(ClientQueueKey, 0);
			//ppid klienta
			int ClientID = msg->idSender;
		
			
			Message msgo = createMessage(LOGIN, getpid());
	
			if(counterClient < MAX_CLIENTS )
			{
				printf("Adding new Client!\n");
				fflush(stdout);
				
				Clients_queue[counterClient][0] = ClientID;
				Clients_queue[counterClient][1] = ClientQueue;
				sprintf(msgo.msg_text, "%d", counterClient);
				
				counterClient++;
				
			}
			else{
				printf("Too much Clients!\n");
				fflush(stdout);
			
				sprintf(msgo.msg_text, "%d", -1);
			}

		
		if (msgsnd(ClientQueue, &msgo, MSG_SIZE, 0) == -1)
			printError("Server: error with Client queu");
}
void handleTime(struct Message *msg){

		int index = findClient(msg->idSender);
		if(index == -1)
		{
			printError("not found queue Client");
			return;
		}
		Message msgo = createMessage(TIME,getpid());
		char buff[20];
		time_t now = time(NULL);
		strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));	
		sprintf(msgo.msg_text, "%s", buff);
		
	    if(msgsnd(Clients_queue[index][1], &msgo, MSG_SIZE, 0) == -1)
				printError("Server: Error with time sender\n");
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
		//funkcja wykonywana przy zamknieciu Serveruu
		atexit(close_queue);
		signal(SIGINT, close_singal);
		
		
		char *path = getenv("HOME");
		 if(path == NULL)
			printError("Server: Error with path\n");
			
		key_t ServerKey = ftok(path, PROJECT_ID);
		if(ServerKey == -1){
			printError("Error with ServerKey");
		 }	
		queue_descriptor = msgget(ServerKey, IPC_CREAT | IPC_EXCL | 0666);
		 if(queue_descriptor == -1){
			printf("Error with queue_descriptor");
			queue_descriptor = msgget(ServerKey, 0);
			close_queue();
			exit(1);
		}
		
		Message msg;
		while(1){
		 if (msgrcv(queue_descriptor, &msg, MSG_SIZE, 0, 0) < 0){
					  printError("Server: receiving message failed\n");
		 }
			
			handle_request(&msg);
			
		}
		
}
