#define _XOPEN_SOURCE 700
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <linux/limits.h>
//NFTW
#include <ftw.h>

#include <sys/types.h>
#include <unistd.h>

// ./test sciezka > data
const char format2[] = "%d-%m-%Y %H:%M";
const char format[] = "%Y-%m-%d %H:%M:%S";
const char *nftwoperant;
time_t nftwDate;
void getFileInfo(const char *path, const struct stat *file){

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
///DRUGA FUNKCJA W TAMTYM PLIKU
int display_info(const char *fpath, const struct stat *fileStat, int tflag, struct FTW *ftwbuf){

		if(tflag != FTW_F ){
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

			return 0;  // continue jak return 0

}


int main(int argc,char* argv[])
{
	 // PROBLEM JEST TAKI ZE DATA MODYFIKACJI PLIKU SIE ROZNI TRZEBA TO OGARNAC JAKOS ALE TAK TO DZIALA
	//char string[] = "a234"; // check for integer if == 0 than smth...
	struct tm *timeptr = malloc(sizeof(struct tm));
	if(argc > 3)
	{
		 //ROZWIAZANIE PROBLEMU DATY W 2 PLIKU
		char *path = argv[1];
    char *sign = argv[2];
    char *userDate = argv[3];
			strptime(argv[3], format, timeptr);
		  time_t date = mktime(timeptr);
	    printf("%s", ctime(&date));
    nftwoperant = sign;
		nftwDate = date;
 	nftw(realpath(path, NULL), display_info, 10, FTW_PHYS);
		

	}else{
		printf("Error, not enough arguments to program.");
		exit(0);
	}

	return 0;
	free(timeptr);
}
