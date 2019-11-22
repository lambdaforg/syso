#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

int maxArguments;
int typeMethod;
pid_t childID = -1;
int countSignalFromChild = 0;
int countSender = 0;


void firstMethod(){
		if(childID == -1){
			printf("Something gone wrong with child. \n");
			exit(0);
		}
			
		for(int i = 0 ; i < maxArguments; i++){
			countSender++;
			printf("Sending signal to child - SIGUSR1 \n");
			kill(childID, SIGUSR1);
			waitpid(childID, NULL, 0);
		}
			printf("Sending signal to child - SIGUSR2 \n");
			kill(childID, SIGUSR2);
}
void secondMethod(){
		
		
}
void thirdMethod(){
	
}
void awaitingForSignals(){
		
}
void shutdownProgram(){
	printf("Shutdown program with child. \n");
	printSenderInfo();
	printSignalInfo();
	kill(childID, SIGKILL);
	exit(0);
	
}
void handleChildSignal(){
	printSignalInfo();
	countSignalFromChild++;
}
void printSignalInfo(){
	printf("Received from child %d singals.: \n"countSignalFromParent);
}
void printSenderInfo(){
	printf("Send to child %d singals \n", countSender);
}
int main(int argc, char *argv[]){
	
		if(argc  ==  2 ){
			maxArguments = argv[0];
			typeMethod = argv[1];
		}
		else{
			return 0;
		}
	
		
		struct sigaction act;
		act.sa_handler = handleChildSignal;
		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;
		
		
		/**Tworzy proces potomny **/
		sigaction(SIGUSR1, &act, NULL);
		char* arg[] = {"./program", NULL};
		int status;
		
		childID = fork();	
		if(childID == 0){
		execvp(arg[0],arg);
		exit(EXIT_SUCCESS);
		}

		signal(SIGINT, shutdownProgram);
		
		firstMethod();
		
		while(1){
			sleep(1);
			//signal(SIGINT, operateINT);	
		}
	
		return 0;
}