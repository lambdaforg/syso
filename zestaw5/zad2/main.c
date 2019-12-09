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

// kolejka servera i klienta
mqd_t queue_descriptor = -1;  
mqd_t queue_client = -1;


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
void addingClientToServer(char *clientKey){
	
	
		
	    Message msg = createMessage(LOGIN,getpid(), "L");
		sprintf(msg.msg_text, "%s", clientKey);
		int sessionID;
		
		
		if (mq_send(queue_descriptor,  (char*) &msg, MSG_SIZE, 1) == -1){
			 printError("client: LOGIN request failed\n");	 
		}
		 if (mq_receive(queue_client, (char*)  &msg, MSG_SIZE, NULL) == -1) 
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

void timeSender(char* clientKey){
	Message msg = createMessage(TIME,getpid(),"Time not found");
	if(mq_send(queue_descriptor, (char*) &msg, MSG_SIZE, 1) == -1)
         printError("client: Sender failded");
    if(mq_receive(queue_client, (char*) &msg, MSG_SIZE, NULL) == -1)
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
	if(mq_send(queue_descriptor, (char*) &msg, MSG_SIZE, 1) == -1)
         printf("client: Sender failded");
	//msgctl(queue_client, IPC_RMID, NULL);
	exit(1);
}
void close_queue(){
		char clientKey[25];
		sprintf(clientKey, "/%d",33);
	
		if (mq_close(queue_descriptor) == -1) {
            printf("client: there was some error closing servers's queue!\n");
        } 

        if (mq_close(queue_client) == -1) {
            printf("client: there was some error closing client's queue!\n");
        } 
        if (mq_unlink(clientKey) == -1) {
            printf("client: there was some error deleting client's queue!\n");
        } 
	
	
	
	

}
void endSender(){
	deleteQueue(2);
}
void close_singal(int signum){
	deleteQueue(1);
}
void gen_random(char *s, const int len) {
    static const char alphanum[] =     "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
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
		char clientKey[25];
		sprintf(clientKey, "/pipeClient%d",getpid());
		//clientKey[7] = "\0";
		printf("%s", clientKey);
		queue_client = mq_open(clientKey, O_RDONLY | O_CREAT | O_EXCL, 0666, &attr); 
		if(queue_client != -1){
				printf("good");
		}
		else{
				printf("LOL");
		}
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
