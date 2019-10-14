#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <time.h>

//TO do generating by operating functions //

// generating file by normally functions avalaible in C 
char randBorder(int from, int to)
{
	return (char)(from + (rand()%(to-from+1)));
}
void generate(char *file, int record, int lengthByte){
	printf("generate");
	if(record !=0 && lengthByte != 0)
	{
			FILE *fileG = fopen(file,"w+");
			int lowLimit = 48;
			int upLimit = 57;
			char text[1];
			if(fileG)
			{
				for(int i =0;i<record;i++)
				{
					for(int j =0;j<lengthByte;j++)
					{
					text[0] = randBorder(lowLimit,upLimit);
					fputs(text,fileG);
			
					}
					fputs("\n",fileG);
				}
			fclose(fileG);
			}
	}	
}
void sortLib(char *file, int record, int lengthByte){
	printf("sort Lib");
	FILE *fileG = fopen(file,"r+");
	char tempValue[2];
	fpos_t *pastPos; 
	fpos_t *nextPos;
	fread(tempValue, 1, 1, fileG);
	//int fsetpos(fileG, fpos_t* pos);
	//int fgetpos(fileG, position);
	printf("%s", tempValue);
	fgetpos(fileG, &pastPos);
	fread(tempValue, 1, 1, fileG);
	printf("%s", tempValue);
	
	fclose(fileG);
}
void copyLib(char *file,char *fileTo,int record, int lengthByte){
	printf("copy lib");
}
void sortSys(char *file, int record, int lengthByte){
	printf("sort sys");
}
void copySys(char *file,char *fileTo,int record, int lengthByte){
	printf("copy sys");
}



int main(int argc,char* argv[]) 
{
	char string[] = "a234"; // check for integer if == 0 than smth...
	srand((unsigned int)time(NULL));
	if(argc >= 5)
	{
		if(strcmp(argv[1], "generate") == 0 && argc == 5)
		{
		  generate(argv[2],atoi(argv[3]),atoi(argv[4]));
		}else if(strcmp(argv[1], "sort") == 0 && argc == 6){
			if(strcmp(argv[5], "sys") == 0){
				sortSys(argv[2],atoi(argv[3]),atoi(argv[4]));
			}
			else if(strcmp(argv[5], "lib") == 0){
				sortLib(argv[2],atoi(argv[3]),atoi(argv[4]));
			}
			
		
		}else if(strcmp(argv[1], "copy") == 0 && argc == 7){
				if(strcmp(argv[6], "sys") == 0){
				copySys(argv[2],argv[3],atoi(argv[4]),atoi(argv[5]));
			}
			else if(strcmp(argv[6], "lib") == 0){
				copyLib(argv[2],argv[3],atoi(argv[4]),atoi(argv[5]));
			}
		}
		
		
	}else{
		printf("Error, not enough arguments to program.");
		exit(0);
	}	
	return 0;
}
