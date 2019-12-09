#define _GNU_SOURCE
#include <mqueue.h>
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
mqd_t  queue_descriptor = -2;  
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
		
	 if (mq_close(clients_queue[index][1]) == -1) {
            printError("server: error closing %d client queue\n");
      }
		
	clients_queue[index][0] = -1;
	clients_queue[index][1] = -1;

}
void handleLogin(struct Message *msg){
	
			char clientQueueKey[25];
			//klucz klienta
			if (sscanf(msg->msg_text, "%s", clientQueueKey) < 0)
			printError("erorr");
			// kolejka klienta
			printf("NAZWA A A ->>%s\n", clientQueueKey);
			fflush(stdout);
			int clientQueue = mq_open(clientQueueKey, O_WRONLY);
			if(clientQueue == -1){
					printf("test");
			}
			
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

		
		if (mq_send(clientQueue,(char*) &msgo, MSG_SIZE, 1) == -1)
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
		
	    if(mq_send(clients_queue[index][1], (char*) &msgo, MSG_SIZE, 1) == -1)
				printError("server: Error with time sender\n");
}
void handleEnd(struct Message *msg){
	exit(1);
}
void close_queue(){
	for(int i = 0 ; i< counterClient; i++){
	if (mq_close(clients_queue[i][1]) == -1) {
            printf("server: error closing client queue\n");
      }
	}
	if(queue_descriptor != -1){
		if(mq_close(queue_descriptor) == -1) {
            printf("server: error closing server queue\n");
        } 
		
		if (mq_unlink("/pipe") == -1) {
            printf("server: error deleting public queue\n");
        }
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
void close_singal(int signum){
		exit(1);
}
int main(){
		//funkcja wykonywana przy zamknieciu serveruu
		atexit(close_queue);
		signal(SIGINT, close_singal);
		
		struct mq_attr attr;
		attr.mq_maxmsg = 9;
		attr.mq_msgsize = MSG_SIZE;
		
		queue_descriptor = mq_open("/pipe", O_RDONLY | O_CREAT | O_EXCL, 0666, &attr);

		 if(queue_descriptor == -1){
			//printError("Erro with queue_descriptor");
			queue_descriptor = mq_open("/pipe", O_RDONLY, &attr);
			if(mq_close(queue_descriptor) == -1) {
            printf("server: error closing server queue\n");
			} 
		
			if (mq_unlink("/pipe") == -1) {
            printf("server: error deleting public queue\n");
			}
		}
		
		Message msg;
		while(1){
		 if (mq_receive(queue_descriptor, (char*) &msg, MSG_SIZE, NULL) < 0){
					  printError("server: receiving message failed\n");
		 }
			
			handle_request(&msg);
			
		}
		
}
