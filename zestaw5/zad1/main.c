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

// kolejka servera i klienta
int queue_descriptor = -1;  
int queue_client = -1;


void printError(char *text){
	printf("%s", text);
	
	exit(-1);
}
struct Message createMessage( long type, pid_t id, char *text){
	Message msg;
	msg.msg_type = type;
	msg.idSender = id;
	sprintf(msg.msg_text, "%s", text);
	return msg;
}
void addingClientToServer(key_t clientKey){
	
	
		
	    Message msg = createMessage(LOGIN,getpid(), "L");
		sprintf(msg.msg_text, "%d", clientKey);
		int sessionID;
		
		if (msgsnd(queue_descriptor, &msg, MSG_SIZE, 0) == -1){
			 printError("client: LOGIN request failed\n");	 
		}
		 if (msgrcv(queue_client, &msg, MSG_SIZE, 0, 0) == -1) 
			printError("client: catching LOGIN response failed\n");
		if (sscanf(msg.msg_text, "%d", &sessionID) < 1)
			printError("client: scanning LOGIN response failed\n");
		if (sessionID < 0){
			printError("client: server cannot have more clients\n");
		}
		if(msg.msg_type == LOGIN){
		 printf("client: client registered. Session no: %d\n", sessionID);
		}
}

void timeSender(key_t clientKey){
	Message msg = createMessage(TIME,getpid(),"Time not found");
	if(msgsnd(queue_descriptor, &msg, MSG_SIZE, 0) == -1)
         printError("client: Sender failded");
    if(msgrcv(queue_client, &msg, MSG_SIZE, 0, 0) == -1)
         printError("client: Fetching time failded");
    
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
         printf("client: Sender failded");
	//msgctl(queue_client, IPC_RMID, NULL);
	exit(1);
}
void close_queue(){
	msgctl(queue_client, IPC_RMID, NULL);
}
void endSender(){
	deleteQueue(2);
}
void close_singal(int signum){
	deleteQueue(1);
}
int main(){
		char text[] = "progfile";
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
		key_t clientKey = ftok(text, 65); 
		queue_client = msgget(clientKey, 0666 | IPC_CREAT); 
		
		char fromUser[20];
		//dodanie kolejki do serwera
		addingClientToServer(clientKey);
		
		while(1){

			 printf("client: enter your request: ");
			if (fgets(fromUser, 20, stdin) == NULL){
				printf("client: [ TIME ] OR [ END ]\n");
				continue;
			}
			int n = strlen(fromUser);
			if (fromUser[n-1] == '\n') fromUser[n-1] = 0;
			 
			if (strcmp(fromUser, "TIME") == 0) {
				timeSender(clientKey);
			} else if (strcmp(fromUser, "END") == 0) {
				endSender(); 
			}
		}
		
}
