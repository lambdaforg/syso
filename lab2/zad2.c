#define _XOPEN_SOURCE 700
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
// ./test sciezka > data 
const char format[] = "%d.%m.%Y %H:%M:%S";

void getFileInfo(char *path, struct stat *file){

		printf("File path\t%s", path);
		printf("File size:\t%lld\n", file->st_size);
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
		printf("Data of last modification /t%lld/n", ctime(&(file->st_mtime)));
		

}
void scanDIR(char *path, char sign, time_t date){
	struct dirent * file;
    DIR * folder;
	struct stat buf;
	char newPath[PATH_MAX];
	if((folder = opendir(path))){
		
		while((file = readdir(folder)) != NULL){
			  
			  strcpy(newPath, path);
			  strcat(newPath, "/");
			  strcat(newPath, file->d_name);
				
				 lstat(file->d_name, &buf);
				if(strcmp(file->d_name, "..") == 0 || strcmp(file->d_name, ".") == 0)
				{
					continue;
				}
				
				//recursion if directory
				if (S_ISDIR(buf.st_mode)) {
					scanDIR(newPath, sign, date);
				}
			
				// if file
				if(S_ISREG(buf.st_mode)){
					int operant = (int) sign;
					switch(operant)
					{
						case '=':
							if(difftime(buf.st_mtime, date) == 0){
								getFileInfo(newPath,&buf);
							}
						break;
						case '>':
							if(difftime(buf.st_mtime, date) > 0){
								getFileInfo(newPath,&buf);
							}
						break;
						case '<':
							if(difftime(buf.st_mtime, date) < 0)
							{
								getFileInfo(newPath,&buf);
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
	else{
		printf("error");
	}	
	
}

int main(int argc,char* argv[]) 
{
	//char string[] = "a234"; // check for integer if == 0 than smth...
	srand((unsigned int)time(NULL));
	printf("%d",argc);
	struct tm *timeptr;
	if(argc > 3)
	{
		timeptr = malloc(sizeof(struct tm));
		strptime(argv[3], format, timeptr);
		 time_t date = mktime(timeptr); // niby mamy date co robimy?
		
		//zmienic path
		scanDIR(argv[1],argv[2][0], date);
		
	}else{
		printf("Error, not enough arguments to program.");
		exit(0);
	}	
	return 0;
}
