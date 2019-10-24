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
			char c;
			if(fileG)
			{
				for(int i =0;i<rows;i++)
				{
					for(int j =0;j<lengthByte;j++)
					{
					
						c = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpRrSsTtUuWwXxYyZz0123456789"[rand() % 24];
						fputc(c,fileG);
			
					}
					fputc('\n',fileG);
					
				}
			fclose(fileG);
			}
	}	
}
void setPositionT(FILE *file, int i, int lenghtByte,char method){
         fseek (file , i*(lenghtByte+1), SEEK_SET );
}
void setPositionC(int fileG, int i, int lenghtByte,char method){
         lseek(fileG, i*(lenghtByte+1), SEEK_SET );
}
void initArray(char *t, int size)
{
    for(int i = 0;i<size;i++)
    {
        t[i] = ' ';
    }
}
int findRealSizeOfRow(FILE *file)
{
	fseek (file , 0, SEEK_SET);
	char arr[1];
	int i = 0;
	while(1){
		fread( arr, sizeof(char), 1, file );
		if(arr[0] == '\n')
		{
			break;	
		}else if(arr[0] != '\n'){
			i++;
		}
	}
	
	return i;
}
void sortLib(char *file, int rows, int lengthByte){
        printf("sort Lib");
        FILE *fileG = fopen(file,"r+");
        int lengthByteT = lengthByte;
        if(findRealSizeOfRow(fileG) != lengthByte){
             lengthByteT = findRealSizeOfRow(fileG);
        }
        char *tempRecord = malloc(sizeof(char) * lengthByte );
        char *tempRecord2 = malloc(sizeof(char) * lengthByte );
        fseek (fileG , 0, SEEK_SET);
        fpos_t currentRow;
        fgetpos(fileG, &currentRow);
        int i = 0;
        initArray(tempRecord, lengthByte );
        initArray(tempRecord2, lengthByte );


        for(int j = rows - 2; j >= 0; j--)
        {
             setPositionT(fileG, j, lengthByteT,'l');
        fread( tempRecord2, sizeof(char), lengthByte, fileG );
        i = j + 1;
              setPositionT(fileG, i, lengthByteT,'l');
        fread( tempRecord, sizeof(char), lengthByte, fileG );
        while((i < rows) && (strcmp(tempRecord2 , tempRecord) > 0))
        {
              setPositionT(fileG, i-1,lengthByteT, 'l');
              fwrite (tempRecord , sizeof(char), lengthByte, fileG);
          i++;
             setPositionT(fileG, i,lengthByteT, 'l');
             fread( tempRecord, sizeof(char), lengthByte, fileG );
        }
        strncpy(tempRecord, tempRecord2, lengthByte);
             setPositionT(fileG, i-1,lengthByteT, 'l');
             fwrite (tempRecord , sizeof(char), lengthByte, fileG);
        }

        fclose(fileG);
        free(tempRecord);
        free(tempRecord2);
}
void copyLib(char *file,char *fileTo,int record, int lengthByte){

	char *arr = malloc(sizeof(char) * lengthByte);
	char temp[1];
	int createNewLine = 0;
	FILE *oldFile = fopen(file,"r");
	FILE *newFile = fopen(fileTo,"w+");
	int realSize = findRealSizeOfRow(oldFile);

	if(lengthByte > realSize){
		lengthByte = realSize;
	}
	if(lengthByte < realSize){
		createNewLine = 1;
	}

	fseek (oldFile , 0, SEEK_SET); 
	
	for(int i =0; i< record; i++){
		
		if(fread( arr, sizeof(char), lengthByte, oldFile ) != lengthByte)
			break;
		if(fwrite (arr , sizeof(char), lengthByte, newFile)  != lengthByte)
			break;
		
		if(createNewLine == 1){
			fseek(oldFile , realSize-lengthByte + 1, SEEK_CUR );
		}else{
			fseek(oldFile , 1, SEEK_CUR );
		}
		temp[0] = '\n';
		fwrite (temp , sizeof(char), 1 , newFile);
	}
	fclose(oldFile);
	fclose(newFile);
	free(arr);
}
int lenghtFileSys(int file)
{
    lseek(file, 0, SEEK_SET);
    char array[1];
    array[0] = ' ';
    int result = 0;
    while(array[0] != '\n')
    {
        read(file, array, 1);
        result++;
    }
    lseek(file, 0, SEEK_SET);
    return result;

}

