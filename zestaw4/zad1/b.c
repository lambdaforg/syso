#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
pid_t pid;
char* arg[] = {"./script", NULL};
int run = 1;
void operateSTR(int signum){
			pid_t pidt;
			int status;
	
			printf("CTRL+Z = uruchomienie procesu, CTRL+C - zakonczenie programu\n");
			if ( waitpid(pid, &status, WNOHANG) == -1){
						run = 1;
				    	pid = fork();	
						if(pid == 0){
						execvp(arg[0],arg);
						exit(EXIT_SUCCESS);
						}
			}
			else{
				run = 0;
				kill(pid, SIGKILL);
			}
}
void operateINT(int signum){
	printf("Zakonczenie programu\n");
	kill(pid, SIGINT);
	exit(0);
}
int main(){
			
		struct sigaction act;
		act.sa_handler = operateSTR;
		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;
		sigaction(SIGTSTP, &act, NULL);
		
		signal(SIGINT, operateINT);
		
		int status;
		time_t t;
		pid = fork();	
		if(pid == 0){
		execvp(arg[0],arg);
		exit(EXIT_SUCCESS);
		
		}
		else{
		
		
				while(1){
					sleep(1);
					if(waitpid(pid, &status, WNOHANG) != 0 && run == 1) {
						// Uruchamia proces jesli ktos wylaczyl go z zewnatrz
						sleep(2);
						pid = fork();	
						if(pid == 0){
						execvp(arg[0],arg);
						exit(EXIT_SUCCESS);
					}
					}	
					
				}
		}
	
		return 0;
}