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
int countSignalFromParent = 0;

void printSignalInfo(){
	printf("Child: Received from parent %d singals.: \n",countSignalFromParent);
}
void sendSignal(int signum, siginfo_t *info, void *context){
			sigset_t newmask;
            sigset_t oldmask;
            sigemptyset(&newmask);
            if(sigprocmask(SIG_SETMASK, &newmask, &oldmask) < 0){
                perror("NIe udalo sie zablokowac sygnalu");
			}
			else{
			sigdelset(&newmask, SIGUSR1);
			}
		
		printf("Sending to parent signal \n");
		countSignalFromParent++;
		kill(getppid(), SIGUSR1);
		sleep(1);
		
		//waitpid(childID, NULL, 0);
}
void sendSignalSigrt(int signum, siginfo_t *info, void *context){
			sigset_t newmask;
            sigset_t oldmask;
            sigemptyset(&newmask);
            if(sigprocmask(SIG_SETMASK, &newmask, &oldmask) < 0){
                perror("NIe udalo sie zablokowac sygnalu");
			}
			else{
			sigdelset(&newmask, SIGRTMIN+3);
			}
		
		printf("Sending to parent signal \n");
		countSignalFromParent++;
		kill(getppid(), SIGRTMIN+3);
		sleep(1);
}
void shutdownProgram(){
	printf("Shutdown child. \n");
	printSignalInfo();
	raise(SIGKILL);
	//exit(0);
	
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
			
		struct sigaction act;
		sigemptyset(&act.sa_mask);
		act.sa_flags = SA_SIGINFO;	
		act.sa_sigaction = sendSignal;
		sigdelset(&act.sa_mask, SIGUSR1);
		
		printf("Child start \n");

		sigaction(SIGUSR1, &act, NULL);
		act.sa_sigaction = sendSignalSigrt;
		sigdelset(&act.sa_mask, SIGRTMIN+3);
		
		sigaction(SIGRTMIN+3, &act, NULL);
		
		act.sa_sigaction = shutdownProgram;
		sigdelset(&act.sa_mask, SIGRTMIN);
		
		sigaction(SIGRTMIN, &act, NULL);
		signal(SIGUSR2, shutdownProgram);
		
		sleep(10);
		
		while(1){
			sleep(10);
			pause();
		}
		
	
		
	
}