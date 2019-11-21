#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

int isPaused;

void operateSTR(int signum){
	switch(isPaused){
		case 0:
		{
			printf("Oczekuje na CTRL+Z - kontynuacja albo CTRL+C - zakonczenie programu\n");
			isPaused = 1;
		}
		break;
		case 1: 
		{
			printf("Kontynuacja programu\n");
			//kill(getpid(), SIGALRM);
			isPaused = 0;
		}
		break;
	}
	
}

void operateINT(int signum){
	printf("Zakonczenie programu\n");
	exit(0);
}

int main(){
	
		isPaused = 0;		
		struct sigaction act;
		act.sa_handler = operateSTR;
		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;
		sigaction(SIGTSTP, &act, NULL);
		
			time_t rawtime;
			struct tm * tm;
	
		
		while(1){
			sleep(1);
			signal(SIGINT, operateINT);	
			if(isPaused == 0){
			time ( &rawtime );
			tm = localtime ( &rawtime );
			
			printf("%d:%d:%d\n", tm->tm_hour, tm->tm_min, tm->tm_sec);
			}
		
		}
	
		return 0;
}