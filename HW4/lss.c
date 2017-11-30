#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <math.h>

struct fileInfo{
	char * name;
	int size;
	char * info;
};

//qsort !(less than 0 if p1< p2), equal to 0 if p1 = p2; !(greater than 0 p1 > p2);
static int compareInfo(const void *p1, const void *p2){
	fileInfo * pp1 = (fileInfo *) p1;
	fileInfo * pp2 = (fileInfo *) p2;
	
	//if pp1 < pp2 then return 1 since we are sorting size in descending order
	if (pp1->size < pp2->size){
		return 1;
	}
	//if pp1 > pp2 then return -1 since we are sorting size in descending order
	else if (pp1->size > pp2-> size){
		return -1;
	}
	//if pp1 == pp2 then we sort by name (alphabetical); negative# if pp1 is less than pp2; positive# if pp1 is greater than pp2
	else if (pp1->size == pp2->size){
		return strcmp(pp1->name, pp2->name);
	}
}

//using the stat struct, check the permission bit and add it to the toprint string
void getPermissionBit(char * toprint, struct stat &stats){
	
	//USER PERMISSION BIT
			strcat(toprint,(stats.st_mode & S_IRUSR) ? "r": "-");
			strcat(toprint,(stats.st_mode & S_IWUSR) ? "w": "-");
			strcat(toprint,(stats.st_mode & S_IXUSR) ? "x": "-");
			
			
			//GROUP PERMISSION BIT
			
			strcat(toprint,(stats.st_mode & S_IRGRP) ? "r": "-");
			strcat(toprint,(stats.st_mode & S_IWGRP) ? "w": "-");
			strcat(toprint,(stats.st_mode & S_IXGRP) ? "x": "-");
			//OTHER PERMISSION BIT
			strcat(toprint,(stats.st_mode & S_IROTH) ? "r": "-");
			strcat(toprint,(stats.st_mode & S_IWOTH) ? "w": "-");
			strcat(toprint,(stats.st_mode & S_IXOTH) ? "x": "-");
}

//this function is ignoring file that are hidden (starts with .)
void gatherInfo(int &count, char* current_dir, int &a, int &A, int &L){
	
	DIR *dp = NULL;
	struct dirent *direntptr = NULL;
	char complete_path[500];
	char time[100];
	struct passwd *pwd;
	struct group *grp;
	struct stat stats;
	
	
	fileInfo * files = (fileInfo *)malloc(sizeof(fileInfo)*count);
	if (files == NULL){
		perror("ERROR: MALLOC FOR fileInfo struct failed");
		
	}
	
	int currentfilecount = 0;
	
	dp = opendir((const char*)current_dir);
	
	
	for (currentfilecount = 0; NULL != (direntptr = readdir(dp)); currentfilecount++){
		if (a != 1 && direntptr->d_name[0] == '.'){
			currentfilecount--;
			continue;
		}
		else if (a != 1 && direntptr->d_name[0] != '.'){
			memset(complete_path,0 ,strlen(complete_path));
			sprintf(complete_path,"%s/%s", current_dir, direntptr->d_name);
			
			//get stat of complete path
			if (L == 0){
				if (lstat(complete_path, &stats) == -1){
					perror("ERROR: Failure on STAT");
					exit(EXIT_FAILURE);
				}
			}
			else if (L == 1){
				if (stat(complete_path, &stats) == -1){
					perror("ERROR: Failure on STAT");
					exit(EXIT_FAILURE);
				}
			}
			char * toprint = (char *)malloc(sizeof(char) * 300);
			if (toprint == NULL){
				perror("ERROR: MALLOC for string info failed");
			}
			
			memset(toprint, 0, strlen(toprint));
			char name[200];
			memset(name, 0, strlen(name));
			strcat(name, direntptr->d_name);
			
			//if the file is a directory append d to the string
			if (S_ISDIR(stats.st_mode) ){
				strcat(toprint, "d");	
			}
			
			
			//if file is a link add l to the string
			//on the case of L ==0 use readlink to get the name of the symbolic link, recompose the file name with symbolic link for later usage
			else if (S_ISLNK(stats.st_mode) && L==0){
				strcat(toprint, "l");
				//need to use readlink
				char buf[100];
				memset(buf, 0, strlen(buf));
				int readLinkStatus = readlink(complete_path, buf, sizeof(buf));
				if (readLinkStatus ==-1){
					perror("ERROR: readlink error");
				}
				else{
					buf[readLinkStatus] = '\0';
					memset(name, 0, strlen(name));
					strcat(name, direntptr->d_name);
					strcat(name, " -> ");
					strcat(name, buf);
				}
			}
			
			else //if (!S_ISDIR(stats.st_mode) && !S_ISLNK(stats.st_mode))
			{
				
				strcat(toprint, "-");
			}
			//get the 10 permission bits of the file
			getPermissionBit(toprint, stats);
			
			//pwd and grp are struct, use it to access username and group name
			pwd = getpwuid(stats.st_uid);
			grp = getgrgid(stats.st_gid);
			
			//time comes in ctime, +4 is to remove the day of week, copy 12 bytes to represent month(3c):date(2c):hour(2c):minute(2c)
			memset(time, 0, strlen(time));
			memcpy(time, ctime(&stats.st_mtime) +4, 12);
			time[12] = '\0';
			
			
			char info[100];
			sprintf(info, " %d %s %s %5lld %13s %s\n", stats.st_nlink, pwd->pw_name, grp->gr_name, stats.st_size, time, name);//, direntptr->d_name);
			strcat(toprint, info);
			
			//storing in fileInfo struct to use qsort later
			files[currentfilecount].name = direntptr->d_name;
			files[currentfilecount].size = stats.st_size;
			files[currentfilecount].info = toprint;
			//printf("%s\n", files[currentfilecount].info);	
		}
	}
	//sorting all the fileInfo by size and filename
	qsort(files, count, sizeof(fileInfo),compareInfo); 
	
	//printing out the file in sorted order
	for (int i = 0; i < count; i++){
		printf("%s", files[i].info);
	}
	printf("\n");
	closedir(dp);
	free(files);
}

