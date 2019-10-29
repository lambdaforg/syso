#define _XOPEN_SOURCE 700
#define _DEFAULT_SOURCE
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
#include <time.h>
#include <ftw.h>

// format daty którą podajemy w argumencie
const char format[] = "%d.%m.%Y %H:%M:%S";
//zmienne globalne dla funkcji display_info
const char *nftwoperant;
time_t nftwDate;
pid_t child_pid = 1;
void getFileInfo(const char *path, const struct stat *file){

		printf("Proces rodzica: Proces rodzica ma pid:%d\n", (int)getpid());
		printf("\nFile path\t%s\n", path);
		printf("File size:\t%ld\n", file->st_size);
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
		printf("\n");
		printf("Data of last modification \t%s\n", ctime(&(file->st_mtime)));
		


}
int display_info(const char *fpath, const struct stat *fileStat, int tflag, struct FTW *ftwbuf){

		if(tflag != FTW_F ){
			child_pid = vfork();
			
			return 0;
		}

    time_t date = fileStat->st_mtime;
    char operant = nftwoperant[0];
		switch(operant)
		{
			case '=':
				if(difftime(date, nftwDate) == 0){
					getFileInfo(fpath,fileStat);
					return 0;
				}
			break;
			case '>':
				if(difftime(date, nftwDate) > 0){
					getFileInfo(fpath,fileStat);
					return 0;
				}
			break;
			case '<':
				if(difftime(date, nftwDate) < 0)
				{
					getFileInfo(fpath,fileStat);
					return 0;
				}
			break;
			default:
					return 0;
			break;

		}
		if(child_pid != 0){
					//scanDIR(newPath, sign, date);
						return 0;
						exit(0);			
		 }

			return 0;  // continue jak return 0

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
				// pomiń takie pliki		
				if(strcmp(file->d_name, "..") == 0 || strcmp(file->d_name, ".") == 0)
				{
					continue;
				}
				
				//rekursja gdy folder
				if (S_ISDIR(buf.st_mode) != 0) {
					pid_t child_pid = vfork();
					if(child_pid == 0){
					scanDIR(newPath, sign, date);
						exit(0);			
					}
				}
			
				// jeśli plik to pokaż informacje
				if(S_ISREG(buf.st_mode) != 0){
					int operant = (int) sign;
					switch(operant)
					{
						case '=':{
							if(difftime(buf.st_mtime, date) ==  0){
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
							printf("Error with operants, incorrect operants.");
						break;
						
					}
					
					
				}
				  
		}
	
	closedir(folder);
	
}

int main(int argc,char* argv[]) 
{
	struct tm *timeptr;
	if(argc > 3)
	{
		//odczyt daty, konwersja z tablicy char na strukture tm
		timeptr = malloc(sizeof(struct tm));
		memset(timeptr, 0, sizeof(struct tm));
		strptime(argv[3], format, timeptr);
		//Dzięki temu zabiegowi poniżej nie dodaje 1 godziny do daty ;)
		timeptr->tm_isdst = -1;
		// konwersja na time_t z struktury tm
		time_t date = timelocal (timeptr); 
		// scieżka bezwględna do folderu (argumentu)
		char buf[PATH_MAX]; 
   		realpath(argv[1], buf);
		//wywołanie funkcji dla 1 sposobu implementacji
		scanDIR(buf,argv[2][0], date);
		
		
		printf("\n ---NFTW--- \n");
		//dane potrzebne do drugiej implementacji
		nftwoperant = argv[2];
		nftwDate = date;
		//wywolanie funkcji dla 2 implementacji
		nftw(buf, display_info, 10, FTW_PHYS);
		//exit(0);
		
	}else{
		printf("Error, not enough arguments for program.");
		exit(0);
	}	
	return 0;
}

 
