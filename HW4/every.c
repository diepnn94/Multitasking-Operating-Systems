#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

//file is used for wc, to get the total number of lines in a file to print
struct file{
	char fileName[100];
	int lineNum;
};

int main(int argc, char* argv[]){
	
	//if argc is 1, the program behaves like cat(1)
	if (argc == 1){
		system("cat");
		return 0;
	}
	
	struct file files[argc-1];
	int valid_file = 0;
	char command1[300];
	char command2[300];
	char result[100];
	char option_sep[100] = "NONE";
	int M =1;
	int N = 1;
	 	
	//loop through the argument and compose the script for each argument
	//then pass the composed command1 to popen and scan the result back for further checking
	for (int i = 1; i < argc; i++){
		memset(command1, 0, strlen(command1));
		memset(result, 0, strlen(result));
		sprintf(command1, "if [ -f %s ]; then wc -l %s | awk '{print $1}';\
 			elif [[ %s =~ ^[-][0-9]+[,][0-9]+$ ]];then echo option;\
			elif [[ %s =~ ^[-][0-9]+$ ]]; then echo default; \
			elif [[ %s =~ ^[-][0-9]+[,]$ ]]; then echo error1;\
			elif [[ %s =~ ^[-][0-9]+[,][-][0-9]+$ ]]; then echo error1;\
			else echo error; fi", argv[i],argv[i], argv[i], argv[i], argv[i], argv[i]); 
		
		//scanning the result from the response of pipe
		FILE *pipe;
		pipe = popen(command1,"r");
		fscanf(pipe, "%s", result );
		pclose(pipe);
		
		//if the result is a valid option and the position of argument is 1, then parse the option to get value of M, N
		//if result is a valid option but it appears after files then raise error
		if (strcmp(result, "option") == 0){
			if (strcmp(option_sep, "NONE") == 0 && (i == 1)){
				memset(option_sep, 0, strlen(option_sep));
				memcpy(option_sep, argv[i]+1, strlen(argv[i])-1);
				char s[2] = ",";
				char *token1;
				char *token2;
				token1 = strtok(option_sep, s);
				token2 = strtok(NULL,s);
				
				N = atoi(token1);
				M = atoi(token2);
				
				if (!(M<=N)){
					perror("ERROR: M must be less than or equal to N\n.");
					exit(EXIT_FAILURE);
				}
			}
			else if ( (i != 1) && (argc > 2) ){
				perror("-N,M must be specified before files.\n");
				exit(EXIT_FAILURE);
			}
				
			
		}
		
		//if result is error, then it is not a file, print message and skip
		else if (strcmp(result, "error") == 0  ){
			printf("ERROR: '%s' is an invalid file.\n", argv[i]);
		}
		
		//if result is error1 and argc is 1, then it is an invalid option so raise error and exit
		//else if argc is not 1, it is assumed to be invalid file
		else if (strcmp(result, "error1") == 0 && i == 1){
			perror("ERROR: The following option is invalid");
			exit(EXIT_FAILURE);
		}
		
		//if result is default and argc is 1, then M is missing then M = 1
		else if (strcmp(result, "default") == 0 && strcmp(option_sep, "NONE") == 0 && i == 1){
			printf("M is missing: default is 1.\n");
			memset(option_sep, 0, strlen(option_sep));
			memcpy(option_sep, argv[i]+1, strlen(argv[i])-1);
			N = atoi(option_sep);
			printf("%d\n", N);			
		}
		
		//if result is default and argc !=1 and no option has been found, raise wrong position error message
		else if (strcmp(result, "default") == 0 && strcmp(option_sep, "NONE") ==0  && i != 1 && argc > 2){
			perror("ERROR: Option M must appear before files.\n");
			exit(EXIT_FAILURE);
			
		}
		
		//if result is default and argc != 1 and option has been found already, raise option already defined error message
		else if (strcmp(result, "default") == 0 && strcmp(option_sep, "NONE") != 0  && i != 1 && argc > 2){
			perror("ERROR: option N,M had already been defined.\n");
			exit(EXIT_FAILURE);
			
		}
		
		//if result is a number, then it is a valid file and the line count is the result
		//store the filename and linecount in the struct to access later
		else if (isdigit(*result)){
			strcpy(files[valid_file].fileName,argv[i]);
			files[valid_file].lineNum = atoi(result);
			valid_file++;
		}
	}
	//If there were no option found in the argument, look for environmental variable EVERY
	//If EVERY exists, use EVERY value as default
	//IF EVERY does not exists, use default -1,1
	if (strcmp(option_sep, "NONE") == 0){
		
		memset(command1, 0, strlen(command1));
		memset(result, 0, strlen(result));
		strcpy(command1, "if env|grep -q EVERY; then echo $EVERY;else echo NONE; fi");
		FILE *pipe;
		pipe = popen(command1,"r");
		fscanf(pipe, "%s", result );
		pclose(pipe);	
		
		if (strcmp(result, "NONE") != 0){
			memset(option_sep, 0, strlen(option_sep));
			memcpy(option_sep, result+1, strlen(result)-1);
			char s[2] = ",";
			char *token1;
			char *token2;
			token1 = strtok(option_sep, s);
			token2 = strtok(NULL,s);
			
			printf("Environmental variable EVERY found: N = %s, M = %s.\n", token1, token2);
			N = atoi(token1);
			M = atoi(token2);
		}
		else{
			printf("Cannot find environmental EVERY: default N = 1, M = 1.\n");
		}
	}
	
	//first loop through the struct that holds all the valid files
	//access the total line number and use M and N as the range to construct command2 script
	//to use it as input for system()
	
	for (int i =0;i < valid_file; i++){//for each valid file to get print
		printf("\nFILE Name: %s\n", files[i].fileName);
		if (N == 0 || M == 0){
			continue;
		}
		for(int j = 1; j <= files[i].lineNum; j = j+N){
			for (int k =j; k < j+M; k++){
				if (k > files[i].lineNum)
					break;
				memset(command2, 0, strlen(command2));
				char line[20];
				char num[20];
				sprintf(num, "%d", k);
				sprintf(line, "%d", k-1);
				strcat(command2, "awk '{printf(\"line %d: %s\\n\",");
				strcat(command2, line);
				strcat(command2, " ,$0)}' ");
				strcat(command2, files[i].fileName);
				strcat(command2, "| awk 'FNR==");
				strcat(command2, num);
				strcat(command2, "'");
				
				system(command2);
				
			}// awk '{printf( "line %d: %s\n",5, $0)}' testing.txt| awk 'FNR==5'
		}
	}
	
	
	return 0;
	
}