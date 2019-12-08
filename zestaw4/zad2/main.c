#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int maxArguments;
int typeMethod;
pid_t childID = -1;
int countSignalFromChild = 0;
int countSignalFromParent = 0;

void firstMethodSender(){
		if(childID == -1){
			printf("Something gone wrong with child. \n");
			exit(0);
		}
		
		for(int i = 0 ; i < maxArguments; i++){
			sleep(3);
			countSignalFromParent++;
			printf("Parent: Sending signal to child - SIGUSR1 \n");
			kill(childID, SIGUSR1);
		}
			sleep(5);
			printf("Parent: Sending signal to child - SIGUSR2 \n");
		
			kill(childID, SIGUSR2);
}
void secondMethodSender(){
		if(countSignalFromParent < maxArguments)
		{
			//sleep(2);
			printf("Parent: Send signal to child\n");
			countSignalFromParent++;
			kill(childID, SIGUSR1);
		
		}
		else{
			printf("Parent: Send signal SIGUSR2 to child\n");
			kill(childID, SIGUSR2);
		}
}
void thirdMethodSender(){
	if(countSignalFromParent < maxArguments)
	{
		//sleep(2);
		printf("Parent: Send signal to child\n");
		countSignalFromParent++;
		kill(childID,SIGRTMIN+3);
	}
		else{
			printf("Parent: Send signal to child\n");
			kill(childID, SIGRTMIN);
		}
}
void printSignalInfo(){
	printf("Parent: Received from child singal. Total signals %d\n",countSignalFromChild);
}
void printSenderInfo(){
	printf("\nParent sent %d signals SIGUSR1 and 1 SIGUSR2 to child\n", countSignalFromParent);
}
void shutdownProgram(){
	int v = waitpid(childID, NULL, 0);
	if(v != childID){
					printf("Shutdown program with child.\n");
					kill(childID, SIGUSR2);
	}
	else{
                printf("Shutdown program. Child was terminated earlier.\n");
	} 
	printSenderInfo();
	printSignalInfo();
	exit(0);
	
}
void handleChildSignal(int signum, siginfo_t *info, void *context){
	// Standard output
	countSignalFromChild++;
	printSignalInfo();
	
	switch(typeMethod){
	case 2:{
		secondMethodSender();
	}
	break;
	case 3:{
		thirdMethodSender();
	}
	break;
	}
	
}
int main(int argc, char *argv[]){
		sigset_t newmask;
        sigset_t oldmask;
        sigemptyset(&newmask);
        if(sigprocmask(SIG_SETMASK, &newmask, &oldmask) < 0){
                perror("NIe udalo sie zablokowac sygnalu");
		}
		else{	
		sigdelset(&newmask, SIGUSR1);
		}
		signal(SIGINT,shutdownProgram);
		printf("Start program %d\n", argc);
		if(argc  ==  3){
			maxArguments = atoi(argv[1]);
			typeMethod = atoi(argv[2]);
		}
		else{
			printf("Not enough arguments");
			return 0;
		}
		childID = fork();	
		
		if(childID == 0){
			 printf("Starting child \n");
			 char *arg[3];
			 arg[0] = "./program";
			 arg[1] = argv[2];
			 arg[2] = NULL;

			 execvp(arg[0], arg);
		}
		else{
			sleep(1);
			struct sigaction act;
			sigemptyset(&act.sa_mask);
			act.sa_flags = SA_SIGINFO;	
			act.sa_sigaction = handleChildSignal;
			sigdelset(&act.sa_mask, SIGUSR2);
			sigaction(SIGUSR1, &act, NULL);
			
			sigdelset(&act.sa_mask, SIGRTMIN+3);
			sigaction(SIGRTMIN+3, &act, NULL);
			

			
			printf("Child process %d \n", childID);
			
			switch(typeMethod){
				case 1:
				{
					firstMethodSender();
				}
				break;
				case 2:
				{
					secondMethodSender();
				}
				break;
				case 3:
				{
					thirdMethodSender();
				}
				break;
				default:{
					printf("Invalid type, program maintance only from 1 to 3 methods.\n");
					shutdownProgram();
				}
				break;
			}
			
				while(1){
					int v = waitpid(childID, NULL, 0);
				if(v == childID){
					raise(SIGINT);
				}
					
				}
				
	
		}
}