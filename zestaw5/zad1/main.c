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
#include <errno.h>

#include "info.h"

// kolejka Servera i klienta
int queue_descriptor = -1;  
int queue_Client = -1;


void close_queue(){
	msgctl(queue_Client, IPC_RMID, NULL);
}
void printError(char *text){
	printf("%s", text);
	close_queue();
	
	exit(-1);
}
struct Message createMessage( long type, pid_t id, char *text){
	Message msg;
	msg.msg_type = type;
	msg.idSender = id;
	sprintf(msg.msg_text, "%s", text);
	return msg;
}
void addingClientToServer(key_t ClientKey){
	
	
		
	    Message msg = createMessage(LOGIN,getpid(), "L");
		sprintf(msg.msg_text, "%d", ClientKey);
		int sessionID;
		
		if (msgsnd(queue_descriptor, &msg, MSG_SIZE, 0) == -1){
			 printError("Client: LOGIN request failed\n");	 
		}
		 if (msgrcv(queue_Client, &msg, MSG_SIZE, 0, 0) == -1) 
			printError("Client: catching LOGIN response failed\n");
		if (sscanf(msg.msg_text, "%d", &sessionID) < 1)
			printError("Client: Storing message failed\n");
		if (sessionID < 0){
			printError("Client: Server has a maximum Clients\n");
		}
		if(msg.msg_type == LOGIN){
		 printf("Client: Client registered. Session no: %d\n", sessionID);
		}
}

void timeSender(key_t ClientKey){
	Message msg = createMessage(TIME,getpid(),"Time not found");
	if(msgsnd(queue_descriptor, &msg, MSG_SIZE, 0) == -1)
         printError("Client: Sender failed");
    if(msgrcv(queue_Client, &msg, MSG_SIZE, 0, 0) == -1)
         printError("Client: Fetching time failed");
    
	printf("%s\n", msg.msg_text);
}
void deleteQueue(int var){
		int type;
		switch(var){
			case 1:
			{
				type = STOP;
			}
			break;
			case 2:
			{
				type = END;
			}
			break;
		}
	 Message msg = createMessage(type,getpid(), "dQ");
	if(msgsnd(queue_descriptor, &msg, MSG_SIZE, 0) == -1)
         printf("Client: Sender failed");
	//msgctl(queue_Client, IPC_RMID, NULL);
	exit(1);
}
void endSender(){
	deleteQueue(2);
}
void close_singal(int signum){
	deleteQueue(1);
}
int main(){
		srand(time(0)); 
		//usuwa kolejke gdy zakonczy sie program
		atexit(close_queue);
		signal(SIGINT, close_singal);
		
		char* path = getenv("HOME");
		int key = ftok(path, PROJECT_ID);
		queue_descriptor = msgget(key, 0);
		if(queue_descriptor == -1){
			printError("Error with queue");
		}
		printf("%s", path);
		
		//Tworzenie kolejki klienta
		int data = rand();
		printf("%d", data);
		key_t ClientKey = ftok(path,rand() ); 
		queue_Client = msgget(ClientKey, 0666 | IPC_CREAT); 
		
		char fromUser[20];
		//dodanie kolejki do serwera
		addingClientToServer(ClientKey);
		
		while(1){

			 printf("Client: enter your request: ");
			if (fgets(fromUser, 20, stdin) == NULL){
				printf("Client: [ TIME ] OR [ END ]\n");
				continue;
			}
			int n = strlen(fromUser);
			if (fromUser[n-1] == '\n') fromUser[n-1] = 0;
			 
			if (strcmp(fromUser, "TIME") == 0) {
				timeSender(ClientKey);
			} else if (strcmp(fromUser, "END") == 0) {
				endSender(); 
			}
		}
		
}