//same as gatherInfo but for handling with files that are hidden
void gatherInfo_a(int &count, char* current_dir, int &a, int &A, int &L){
	
	DIR *dp = NULL;
	struct dirent *direntptr = NULL;
	char complete_path[500];
	char time[100];
	struct passwd *pwd;
	struct group *grp;
	struct stat stats;
	fileInfo * files = (fileInfo *)malloc(sizeof(fileInfo)*count);
	
	if (files == NULL){
		perror("ERROR: MALLOC FOR fileINFO struct failed");
		
	}
	
	int currentfilecount = 0;
	
	dp = opendir((const char*)current_dir);
	
	
	for (currentfilecount = 0; NULL != (direntptr = readdir(dp)); currentfilecount++){
		
		memset(complete_path,0 ,strlen(complete_path));
		sprintf(complete_path,"%s/%s", current_dir, direntptr->d_name);
		
		//get stat of complete path
		if (L == 0){
			if (lstat(complete_path, &stats) == -1){
				perror("ERROR: Failure on STAT");
				exit(EXIT_FAILURE);
			}
		}
		else if (L == 1){
			if (stat(complete_path, &stats) == -1){
				perror("ERROR: Failure on STAT");
				exit(EXIT_FAILURE);
			}
		}
		char * toprint = (char *)malloc(sizeof(char) * 300);
		if (toprint == NULL){
			perror("ERROR: MALLOC for string info failed");
		}
		memset(toprint, 0, strlen(toprint));
		char name[200];
		memset(name, 0, strlen(name));
		strcat(name, direntptr->d_name);
		
		if (S_ISDIR(stats.st_mode) ){
			strcat(toprint, "d");	
		}
		//need to check for other option
		
		
		else if (S_ISLNK(stats.st_mode) && L==0){
			strcat(toprint, "l");
			//need to use readlink
			char buf[100];
			memset(buf, 0, strlen(buf));
			int readLinkStatus = readlink(complete_path, buf, sizeof(buf));
			if (readLinkStatus ==-1){
				perror("ERROR: readlink error");
			}
			else{
				buf[readLinkStatus] = '\0';
				memset(name, 0, strlen(name));
				strcat(name, direntptr->d_name);
				strcat(name, " -> ");
				strcat(name, buf);
			}
		}
		
		else //if (!S_ISDIR(stats.st_mode) && !S_ISLNK(stats.st_mode))
		{
			
			strcat(toprint, "-");
		}
		getPermissionBit(toprint, stats);
		
		pwd = getpwuid(stats.st_uid);
		grp = getgrgid(stats.st_gid);
		
		memset(time, 0, strlen(time));
		memcpy(time, ctime(&stats.st_mtime) +4, 12);
		time[12] = '\0';
		
		
		char info[100];
		sprintf(info, " %d %s %s %5lld %13s %s\n", stats.st_nlink, pwd->pw_name, grp->gr_name, stats.st_size, time, name);//, direntptr->d_name);
		strcat(toprint, info);
		
		files[currentfilecount].name = direntptr->d_name;
		files[currentfilecount].size = stats.st_size;
		files[currentfilecount].info = toprint;
		//printf("%s\n", files[currentfilecount].info);	
		
	}
	//sorting all the fileInfo by size and filename
	qsort(files, count, sizeof(fileInfo),compareInfo); 
	
	//printing out the file in sorted order
	for (int i = 0; i < count; i++){
		printf("%s", files[i].info);
	}
	printf("\n");
	closedir(dp);
	free(files);
}
//this function handle if the argument is a file not a directory
void gatherFileInfo(char *current_file, int &a, int&A, int &L){
	char time[100];
	struct passwd *pwd;
	struct group *grp;
	struct stat stats;
	

	if (L == 0){
		if (lstat(current_file, &stats) == -1){
			perror("ERROR: Failure on STAT");
			exit(EXIT_FAILURE);
		}
	}
	else if (L == 1){
		if (stat(current_file, &stats) == -1){
			perror("ERROR: Failure on STAT");
			exit(EXIT_FAILURE);
		}
	}
	char * toprint = (char *)malloc(sizeof(char) * 300);
	if (toprint == NULL){
		perror("ERROR: MALLOC for string info failed");
	}
	memset(toprint, 0, strlen(toprint));
	char name[200];
	memset(name, 0, strlen(name));
	strcat(name, current_file);
	
	
	
	
	if (S_ISLNK(stats.st_mode) && L==0){
		strcat(toprint, "l");
		char buf[100];
		memset(buf, 0, strlen(buf));
		int readLinkStatus = readlink(current_file, buf, sizeof(buf));
		if (readLinkStatus ==-1){
			perror("ERROR: readlink error");
		}
		else{
			buf[readLinkStatus] = '\0';
			memset(name, 0, strlen(name));
			strcat(name, current_file);
			strcat(name, " -> ");
			strcat(name, buf);
			
		}
	}
	
	else //if (!S_ISDIR(stats.st_mode) && !S_ISLNK(stats.st_mode))
	{
		
		strcat(toprint, "-");
	}
	getPermissionBit(toprint, stats);
	
	pwd = getpwuid(stats.st_uid);
	grp = getgrgid(stats.st_gid);
	
	memset(time, 0, strlen(time));
	memcpy(time, ctime(&stats.st_mtime) +4, 12);
	time[12] = '\0';
	
	
	char info[100];
	sprintf(info, " %d %s %s %5lld %13s %s\n", stats.st_nlink, pwd->pw_name, grp->gr_name, stats.st_size, time, name);
	strcat(toprint, info);
	printf("%s", toprint);
}
	
