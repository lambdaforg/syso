#define _GNU_SOURCE
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <mqueue.h>
#include <ctype.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>

#include "info.h"

// kolejka Servera i klienta
mqd_t queue_descriptor = -1;  
mqd_t queue_Client = -1;


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
void addingClientToServer(char *ClientKey){
	
	
		
	    Message msg = createMessage(LOGIN,getpid(), "L");
		sprintf(msg.msg_text, "%s", ClientKey);
		int sessionID;
		
		
		if (mq_send(queue_descriptor,  (char*) &msg, MSG_SIZE, 1) == -1){
			 printError("Client: LOGIN request failed\n");	 
		}
		 if (mq_receive(queue_Client, (char*)  &msg, MSG_SIZE, NULL) == -1) 
			printError("Client: catching LOGIN response failed\n");
		if (sscanf(msg.msg_text, "%d", &sessionID) < 1)
			printError("Client: Storing message failed\n");
		if (sessionID < 0){
			printError("Client: Server has a maximum Clients\n");
		}
		if(msg.msg_type == LOGIN){
		 printf("Client: Client registered. Session id: %d\n", sessionID);
		}

}

void timeSender(char* ClientKey){
	Message msg = createMessage(TIME,getpid(),"Time not found");
	if(mq_send(queue_descriptor, (char*) &msg, MSG_SIZE, 1) == -1)
         printError("Client: Sender failed");
    if(mq_receive(queue_Client, (char*) &msg, MSG_SIZE, NULL) == -1)
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
	if(mq_send(queue_descriptor, (char*) &msg, MSG_SIZE, 1) == -1)
         printf("Client: Sender failed");
	//msgctl(queue_Client, IPC_RMID, NULL);
	exit(1);
}
void close_queue(){
		char ClientKey[25];
		sprintf(ClientKey, "/%d",33);
	
		if (mq_close(queue_descriptor) == -1) {
            printf("Client: Error with server queue [CLOSE]!\n");
        } 

        if (mq_close(queue_Client) == -1) {
            printf("Client: Error with Client queue [CLOSE]!\n");
        } 
        if (mq_unlink(ClientKey) == -1) {
            printf("Client: Error with Client queue[UNLINK]!\n");
        } 
	
	
	
	

}
void endSender(){
	deleteQueue(2);
}
void close_singal(int signum){
	deleteQueue(1);
}
int main(){
		
		//usuwa kolejke gdy zakonczy sie program
		atexit(close_queue);
		signal(SIGINT, close_singal);
		
		
		queue_descriptor = mq_open("/pipe", O_WRONLY);
		if(queue_descriptor == -1){
			printError("Error with queue");
		}
		
		struct mq_attr attr;
		attr.mq_maxmsg = 9;
		attr.mq_msgsize = MSG_SIZE;
		
		//Tworzenie kolejki klienta
		char ClientKey[25];
		sprintf(ClientKey, "/pipeClient%d",getpid());
		//ClientKey[7] = "\0";
		printf("%s", ClientKey);
		queue_Client = mq_open(ClientKey, O_RDONLY | O_CREAT | O_EXCL, 0666, &attr); 
		if(queue_Client == -1){
				printf("Something goes bad with queue_Client");
		}
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
