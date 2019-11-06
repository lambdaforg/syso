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

const int maxParametrs = 256;
const int maxLines = 256;
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
			 size_t len = 0;
			 ssize_t line_size; 
			 line_size = getline(&line_buf, &len, file);
			 char *param[256];
			 while (line_size >= 0)
			{
				if(readed_lines >= maxLines)
				{
					printf("Too much lines to read.");
					showEntity(param, maxParametrs);
					break;
				}
			   init(param,maxParametrs);
			   int index = 0 ;
			   char * pch = strtok(line_buf, " \n\t\r");
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
			pid_t pid = fork();
			if(pid == 0) {
				printf("------------------------------------------------------------\n");
				printf("--------------------------PID: %d-------------------------\n", (int)getpid());
			execvp(param[0], param);
			exit(1);
			}		
			int status;
			wait(&status);
			if (status) {
				printf( "Error, couldn't open your operations! Your last trace is:\n");
				showEntity(param, maxParametrs);
				printf("\n");
			}
			line_size = getline(&line_buf, &len, file);
				readed_lines++;
			}
			
			
			  
			  free(line_buf);
			  line_buf = NULL;		
			 
			  fclose(file);
		
	}else{
		printf("Error, not enough arguments for program.");
		exit(0);
	}	
	return 0;
}

 
