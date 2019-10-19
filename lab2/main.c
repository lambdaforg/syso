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
// zrobic cos fajnego że jęśli 1 to do sys jeśli 2 to do lib auu
void setPositionT(FILE *file, int i, int lenghtByte,char method){
            //nextRow = i * (lenghtbye +2)
        // fsetpos(file, &currentRow);

         fseek (file , i*(lenghtByte+2), SEEK_SET );


}
void setPositionC(int fileG, int i, int lenghtByte,char method){
            //nextRow = i * (lenghtbye +2)
        // fsetpos(file, &currentRow);

         lseek(fileG, i*(lenghtByte+2), SEEK_SET );

}
int lenghtFile(FILE *file)
{
    fseek (file, 0, SEEK_SET );
    char array[1];
    array[0] = ' ';
    int result = 0;
    while(array[0] != '\n')
    {

        fread( array, sizeof(char), 1, file );
        result++;
    }
    fseek (file, 0, SEEK_SET );
    return result;

}
/** ZMIENIĆ POZNIEJ NA COS LADNIEJSZEGO **/
void initArray(char *t, int size)
{
    for(int i = 0;i<size;i++)
    {
        t[i] = ' ';
    }
}
/** OGOLNie DziALA TO TAK ŻE NAWET JAK POdAMY NP 3 2 A MAMY 3 3 TO I TAK DOBRZE POSORTUJE -> WYBRaLEM OPCJE ŻE POSORTUJE TOW TAKI SPOSOB ZE SORTUJE WYBRANE A RESZTA NIE OBCHODZI ALE JEST **/
/** czyli mozna powiedziec ze jest jakies zabezpiczenie jak przekroczy ktos lengthbyte albo wpisze za malo, na rowy to pierdole **/
void sortLib(char *file, int rows, int lengthByte){
        printf("sort Lib");
        FILE *fileG = fopen(file,"r+");
        int lengthByteT = lengthByte;
        if(lenghtFile(fileG) != lengthByte+1){
             lengthByteT = lenghtFile(fileG) - 1;
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
/** Niby działa jak tamto **/
void sortSys(char *file, int rows, int lengthByte){
	 printf("sort sys");
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
/** tu sie narazie nic nie poprawialem, mysle ze nigdy nie bede tutaj juz poprawial ;)**/
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
