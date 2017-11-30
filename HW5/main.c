#include <stdio.h>

#include "parse.h"

// struct commandLine{
	// int total_command;
	// char command_arg[30][400];
	// char input_re[10][400];
	// int input_re_count;
	// int total_argument;
// };

// void parse2( char *command, struct commandLine* a){
	// char str[100];
	
	// memset(str, 0, strlen(str));
	
	// strcpy(str, command);
	// str[strlen(command)-1] = '\0';
	// struct commandLine a;
	// a.input_re_count = 0;
	// a.total_argument = 0;
	// command[strlen(command)-1] = '\0';
	// int i;
	// char buf[100];
	// int count = 0;
	
	
	// for (i = 0; i <= strlen(str); i++){
		
		// if (str[i] != '|'  && str[i] != '<' && str[i] != '>'  && str[i] != ' ' && str[i] != '\0'){
			// printf("%c\n", str[i]);
			// buf[count++] = str[i];

			
		// }
		// else {
			// printf("%s\n", buf);
			// if (str[i] == ' ' ){
				// memset(buf, 0, strlen(buf));
				// continue;
			// }
			// strcpy(a.command_arg[a.total_argument++], buf);
			// memset(buf, 0, strlen(buf));
			
			// if (str[i] == '|' || str[i] == '<' || str[i] == '>'){
				// strcpy(a.command_arg[a.total_argument++], &str[i]);
			// }
			// if (str[i] == '|'){
				// a.total_command++;
			// }
			// count = 0;
		// }
	// }
	
	// int j;
	// for (j = 0; j < a.total_argument; j++){
		// printf(" pipe %d %d %s\n",a.total_command, a.total_argument, a.command_arg[j]);
	// }
	// printf("\n");	
// }


// void parse(char* command){
	// char command1[100];
	// command[strlen(command)-1] = '\0';
	// struct commandLine a;
	// a.input_re_count = 0;
	// a.total_argument = 0;
	// printf("Parse: %s\n", command);
	
	// char t[2] = " ";
	// char *token;
	
	// int count = 1;
	
	// token = strtok(command, t);
	// while (token != NULL){
		
		// if (a.input_re_count == 1){
			// sprintf(a.input_re[1], "%s", token);
			// a.input_re_count = 2;
			
		// }
		// else if (*token == '|'){
			// count++;
			// sprintf(a.command_arg[a.total_argument++], "%s", token);
		// }
		// else if (*token == '>'){
			// sprintf(a.command_arg[a.total_argument++], "%s", token);
		// }
		// else if (*token == '<'){
			// sprintf(a.input_re[0], "%s", token);
			// a.input_re_count = 1;
			
		// }
		// else
			// sprintf(a.command_arg[a.total_argument++], "'%s'", token);

		
		// token = strtok(NULL, t);
	// }
	
	// a.total_command = count;
	
	// int i;
	// printf("%d:", a.total_command);
	// for (i = 0; i < a.input_re_count; i++){
		// printf("%s ", a.input_re[i]);
		
	// }
	// for (i = 0; i < a.total_argument; i++){
		// printf("%s ", a.command_arg[i]);
	// }
	// printf("\n");

// }

int main( int argc, char *argv[]){
	
	char* command = NULL;
	size_t len = 0;
	struct commandLine a;
	while(1){
		
		printf("? ");
		if (getline(&command, &len, stdin) == EOF){
			printf("\n");
			break;
		}
		
		
		//printf("%s\n", command);
		// parse(command);
		parse2(command, &a);
		
	}
	return 0;
	
}
