#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <time.h>

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h> 

void generate(char *file, int rows, int lengthByte){
	printf("generate %d", lengthByte);
	if(rows !=0 && lengthByte != 0)
	{
			FILE *fileG = fopen(file,"w+");
			int lowLimit = 48;
			int upLimit = 57;
			char c;
			if(fileG)
			{
				for(int i =0;i<rows;i++)
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
void sortLib(char *file, int rows, int lengthByte){
	printf("sort Lib");
	FILE *fileG = fopen(file,"r+");
	char tempValue[1]; 
	fpos_t currentRow; 
	// DOROBIĆ IFA ŻE JESLI NIE PODA DOBREGO SIZA TO SZUKA KOncA LINI I NASTEPNA LINIE DAJE!!			
	for(int e = 0; e < rows ; e++)
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
fpos_t findNextRow(FILE *file, int row){
	/**niby działa **/
	fseek (file , 0, SEEK_SET);
	char arr[1];
	fpos_t currentRow;
	int i = 0;
	while(1){
		fread( arr, sizeof(char), 1, file );
		if(arr[0] == '\n' && i==row)
		{
			break;	
		}else if(arr[0] == '\n'){
			i++;
		}
	}
	fgetpos(file, &currentRow);
	return currentRow;
}
void copyLib(char *file,char *fileTo,int record, int lengthByte){
	/** Kopiowane do pliku B, uwzglednione z tym że mozna podać mniej bytów i tak skopiuje dobrze i zrobi nowa linie. **/
	/** problem może być jak ktoś przekroczy byte. **/
	/** ZROBIĆ POZNIEJ CHECKA NA BYTE > ROWBYTES. **/
	
	printf("copy lib");
	char *arr = malloc(sizeof(char) * lengthByte);
	char temp[1];
	
	FILE *oldFile = fopen(file,"r");
	FILE *newFile = fopen(fileTo,"w+");
	
	fpos_t currentRow;
	fseek (oldFile , 0, SEEK_SET); 
	fgetpos(oldFile, &currentRow);
	
	for(int i =0; i< record; i++){
		fsetpos(oldFile, &currentRow);
		fread( arr, sizeof(char), lengthByte, oldFile );
		fwrite (arr , sizeof(char), lengthByte, newFile);	
			fpos_t currentRow2;
			fgetpos(oldFile, &currentRow2);
			fread( temp, sizeof(char), 1, oldFile );
		if(temp[0] != '\n'){
			// jeśli nie napotka końca linii to próbuje tworzyć nową linie.
			currentRow = findNextRow(oldFile, i);
			temp[0] = '\n';
			fwrite (temp , sizeof(char), 1, newFile);
		}
		else{
			//tworzy nową linie.
			fwrite (temp , sizeof(char), 1, newFile);	
			fgetpos(oldFile, &currentRow);
		}
	}
	fclose(oldFile);
	fclose(newFile);
	free(arr);
}
void sortSys(char *file, int rows, int lengthByte){
	 printf("sort sys");
     int fileG = open(file, O_RDWR);
	 char tempValue[1]; 
	 int ofset = 0;
	for(int e = 0; e < rows ; e++)
	{
		ofset = e*(lengthByte+2);
		lseek(fileG, ofset, SEEK_SET);
		char x;
		int i = 0;
		for(int j = lengthByte - 2; j >= 0; j--)
	  {
		//setPosition(fileG, j, currentRow);
		//fread( tempValue, sizeof(char), 1, fileG );
			lseek(fileG, ofset+j, SEEK_SET );
			read(fileG, tempValue, 1);
		x = tempValue[0];
		i = j + 1;
			lseek(fileG, ofset+i, SEEK_SET );
			read(fileG, tempValue, 1);
		//setPosition(fileG, i, currentRow);
		//fread( tempValue, sizeof(char), 1, fileG );  	
			//read(file, tempValue, 1);
		while((i < lengthByte) && (x > tempValue[0]))
		{
		 //setPosition(fileG, i-1, currentRow);
		 // fwrite (tempValue , sizeof(char), 1, fileG);	
				lseek(fileG, ofset+i-1, SEEK_SET );
				write(fileG, tempValue, 1);
		  i++;
		 // setPosition(fileG, i, currentRow);
		  //fread( tempValue, sizeof(char), 1, fileG );  
				lseek(fileG, ofset+i, SEEK_SET );
				read(fileG, tempValue, 1);
		}
		tempValue[0] = x;
       // setPosition(fileG, i-1, currentRow);		
		//fwrite (tempValue , sizeof(char), 1, fileG);	
				lseek(fileG, ofset+i-1, SEEK_SET );
				write(fileG, tempValue, 1);
			
	}
		  //setPosition(fileG, lengthByte+2, currentRow);
}
	
	
	
	
	close(fileG);
}
void copySys(char *file,char *fileTo,int rows, int lengthByte){
	printf("copy sys");
	
    int source = open(file, O_RDONLY);
    int newFile = open(fileTo, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR); // create ifne, wr only, trunc to 0, args with OCR
    char *tmp = malloc(lengthByte * sizeof(char));
  
  
     for (int i = 0; i < rows; i++){
		 //**pozniej dodać sprawdzić czy to kopiuje wszystko itp**/
		 
        read(source, tmp,  (lengthByte + 1) * sizeof(char));
        write(newFile, tmp,  (lengthByte + 1) * sizeof(char));
        
    }
  
	close(source);
    close(newFile);
    free(tmp);
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