void sortSys(char *file, int rows, int lengthByte){
         int fileG = open(file, O_RDWR);
         int lengthByteT = lengthByte;
         if(lenghtFileSys(fileG) != lengthByte+1){
              lengthByteT = lenghtFileSys(fileG) - 1;
         }
         char *tempRecord = malloc(sizeof(char) * lengthByte );
         char *tempRecord2 = malloc(sizeof(char) * lengthByte );
         int i = 0;
         initArray(tempRecord, lengthByte );
         initArray(tempRecord2, lengthByte );


                for(int j = rows - 2; j >= 0; j--)
                {
                     setPositionC(fileG, j, lengthByteT,'s');
                     read(fileG, tempRecord2, lengthByte);
                i = j + 1;
                      setPositionC(fileG, i, lengthByteT,'s');
                      read(fileG, tempRecord, lengthByte);
                while((i < rows) && (strcmp(tempRecord2 , tempRecord) > 0))
                {
                      setPositionC(fileG, i-1,lengthByteT, 's');
                      write(fileG, tempRecord, lengthByte);
                  i++;
                     setPositionC(fileG, i,lengthByteT, 's');
                     read(fileG, tempRecord, lengthByte);
                }
                     strncpy(tempRecord, tempRecord2, lengthByte);
                     setPositionC(fileG, i-1,lengthByteT, 's');
                     write(fileG, tempRecord, lengthByte);
                }

                close(fileG);
                free(tempRecord);
                free(tempRecord2);
}
void copySys(char *file,char *fileTo,int rows, int lengthByte){

	
    int source = open(file, O_RDONLY);
    int newFile = open(fileTo, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR); 
    char *tmp = malloc(lengthByte * sizeof(char));
	char temp[1];
		printf("copy sys %d", lenghtFileSys(source));
	int realSize = lenghtFileSys(source) - 1;
	int createNewLine = 0;
	
	if(lengthByte > realSize){
		lengthByte = realSize;
	}
	if(lengthByte < realSize){
		createNewLine = 1;
	}
  
  
     for (int i = 0; i < rows; i++){
	
        if(read(source, tmp,  (lengthByte) * sizeof(char)) != lengthByte )
			break;
        if(write(newFile, tmp,  (lengthByte) * sizeof(char)) != lengthByte)
			break;
		
		if(createNewLine == 1){
			lseek(source , realSize-lengthByte + 1, SEEK_CUR );
		}else{
			lseek(source , 1, SEEK_CUR );
		}
		temp[0] = '\n';
		write(newFile, temp,  1 * sizeof(char));
        
    }
  
	close(source);
    close(newFile);
    free(tmp);
}



int main(int argc,char* argv[]) 
{
	
	srand((unsigned int)time(NULL));
	if(argc >= 5)
	{	
		if(strcmp(argv[1], "generate") == 0 && argc == 5)
		{
			if(atoi(argv[3]) == 0 || atoi(argv[4]) == 0){
			printf("Incorrect sizes!");
			return 0;
			}
		  generate(argv[2],atoi(argv[3]),atoi(argv[4]));
        }else if(strcmp(argv[1], "sort") == 0 && argc == 6){
			if(atoi(argv[3]) == 0 || atoi(argv[4]) == 0){
			printf("Incorrect sizes!");
			return 0;
			}

			if(strcmp(argv[5], "sys") == 0){
				sortSys(argv[2],atoi(argv[3]),atoi(argv[4]));
			}
			else if(strcmp(argv[5], "lib") == 0){
				sortLib(argv[2],atoi(argv[3]),atoi(argv[4]));
			}
		}else if(strcmp(argv[1], "copy") == 0 && argc == 7){
			if(atoi(argv[4]) == 0 || atoi(argv[5]) == 0){
			printf("Incorrect sizes!");
			return 0;
			}

				if(strcmp(argv[6], "sys") == 0){
				copySys(argv[2],argv[3],atoi(argv[4]),atoi(argv[5]));
			}
			else if(strcmp(argv[6], "lib") == 0){
				copyLib(argv[2],argv[3],atoi(argv[4]),atoi(argv[5]));
			}
		}
		
		
	}else{
		printf("Error, not enough arguments for program.");
		exit(0);
	}	
	return 0;
}
