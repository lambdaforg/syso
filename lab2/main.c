#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <time.h>

//TO do generating by operating functions //

// wyjebać to gówno
char randBorder(int from, int to)
{
	return (char)(from + (rand()%(to-from+1)));
}
void generate(char *file, int record, int lengthByte){
	printf("generate %d", lengthByte);
	if(record !=0 && lengthByte != 0)
	{
			FILE *fileG = fopen(file,"w+");
			int lowLimit = 48;
			int upLimit = 57;
			char c;
			if(fileG)
			{
				for(int i =0;i<record;i++)
				{
					for(int j =0;j<lengthByte;j++)
					{
						//dopisać to gówno
					c = "AaBbCcDdEeFfGg0123456789"[rand() % 24];
					fputc(c,fileG);
			
					}
					fputc('\n',fileG);
				}
			fclose(fileG);
			}
	}	
}
void setPosition(FILE *file, int i, fpos_t currentRow){
	 fsetpos(file, &currentRow);
	 fseek (file , i, SEEK_CUR ); 
}
void sortLib(char *file, int record, int lengthByte){
	printf("sort Lib");
	FILE *fileG = fopen(file,"r+");
	char tempValue[1]; 
	fpos_t currentRow; 
	// DOROBIĆ IFA ŻE JESLI NIE PODA DOBREGO SIZA TO SZUKA KOncA LINI I NASTEPNA LINIE DAJE!!			
	for(int e = 0; e < record ; e++)
	{
		if(e == 0){
		  fseek (fileG , 0, SEEK_SET); 
		}
		fgetpos(fileG, &currentRow);
		char x;
		int i = 0;
		for(int j = lengthByte - 2; j >= 0; j--)
	  {
		setPosition(fileG, j, currentRow);
		fread( tempValue, sizeof(char), 1, fileG );  
		x = tempValue[0];
		i = j + 1;
		setPosition(fileG, i, currentRow);
		fread( tempValue, sizeof(char), 1, fileG );  		
		while((i < lengthByte) && (x > tempValue[0]))
		{
		 setPosition(fileG, i-1, currentRow);		
		  fwrite (tempValue , sizeof(char), 1, fileG);	
		  //d[i - 1] = d[i];
		  i++;
		  setPosition(fileG, i, currentRow);
		  fread( tempValue, sizeof(char), 1, fileG );     
		}
		tempValue[0] = x;
        setPosition(fileG, i-1, currentRow);		
		fwrite (tempValue , sizeof(char), 1, fileG);	
			
	}
		  setPosition(fileG, lengthByte+2, currentRow);
}
	fclose(fileG);
}
void copyLib(char *file,char *fileTo,int record, int lengthByte){
	printf("copy lib");
	char *arr = malloc(sizeof(char) * lengthByte);
	for(int i =0; i< record; i++){
		
		//if(fread( tempValue, sizeof(char), 1, fileG ))
	//	{
			
		//}
		//if(fwrite (tempValue , sizeof(char), 1, fileG)){
//
	//	}			
	}
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
