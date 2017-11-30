#include "parse.h"
#include <assert.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
/*
** A very simple main program that re-prints the line after it's been scanned and parsed.
*/


void check_error(int n){
	if (n == -1){
		perror("");
		exit(EXIT_FAILURE);
	}
}
int main(int argc, char *argv[])
{
    FILE *input;
    char line[MAX_LINE];
	int status;
	

    if(argc == 2)
    {
		input = fopen(argv[1], "r");
	
		if(input == NULL)
		{
			perror(argv[1]);
			exit(1);
		}
    }
    else
    {
	assert(argc == 1);
		input = stdin;
		printf("? ");
		/* By default, printf will not "flush" the output buffer until
		* a newline appears.  Since the prompt does not contain a newline
		* at the end, we have to explicitly flush the output buffer using
		* fflush.
		*/
		fflush(stdout);
    }
	
    setlinebuf(input);
    while(fgets(line, sizeof(line), input))
    {
		int i;
		struct commandLine cmdLine;
		

		if(line[strlen(line)-1] == '\n')
			line[strlen(line)-1] = '\0';   /* zap the newline */

		Parse(line, &cmdLine);
		
		
		//implementing exit
		// if (cmdLine.numCommands == 1 && strcmp(cmdLine.argv[cmdLine.cmdStart[0]], "exit")){
			// printf("%s %s exit\n", cmdLine.argv[0], cmdLine.argv[1]);
		// }
	// printf("%d: ", cmdLine.numCommands);

		// if(cmdLine.infile)
			// printf("< '%s' ", cmdLine.infile);

		// for(i=0; i < cmdLine.numCommands; i++)
		// {
			// int j;
			// for(j=cmdLine.cmdStart[i]; cmdLine.argv[j] != NULL; j++)
				// printf("'%s' ", cmdLine.argv[j]);
			// if(i < cmdLine.numCommands - 1)
				// printf("| ");
		// }

		// if(cmdLine.append)
		// {
			// /* verify that if we're appending there should be an outfile! */
			// assert(cmdLine.outfile);
			// printf(">");
		// }
		// if(cmdLine.outfile)
			// printf(">'%s'", cmdLine.outfile);

	/* Print any other relevant info here, such as '&' if you implement
	 * background execution, etc.
	 */

		// printf("\n");
	
		int index;
		pid_t process_id;
		int error_check;
	
		int parent_p_in = dup(0);//making copy of the parent stdin
		check_error(parent_p_in);
		int parent_p_out= dup(1);//making copy of the parent stdout
		check_error(parent_p_out);
		int fdin;//default is parent in, else to a newly open file descriptor
		int fdout;//default is parent out, else to a newly open file descriptor
		
		

	
	
		//check if there is in infile to read from
		if (cmdLine.infile != NULL){
			fdin = open(cmdLine.infile, O_RDONLY);
			// check_error(fdin);
			if (fdin == -1){
				perror("");
				cmdLine.outfile = NULL;
				cmdLine.append = 0;
				fflush(stdout);	
				printf("? ");	
				continue;
			}
		}
	
		else{
			fdin = dup(parent_p_in);
			check_error(fdin);
		}
		int j;
		
		for (index = 0; index < cmdLine.numCommands; index++){
			//duplicate file descriptor to fdin; if a file is given, point to it else it is the default so 0
			error_check = dup2(fdin, 0);
			check_error(error_check);
			close(fdin);
			
			if (strcmp(cmdLine.argv[cmdLine.cmdStart[index]], "cd") == 0){
				
				//if there is no argument, then go back to root which is HOME
				if (cmdLine.argv[cmdLine.cmdStart[index]+1] == NULL){
					chdir(getenv("HOME"));
				}
				else{//there is a path
					chdir(cmdLine.argv[cmdLine.cmdStart[index]+1]);
				}	
				break;
			}
			
			//environemntal variable
			for(j=cmdLine.cmdStart[i]; cmdLine.argv[j] != NULL; j++){
				//$USER USER
				if((getenv(cmdLine.argv[j]+1) != NULL)){
					cmdLine.argv[j] = getenv(cmdLine.argv[j]+1);
				}
				else if(strchr(cmdLine.argv[j], '=') != NULL){
					putenv(cmdLine.argv[j]);
				}
				else if (strstr(cmdLine.argv[j] , "#") != NULL ){
					
					cmdLine.argv[j] = NULL;		
				}

			}
			

			if (index == cmdLine.numCommands-1){
				if (cmdLine.outfile != NULL && cmdLine.append == 0){
					fdout = open(cmdLine.outfile, O_RDWR| O_CREAT,0777);
					check_error(fdout);
				}
				else if (cmdLine.outfile != NULL && cmdLine.append == 1){
					fdout = open(cmdLine.outfile, O_APPEND| O_RDWR |O_CREAT, 0777);
					check_error(fdout);
				}
				else{
					fdout=dup(parent_p_out);
					check_error(fdout);
				}
			}
		//connecting pipe between command
			else{
			
				int fdpipe[2];
				error_check = pipe(fdpipe);//making pipe; fdpipe[0] is read; fdpipe[1] is write
				check_error(error_check);
				fdin = fdpipe[0];
				check_error(fdin);
				fdout=fdpipe[1];
				check_error(fdout);
			
			}
		//duplicate file descriptor to fdout, if a file is given fdout will point to it else the defaul is 1
			error_check = dup2(fdout,1);
			check_error(error_check);
			close(fdout);
		
			
			process_id = fork();
		
			if (process_id == -1){
				perror("fork() failed\n");
				exit(1);
			}
			else if (process_id == 0){//child process	
			//trying to use execvpe to handle environmental variable but can't access it once the child died
				// execvpe(cmdLine.argv[cmdLine.cmdStart[index]], &(cmdLine.argv[cmdLine.cmdStart[index]]), envp);
				execvp(cmdLine.argv[cmdLine.cmdStart[index]], &(cmdLine.argv[cmdLine.cmdStart[index]]));
				_exit(1);
				
			}
			else if (process_id != 0){
				wait(NULL);//parent wait for all process to finish
				// if (waitpid(process_id, &status, 0) ==-1){
					// perror("waitpid failed");
					// return EXIT_FAILURE;
				// }
			}
			
		}
		
		error_check = dup2(parent_p_in,0);//restore the parent file descriptor
		check_error(error_check);
		error_check = dup2(parent_p_out,1);//restore the parent file descriptor
		check_error(error_check);
		close(parent_p_in);
		close(parent_p_out);
		

		if(input == stdin)
		{
			//restore the value back to default to pass assert statement
			cmdLine.outfile = NULL;
			cmdLine.append = 0;
			fflush(stdout);	
			printf("? ");	
		}
	}

    return 0;
}
