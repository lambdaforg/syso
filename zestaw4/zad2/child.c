#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

int maxArguments;
int typeMethod;
pid_t childID = -1;
int countSignalFromParent = 0;

void sendSignal(){
		printf("Sending to parent signal \n");
		countSignalFromParent++;
		kill(getppid, SIGUSR1);
}
void shutdownProgram(){
	printf("Shutdown child. \n");
	printSignalInfo;
	exit(0);
	
}
void printSignalInfo(){
	printf("Received from parent %d singals.: \n"countSignalFromParent);
}
int main(int argc, char *argv[]){
	
		struct sigaction act;
		act.sa_handler = sendSignal;
		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;
		
		sigaction(SIGUSR1, &act, NULL);
		
		signal(SIGINT, shutdownProgram);
		

		while(1){
			sleep(3);

		}
	
		return 0;
}