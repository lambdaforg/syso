#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int isPaused;
pid_t pid;
void operateSTR(int signum){
	switch(isPaused){
		case 0:
		{
			printf("CTRL+Z = zakonczenie potomka, CTRL+C - zakonczenie programu\n");
			isPaused = 1;
			kill(pid, SIGINT);
		}
		break;
		case 1: 
		{
			//kill(getpid(), SIGALRM);
			isPaused = 0;
		}
		break;
	}
	
}
void operateINT(int signum){
	printf("Zakonczenie programu\n");
	kill(pid, SIGINT);
	exit(0);
}

int main(){
	
		isPaused = 0;		
		struct sigaction act;
		act.sa_handler = operateSTR;
		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;
		sigaction(SIGTSTP, &act, NULL);
		char* arg[] = {"./script", NULL};
		int status;
		
		pid = fork();	
		if(pid == 0){
		execvp(arg[0],arg);
		exit(EXIT_SUCCESS);
		}
		
		while(1){
			sleep(1);
			signal(SIGINT, operateINT);	
			if(waitpid(pid, &status, WNOHANG) != 0) {
				sleep(2);
				pid = fork();	
				if(pid == 0){
				execvp(arg[0],arg);
				exit(EXIT_SUCCESS);
				}
			}	
			
		
		}
	
		return 0;
}