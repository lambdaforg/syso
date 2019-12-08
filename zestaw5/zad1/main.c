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

  
int queue_descriptor = -1;  
  
// CLIENT
int main(){
		
		char* path = getenv("HOME");
		int key = ftok(path, PROJECT_ID);
		queue_descriptor = msgget(key, 0);
		
		key_t clientKey = ftok("progfile", 65); 
		int msgid;
		msgid = msgget(clientKey, 0666 | IPC_CREAT); 
		
		Message msg;
		char fromUser[20];
		addingClientToServer(clientKey);
		
		while(1){

			 printf("client: enter your request: ");
			if (fgets(fromUser, 20, stdin) == NULL){
				printf("client: [ TIME ] OR [ END ]\n");
				continue;
			}
			int n = strlen(fromUser);
			if (fromUser[n-1] == '\n') fromUser[n-1] = 0;
			 
			if (strcmp(cmd, "TIME") == 0) {
				timeSender(&msg);
			} else if (strcmp(cmd, "END") == 0) {
				endSender(&msg, msgid); 
			}
		}
		
}
void addingClientToServer(key_t clientKey){
		Message msg;
		msg.msg_type = LOGIN;
		msg.idSender = clientKey;
		sprintf(msg.msg_text, "%d", privateKey);
		
		if (msgsnd(queue_descriptor, &msg, MSG_SIZE, 0) == -1)
			FAILURE_EXIT("client: LOGIN request failed\n");
		 if (msgrcv(privateID, &msg, MSG_SIZE, 0, 0) == -1)
        FAILURE_EXIT("client: catching LOGIN response failed\n");
		if (sscanf(msg.message_text, "%d", &sessionID) < 1)
			FAILURE_EXIT("client: scanning LOGIN response failed\n");
		if (sessionID < 0)
			FAILURE_EXIT("client: server cannot have more clients\n");
	
		 printf("client: client registered. Session no: %d\n", sessionID);
}

void timeSender(Message *msg){
	msg->msg_type = TIME;
	
	if(msgsnd(queue_descriptor, msg, MSG_SIZE, 0) == -1)
        FAILURE_EXIT("client: Sender failded");
    if(msgrcv(privateID, msg, MSG_SIZE, 0, 0) == -1)
        FAILURE_EXIT("client: Fetching time failded");
    
	printf("%s\n", msg->msg_text);
	
	
}
void endSender(Message *msg, int msgid){
	deleteQueue(msgid, 2);
}
void deleteQueue(int msgid, int var){
		Message *msg;
		
		switch(var){
			case 1:
			{
				msg->msg_type = STOP;
			}
			break;
			case 2;
			{
				msg->msg_type = END;
			}
			break;
		}
	if(msgsnd(queue_descriptor, msg, MSG_SIZE, 0) == -1)
        FAILURE_EXIT("client: Sender failded");
		msgctl(msgid, IPC_RMID, NULL);
}