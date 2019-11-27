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

/*char *getParametrs(char *command){
	
	char *param[256]; 
	init(param,maxParametrs);
	char * pch = strtok(line_buf, " \n\t\r");
	int index = 0;
	  while (pch != NULL) 
				{ 
				if(index < maxParametrs)
						param[index] = pch;
						else{
							printf("Too much arguments to maintance.");
							showEntity(param, maxParametrs);
							exit(0);
						}
						index++;
						pch = strtok(NULL, " \n\t\r");
		
				} 	
				
		return param;		
	
}*/
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
				
					//free(pch);
					//free(pointToParam);
				
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
			printf("%s", str);
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
	
	// robi tyle rur ile jest komend tylko 1 mniej chodzi o to ze rury lacza se soba te jebane komendy
	
		int pipes[index - 1][2];
		
		for(int i = 0; i < index; i++){
			
			
			/*if(i > 0){
				
				pipe2[0] = pipe1[0];
				pipe2[1] = pipe1[1];
			}*/
			
			if(i < index - 1){
			if(pipe(pipes[i]) == -1) {
            printf("Error on pipe.\n");
            exit(EXIT_FAILURE);
			}
			}
		
			pid_t nChild = fork();
			if(nChild == 0){
					if(i == 0){
						 dup2(pipes[i][1], STDOUT_FILENO);
						  // zamyka 
						  close(pipes[i][0]);
						  close(pipes[i][1]);
						
						execvp(commands[i].param[0], commands[i].param);
						// 1 syf 
						_exit(1);
					}
					else if(i == index - 1){
						
						dup2(pipes[i-1][0],STDIN_FILENO);
						  close(pipes[i-1][0]);
						  close(pipes[i-1][1]);
						
						execvp(commands[i].param[0], commands[i].param);
						//ma polaczyc ostatniego pipa ze sobA
						_exit(1);
					}
					else{
						// bierze inputa z poprzedniego do swojego
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
			
			if(i > 0){
				close(pipes[i-1][0]);
				close(pipes[i-1][1]);
			}
		
		
		
		}
	

		return 1;
}

//execvp(param[0], param);
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
			 
			 char *line_buf = NULL;
			 struct Command *commands;
			 size_t len = 0;
			 ssize_t line_size; 
			 line_size = getline(&line_buf, &len, file);
		
			 while (line_size >= 0)
			{
				if(readed_lines >= maxLines)
				{
					printf("Too much lines to read.");
					//showEntity(param, maxParametrs);
					break;
				}
			 	
				int index;
				commands = getCommand(line_buf, &index);
				printf("INDEX INDEX %d", index);
				//pid_t pid = fork();
				///if(pid == 0) {
				printf("------------------------------------------------------------\n");
				printf("--------------------------MOTHER PID: %d-------------------------\n", (int)getpid());
				launch_command(commands, index);
				//exit(1);
				//}		
				int status;
				wait(&status);
				if (status) {
				printf( "Error, couldn't open your operations! Your last trace is:\n");
				//showEntity(param, maxParametrs);
				printf("\n");
				}
			line_size = getline(&line_buf, &len, file);
				readed_lines++;
			}
			
			
			  
			  free(line_buf);
			  free(commands);
			  commands = NULL;
			  line_buf = NULL;		
			 
			  fclose(file);
		
	}else{
		printf("Error, not enough arguments for program.");
		exit(0);
	}	
	return 0;
}

 
