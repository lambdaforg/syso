//#define _XOPEN_SOURCE 700
//#define _DEFAULT_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include <string.h> 
 #include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <memory.h>
#include <stdlib.h>
#include <sys/types.h>

#define maxParametrs 256
const int maxLines = 256;
const int maxCommandPerLine = 256;


struct Command{
	char *param[maxParametrs];
	char *std;
};



void init(char **arr, int size){
		for(int i = 0 ; i < size ; i++)
		{
			arr[i] = NULL;
		}
	
}
void showEntity(char **arr, int size){
		if(arr[0] == NULL)
		{
			printf("White space");
		}
	
		 for (int i = 0; i < 256; i++) {
				 if(arr[i] != NULL){
						printf("%s ", arr[i]);
				 }
		 }	
}
int getParametrs(char line_buf[], struct Command *commands, int i){
	
	int status = 1;
	char *pointToParam = line_buf;
	init(commands[i].param,maxParametrs);
	char * pch = strtok(pointToParam, " ");
	int e = 0;
	  while (pch != NULL) 
				{ 
				if(e < maxParametrs){
						commands[i].param[e] = pch;
					//param[index] = pch;
				}
						else{
							printf("Too much arguments to maintance.");
							showEntity(commands[i].param, maxParametrs);
							return 0;
							//exit(0);
						}
						e++;
						pch = strtok(NULL, " ");
		
				} 	
				

				
	return status;			
}
struct Command * getCommand(char param[], int *i){
	
		 char *str;
		 char *pointToParam = param;
		 int index = 0;
		 struct Command *commands = malloc(sizeof (struct Command));
		 str = strtok( pointToParam, "|" );
		while( str != NULL )
		{
			if(index > 0){
				commands =  realloc(commands,(index +1) *sizeof (struct Command));
			}
			commands[index].std = str;
			str = strtok( NULL, "|\n" );
			index++;

		}
		for(int i =0; i < index; i++){
			if(getParametrs(commands[i].std, commands, i) == 0){
					printf("\n !! ERRORS WITH YOURS ARGUMENTS !! \n");
					exit(-1);
			}
		}
		*i = index;
	return commands;
}
int launch_command(struct Command *commands, int index){
	
	
		int pipes[index][2];
		int i;
	
		for(i = 0; i < index; i++){
			
			
			
			if(i < index){
			if(pipe(pipes[i]) == -1) {
            printf("Error on pipe.\n");
            exit(EXIT_FAILURE);
			}
			}
		
			pid_t nChild = fork();
			if(nChild == 0){
					if(i == 0){
						
						 dup2(pipes[i][1], STDOUT_FILENO);
						   
						  close(pipes[i][0]);
						  close(pipes[i][1]);
						
						execvp(commands[i].param[0], commands[i].param);
					
						_exit(1);
					}
					else if(i == index - 1){
						//close(pipes[i-1][0]);
						
						dup2(pipes[i-1][0],STDIN_FILENO);
						  close(pipes[i-1][0]);
						  close(pipes[i-1][1]);
						
						execvp(commands[i].param[0], commands[i].param);

						_exit(1);
					}
					else{
							//close(pipes[i-1][1]);
							dup2(pipes[i-1][0], STDIN_FILENO);
							
							dup2(pipes[i][1], STDOUT_FILENO);
					
							 close(pipes[i][0]);
							 close(pipes[i][1]);
					
					
							 close(pipes[i-1][0]);
							 close(pipes[i-1][1]);
							
							execvp(commands[i].param[0], commands[i].param);
							_exit(1);
					
					}
					
		
			}
			else{
			
			if (i > 1) {
            close(pipes[i - 1][0]);
            close(pipes[i - 1][1]);
			}
			}
		
		
		}
		close(pipes[i - 1][0]);
		close(pipes[i - 1][1]);
		
}
int main(int argc,char* argv[]) 
{
	
	if(argc > 1)
	{	
			 int readed_lines = 0;
			 FILE* file = fopen(argv[1], "r");
		
			 if(file == NULL){
				 printf("Cannot open the file.");
				 exit(0);
			 }
			 ssize_t nread;
			 char line_buf[512];
			 struct Command *commands;
			 size_t len = 0;
			 ssize_t line_size = 1 ;
			  char *line = NULL;
			// line_size = getline(&line_buf, &len, file);
		
			  while ((nread = getline(&line, &len, file)) != -1) {
				if(readed_lines >= maxLines)
				{
					printf("Too much lines to read.");
					break;
				}
				int index;
				commands = getCommand(line, &index);
			 	launch_command(commands, index);
				wait(NULL);
				
				
			}
			
			
			  
			 // free(line_buf);
			 // free(commands);
			//  commands = NULL;
			 //// line_buf = NULL;		
			 
			  fclose(file);
		
	}else{
		printf("Error, not enough arguments for program.");
		exit(0);
	}	
	return 0;
}

 