void printLongListing(char *current_dir, int &a, int &A, int &L){
	
	DIR *dp = NULL;
	struct dirent *direntptr = NULL;
	int count = 0;
	int status = 0;
	char complete_path[500];
	char time[100];
	struct passwd *pwd;
	struct group *grp;
	struct stat stats;
	
	//assuming the path is a directory on sucess will move on and descend into directory to print files info
	//if the path is not a directory, check again if it is a file, if it is a file call gatherFileInfo to print file info
	//else exit failure 
	dp = opendir((const char*)current_dir);
	if (dp == NULL){
		struct stat file;
		if (stat(current_dir, &file) == -1){
			perror("ERROR: Cout not open current directory");
			exit(EXIT_FAILURE);
		}
		//if the argument is a file, print and return
		else{
			gatherFileInfo(current_dir, a, A, L);
			return;
		} 
		
	}
	
	
	//to get number of files in current directory to do fileInfo struct malloc
	for (count = 0; NULL!= (direntptr = readdir(dp)); count++){
		if (a != 1){
			if (direntptr->d_name[0] == '.'){
				count--;
			}
		}
		
	}
	
	closedir(dp);
	//calling different gatherInfo function depending on value of a
	if (a ==1){
		gatherInfo_a(count, current_dir, a, A, L);
	}
	else if (a == 0){
		gatherInfo(count, current_dir, a, A, L);
	}
	
}


int main(int argc, char* argv[]){
	
	int option_a = 0;
	int option_A = 0;
	int option_L = 0;
	
	int option;
	//opt_status ==-1 when no more argument
	//if getopt can't find the patter, it return ?
	while((option = getopt(argc, argv, "aAL")) != -1){
		switch(option){
			case 'a':
				option_a = 1;
				break;
			case 'L':
				option_L = 1;
				break;
			case 'A':
				option_A = 1;
				break;
			case '?':
				perror("ERROR: lss can only support -a, -A, -L");
				exit(EXIT_FAILURE);
		}	
	}
	
	
	int option_count = 0;
	//loops to see if there were any path given
	for (int i = 1; i < argc; i++){
		if (argv[i][0] == '-'){
			option_count++;
			continue;
		}
		printLongListing(argv[i], option_a, option_A, option_L);	
	}
	
	//running default on current directory if no path was found
	if (option_count == argc-1){
		char *current_dir;
		current_dir = getenv("PWD");
		if (NULL == current_dir){
			perror("ERROR: Could not get current directory");
			exit(EXIT_FAILURE);
		}
		printLongListing(current_dir, option_a, option_A, option_L);
	}
	
	
	
	return 0;
	
}