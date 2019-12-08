#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int typeMethod;
pid_t childID = -1;
int countSignalFromParent = 0;

void printSignalInfo(){
	printf("Child: Received from parent %d singals.: \n",countSignalFromParent + 1);
}
void shutdownProgram(){
	printf("Shutdown child. \n");
	printSignalInfo();
	raise(SIGKILL);
	
}
void sendSignal(int signum, siginfo_t *info, void *context){
				sigset_t newmask;
				sigset_t oldmask;
				sigemptyset(&newmask);
				sigfillset(&newmask);
				if(sigprocmask(SIG_SETMASK, &newmask, &oldmask) < 0){
					perror("NIe udalo sie zablokowac sygnalu");
				}
				
		sleep(3);
		printf("Sending to parent signal \n");
		countSignalFromParent++;
		if(typeMethod != 3){
			kill(getppid(), SIGUSR1);
		}
		else{
			kill(getppid(), SIGRTMIN+3);
		}
		

}
int main(int argc, char *argv[]){
		typeMethod = atoi(argv[1]);
		sigset_t newmask;
        sigset_t oldmask;
		sigfillset(&newmask);
		sigdelset(&newmask, SIGUSR1);
	    sigdelset(&newmask, SIGUSR2);
		sigdelset(&newmask, SIGRTMIN+3);
		sigdelset(&newmask, SIGRTMIN);
            if(sigprocmask(SIG_SETMASK, &newmask, &oldmask) < 0){
                perror("NIe udalo sie zablokowac sygnalu");
			}
			
		signal(SIGUSR2, shutdownProgram);
			
		struct sigaction act;
		sigemptyset(&act.sa_mask);
		act.sa_flags = SA_SIGINFO;	
		act.sa_sigaction = sendSignal;
		sigdelset(&act.sa_mask, SIGUSR1);
		
		printf("Child start \n");

		sigaction(SIGUSR1, &act, NULL);
		act.sa_sigaction = sendSignal;
		sigdelset(&act.sa_mask, SIGRTMIN+3);
		
		sigaction(SIGRTMIN+3, &act, NULL);
		
		act.sa_sigaction = shutdownProgram;
		sigdelset(&act.sa_mask, SIGRTMIN);
		
		sigaction(SIGRTMIN, &act, NULL);

		while(1){
			pause();
		}
		
	
		
	
}