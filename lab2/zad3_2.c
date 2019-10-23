
#define _XOPEN_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <limits.h>

#include <unistd.h>
// format daty którą podajemy w argumencie
//const char format[] = "%Y-%m-%d %H:%M:%S";
const char format[] = "%d.%m.%Y %H:%M:%S";

void getFileInfo(char *path, struct stat *file){

		printf("File path \t%s\n", path);
		printf("File size: \t%lld\n",  (long long) file->st_size);
		printf("Permission: \t");
		printf( (S_ISDIR(file->st_mode)) ? "d" : "-");
		printf( (file->st_mode & S_IRUSR) ? "r" : "-");
		printf( (file->st_mode & S_IWUSR) ? "w" : "-");
		printf( (file->st_mode & S_IXUSR) ? "x" : "-");
		printf( (file->st_mode & S_IRGRP) ? "r" : "-");
		printf( (file->st_mode & S_IWGRP) ? "w" : "-");
		printf( (file->st_mode & S_IXGRP) ? "x" : "-");
		printf( (file->st_mode & S_IROTH) ? "r" : "-");
		printf( (file->st_mode & S_IWOTH) ? "w" : "-");
		printf( (file->st_mode & S_IXOTH) ? "x" : "-");
		printf("\n\n");
		printf("Data of last modification \t%s\n", ctime(&(file->st_mtime)));
		

}
void scanDIR(char *path, char sign, time_t date){
	struct dirent * file;
    	DIR * folder =  opendir(path);
	struct stat buf;
	char newPath[PATH_MAX];
	if(folder == NULL){
		printf("ERROR");
	}
		
		while((file = readdir(folder)) != NULL){
			  
			  strcpy(newPath, path);
			  strcat(newPath, "/");
			  strcat(newPath, file->d_name);
				
				if(lstat(newPath, &buf) == -1)
					{
						continue;	
					}
									
				if(strcmp(file->d_name, "..") == 0 || strcmp(file->d_name, ".") == 0)
				{
					continue;
				}
				
				//rekursja gdy folder
				if (S_ISDIR(buf.st_mode) != 0) {
					scanDIR(newPath, sign, date);
				}
			
				// jeśli plik to pokaż informacje
				if(S_ISREG(buf.st_mode) != 0){
					int operant = (int) sign;
					switch(operant)
					{
						case '=':{
							setenv("TZ", "", 1);
							tzset();
							printf("%s->%s", ctime(&buf.st_mtime),file->d_name);
							printf("%s", ctime(&date));
							printf("%f\n", difftime(buf.st_mtime, date));

							if(difftime(buf.st_mtime, date) == 0){
								getFileInfo(newPath,&buf);
							}
						}
						break;
						case '>':{
							if(difftime(buf.st_mtime, date) > 0){
								getFileInfo(newPath,&buf);
							}
						}
						break;
						case '<':{
							if(difftime(buf.st_mtime, date) < 0)
							{
								getFileInfo(newPath,&buf);
							}
						}
						break;
						default:
							printf("Error with operants");
						break;
						
					}
					
					
				}
				  
		}
			
		
	closedir(folder);
	
}

int main(int argc,char* argv[]) 
{
	//char string[] = "a234"; // check for integer if == 0 than smth...
	srand((unsigned int)time(NULL));
	struct tm *timeptr;
	if(argc > 3)
	{
		//odczyt daty, konwersja z tablicy char na strukture tm
		timeptr = malloc(sizeof(struct tm));
		memset(timeptr, 0, sizeof(struct tm));
		strptime(argv[3], format, timeptr);
			//ustawienie time zone żeby mktime nie dodawał 1h do daty
			setenv("TZ", "", 1);
			tzset();
		 	time_t date = mktime(timeptr); 
		printf("%s", ctime(&date));
		// scieżka bezwględna do folderu (argumentu)
		char buf[PATH_MAX]; 
   		realpath(argv[1], buf);
		//wywołanie funkcji
		scanDIR(buf,argv[2][0], date);
		
	}else{
		printf("Error, not enough arguments to program.");
		exit(0);
	}	
	return 0;
}

 
