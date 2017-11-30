#include <stdio.h>
#include <string.h>
#include "parse.h"


void parse2( char *command, struct commandLine* a){
	char str[10000];
	
	memset(str, 0, strlen(str));
	
	strcpy(str, command);
	str[strlen(command)-1] = '\0';

	a->total_command = 1;
	a->total_struct_count=0;
	a->IO_in_n = 0;
	a->IO_out_n = 0;
	a->if_first_arg_RD = 0;
	a->current_IO_out = 0;
	a->IO_recent[0] = '\0';
	sprintf(a->IO_out[0],"%c",'>') ;
	sprintf(a->IO_in[0],"%c",'<') ;
	memset(a->IO_in[1], '\0', strlen(a->IO_in[1]));
	memset(a->IO_out[1], '\0', strlen(a->IO_in[1]));
	
	
	
	
	command[strlen(command)-1] = '\0';
	int i;
	char buf[100];
	memset(buf, '\0', strlen(buf));
	int count = 0;
	int spaceCount = 0;
	
	
	
	
	for (i = 0; i <= strlen(str); i++){
		
		if ((str[i] != '|'  && str[i] != '<' && str[i] != '>'  && str[i] != ' ' && str[i] != '\0') || (str[i] == '-' && strlen(buf) == 0)){
			buf[count++] = str[i];
		}
		else {
			
			if (str[i] == '\0' && strlen(buf) == 0){
				continue;
			}
			
			if (str[i] == ' ' && strlen(buf) == 0){
				count = 0;
				continue;
			}
			
			
			if (a->IO_in_n ==1 && strlen(buf)!= 0 ){
				sprintf(a->IO_in[a->IO_in_n++], "'%s'", buf);
				spaceCount++;
				count = 0;
				
				a->total_struct_command[a->total_struct_count].option_count = 0;
				memset(buf, 0, strlen(buf));
				continue;
			}
			
			if (a->IO_out_n == 1 && strlen(buf)!= 0 && a->current_IO_out == a->total_struct_count ){
			
				sprintf(a->IO_out[a->IO_out_n++], "'%s'", buf);
				spaceCount++;	
				count = 0;
				a->total_struct_command[a->total_struct_count].option_count = 0;
				memset(buf, 0, strlen(buf));
				continue;
			}
			
			if ((str[i] == '<' || str[i] == '>') && a->total_struct_count == 0 && a->total_struct_command[0].argument_count==0 && strlen(a->total_struct_command[0].scriptname ) == 0 && a->total_struct_command[0].option_count == 0){
				a->total_command = 0;
			} 
			
			if (str[i] == ' ' && strlen(buf) != 0){
				if (spaceCount == 0){
					memset(a->total_struct_command[a->total_struct_count].scriptname, '\0', strlen(a->total_struct_command[a->total_struct_count].scriptname));
					sprintf(a->total_struct_command[a->total_struct_count].scriptname, "'%s'", buf);
					a->total_struct_command[a->total_struct_count].option_count = 0;
				}
				
				else if (spaceCount != 0 && buf[0] == '-'){
					memset(a->total_struct_command[a->total_struct_count].option[a->total_struct_command[a->total_struct_count].option_count],'\0',strlen(a->total_struct_command[a->total_struct_count].option[a->total_struct_command[a->total_struct_count].option_count]));
					sprintf(a->total_struct_command[a->total_struct_count].option[a->total_struct_command[a->total_struct_count].option_count++], "'%s'", buf);
					
				}
				else{
					memset(a->total_struct_command[a->total_struct_count].arguments[a->total_struct_command[a->total_struct_count].argument_count], '\0', strlen(a->total_struct_command[a->total_struct_count].arguments[a->total_struct_command[a->total_struct_count].argument_count]));
					sprintf(a->total_struct_command[a->total_struct_count].arguments[a->total_struct_command[a->total_struct_count].argument_count++], "'%s'", buf);
					
				}
				spaceCount++;
				memset(buf, 0, strlen(buf));
				count = 0;
				continue;
			}
			
			
			
			if (str[i] == '|' || str[i] == '<' || str[i] == '>' || str[i] == '\0'){

				if (str[i] == '<'){
					sprintf(a->IO_recent, "%c", str[i]);
					if (str[i-1] == '<'){
						sprintf(a->IO_in[0], "%s", "<<");
					}
					else{
						sprintf(a->IO_in[0], "%s", "<");
					}
					a->IO_in_n = 1;
					
				}
				
				if (str[i] == '>'){
					sprintf(a->IO_recent, "%c", str[i]);
					if (str[i-1] == '>'){
						
						sprintf(a->IO_out[0], "%s", ">>");
						
					}
					
					else{
						sprintf(a->IO_out[0], "%s", ">");
						
						a->current_IO_out = a->total_struct_count+1;
						
					}
					
					a->IO_out_n = 1;
					
				}
				
				if (str[i] == '\0' && a->IO_recent[0] != '|' && a->IO_recent[0] != '\0' ){
					if (strcmp(a->IO_recent, ">")==0){
						sprintf(a->IO_out[1], "'%s'", buf);
					}
					else{
						sprintf(a->IO_in[1], "'%s'", buf);
					}
					
					continue;
				}
				
				
				if (str[i] != '\0'){
					if (str[i] == '|' && strlen(buf) == 0){
						sprintf(a->total_struct_command[a->total_struct_count].IO, "%c", str[i]);
						a->total_command++;
						count = 0;
						spaceCount=0;
						a->total_struct_count++;
						continue;
					}
					
					else if ((str[i] == '>' || str[i] == '<' )&& strlen(buf) == 0){
						count = 0;
						spaceCount=0;
						if (str[i-1] != str[i])
							a->total_struct_count++;
						continue;	
					}
				}
				
				if (spaceCount == 0 && strlen(buf) != 0){
					memset(a->total_struct_command[a->total_struct_count].scriptname, '\0', strlen(a->total_struct_command[a->total_struct_count].scriptname));
					sprintf(a->total_struct_command[a->total_struct_count].scriptname, "'%s'", buf);
					a->total_struct_command[a->total_struct_count].option_count = 0;
						
				}
				
				else if (spaceCount != 0 && buf[0] == '-'){
					memset(a->total_struct_command[a->total_struct_count].option[a->total_struct_command[a->total_struct_count].option_count],'\0',strlen(a->total_struct_command[a->total_struct_count].option[a->total_struct_command[a->total_struct_count].option_count]));
					sprintf(a->total_struct_command[a->total_struct_count].option[a->total_struct_command[a->total_struct_count].option_count++], "'%s'", buf);
				}
				else{
					memset(a->total_struct_command[a->total_struct_count].arguments[a->total_struct_command[a->total_struct_count].argument_count], '\0', strlen(a->total_struct_command[a->total_struct_count].arguments[a->total_struct_command[a->total_struct_count].argument_count]));
					sprintf(a->total_struct_command[a->total_struct_count].arguments[a->total_struct_command[a->total_struct_count].argument_count++], "'%s'", buf);
				}
				
				memset(buf, '\0', strlen(buf));
				a->total_struct_count++;
				spaceCount=0;
			}
		
			if (str[i] == '|'){
				sprintf(a->total_struct_command[a->total_struct_count-1].IO, "%c", str[i]);
				a->total_command++;
			}
			
			count = 0;
		}
	}
	

	int j,k;
	
	printf("%d: ", a->total_command); 
	if (a->IO_in_n == 2){
		printf("%s%s ", a->IO_in[0], a->IO_in[1]);
	}
	else if (a->IO_in_n == 1){
		printf("%s''", a->IO_in[0]);
	}
	
	for (j = 0; j < a->total_struct_count; j++){
		if (strlen(a->total_struct_command[j].scriptname)!= 0)
			printf("%s ", a->total_struct_command[j].scriptname);
		
		for(k =0; k < a->total_struct_command[j].option_count; k++)
			printf("%s ", a->total_struct_command[j].option[k]);
		
		for (k = 0; k < a->total_struct_command[j].argument_count ; k++){
			printf("%s ", a->total_struct_command[j].arguments[k]);
		}
		
		if (a->total_struct_command[j].IO[0] == '|'  )
			printf("%c ",  a->total_struct_command[j].IO[0]);
	}
	if (a->IO_out_n == 2){
		printf("%s%s ", a->IO_out[0], a->IO_out[1]);
	}
	else if (a->IO_out_n == 1){
		printf("%s''", a->IO_out[0]);
	}
	
	
	for (j = 0; j < a->total_struct_count; j++){
		memset(a->total_struct_command[j].scriptname, '\0', strlen(a->total_struct_command[j].scriptname));
		memset(a->total_struct_command[j].IO, '\0', strlen(a->total_struct_command[j].IO));
		a->total_struct_command[j].argument_count = 0;
		
		for (k = 0; k < a->total_struct_command[j].option_count ; k++){
				memset(a->total_struct_command[j].option[k], '\0', strlen(a->total_struct_command[j].option[k]));
			}
		for (k = 0; k < a->total_struct_command[j].argument_count ; k++){
				memset(a->total_struct_command[j].arguments[k], '\0', strlen(a->total_struct_command[j].arguments[k]));
			}
	}
	
	memset(a->IO_out[0], '\0', strlen(a->IO_out[0]));
	memset(a->IO_out[1], '\0', strlen(a->IO_out[1]));
	memset(a->IO_in[0], '\0', strlen(a->IO_in[0]));
	memset(a->IO_in[0], '\0', strlen(a->IO_in[0]));
	
	
	printf("\n");	
}


// void parse2( char *command, struct commandLine* a){
	// char str[10000];
	
	// memset(str, 0, strlen(str));
	
	// strcpy(str, command);
	// str[strlen(command)-1] = '\0';

	// a->total_command = 1;
	// a->total_struct_count=0;
	// command[strlen(command)-1] = '\0';
	// int i;
	// char buf[100];
	// int count = 0;
	// int spaceCount = 0;
	
	
	
	// for (i = 0; i <= strlen(str); i++){
		
		// if ((str[i] != '|'  && str[i] != '<' && str[i] != '>'  && str[i] != ' ' && str[i] != '\0') || (str[i] == '-' && strlen(buf) == 0)){
			// buf[count++] = str[i];
		// }
		// else {
			// printf("%s\n", buf);
			// if (str[i] == ' ' && strlen(buf) != 0){
				// if (spaceCount == 0){
					// sprintf(a->total_struct_command[a->total_struct_count].scriptname, "'%s'", buf);
		
					// memset(a->total_struct_command[a->total_struct_count].option,0 ,strlen(a->total_struct_command[a->total_struct_count].option));
					
				// }
				
				// else if (spaceCount != 0 && buf[0] == '-'){
					// strcat(a->total_struct_command[a->total_struct_count].option, buf);
				// }
				// else{
					// sprintf(a->total_struct_command[a->total_struct_count].arguments[a->total_struct_command[a->total_struct_count].argument_count++], "'%s'", buf);
					
				// }
				// spaceCount++;
				// memset(buf, 0, strlen(buf));
				// count = 0;
				// continue;
			// }
			
			// if (str[i] == ' ' && strlen(buf) == 0){
				// printf("empty space\n");
				// count = 0;
				// continue;
			// }
			
			// if (str[i] == '|' || str[i] == '<' || str[i] == '>' || str[i] == '\0'){
				// //except null
				
				// if (str[i] != '\0'){
					// sprintf(a->total_struct_command[a->total_struct_count].IO, "%c", str[i]);
					// if (str[i] == '|' && strlen(buf) == 0){
						// sprintf(a->total_struct_command[a->total_struct_count].IO, "%c", str[i]);
						// printf("JUST SKIP\n");
						// a->total_command++;
						// count = 0;
						// spaceCount=0;
						// a->total_struct_count++;
						// continue;
					// }
					// //change here
					// else if ((str[i] == '>' || str[i] == '<' )&& strlen(buf) == 0){
						// printf("JUST SKIP\n");
						// count = 0;
						// spaceCount=0;
						// a->total_struct_count++;
						// continue;
						
					// }
					// // else if ((str[i] == '>' || str[i] == '<' )&& strlen(buf) == 0){
						// // printf("JUST SKIP\n");
						// // count = 0;
						// // spaceCount=0;
						// // a->total_struct_count++;
						// // continue;
						
					// // }
		
				// }
				
				// if (spaceCount == 0 && strlen(buf) != 0){
					
					// sprintf(a->total_struct_command[a->total_struct_count].scriptname, "'%s'", buf);
					
					
					// a->total_struct_command[a->total_struct_count].argument_count = 0;
					// memset(a->total_struct_command[a->total_struct_count].option,0 ,strlen(a->total_struct_command[a->total_struct_count].option));
					
				// }
				
				// else if (spaceCount != 0 && buf[0] == '-'){
					// strcat(a->total_struct_command[a->total_struct_count].option, buf);
					// printf("----%s\n",a->total_struct_command[a->total_struct_count].option);
					// printf("OPTION\n");
				// }
				// else{
					// sprintf(a->total_struct_command[a->total_struct_count].arguments[a->total_struct_command[a->total_struct_count].argument_count++], "'%s'", buf);
					// //printf("\nargument count %d %s %s\n",a->total_struct_command[a->total_struct_count].argument_count,buf,a->total_struct_command[a->total_struct_count].arguments[a->total_struct_command[a->total_struct_count].argument_count-1]);
				// }
				
				// memset(buf, 0, strlen(buf));
				// a->total_struct_count++;
				// spaceCount=0;
				
				
			// }
			// if (str[i] == '|'){
				// a->total_command++;
			// }
			
			// count = 0;
		// }
	// }
	

	// int j,k;
	
	// //swapping
	// for (j = 0; j < a->total_struct_count;j++){
		// if (a->total_struct_command[j].IO[0] == '<' && j+1 < a->total_struct_count){
			// //swap
			// struct command b;
			// b = a->total_struct_command[j+1];//1
			// printf("b %s\n", b.scriptname);
			// a->total_struct_command[j+1] = a->total_struct_command[j];//1=0
			
			
			// printf("c %s\n", a->total_struct_command[j].scriptname);
			// a->total_struct_command[j] = b;//0=1
			
			// a->total_struct_command[j+1].IO[0] = b.IO[0];
			// a->total_struct_command[j].IO[0] = '<';
			
		// }
	// }
	
	// printf("TOTAL STRUCT COUNT %d\n", a->total_struct_count);
	// printf("%d: ", a->total_command); 
	// for (j = 0; j < a->total_struct_count; j++){
		// //customize printing for < tomorrow
		// if (a->total_struct_command[j].IO[0] != '<'){
			// printf("%s ", a->total_struct_command[j].scriptname);
			// if (strlen(a->total_struct_command[j].option) != 0)
				// printf("'%s' ", a->total_struct_command[j].option);
			// for (k = 0; k < a->total_struct_command[j].argument_count ; k++){
				// printf("%s ", a->total_struct_command[j].arguments[k]);
			// }
			
			// if (a->total_struct_command[j].IO[0] == '<' || a->total_struct_command[j].IO[0] == '|' || a->total_struct_command[j].IO[0] == '>' )
				// printf("%c ",  a->total_struct_command[j].IO[0]);
		// }
		// //for < we print it first
		// else{
			// printf("%c ",  a->total_struct_command[j].IO[0]);
			// printf("%s ", a->total_struct_command[j].scriptname);
			// if (strlen(a->total_struct_command[j].option) != 0)
				// printf("'%s' ", a->total_struct_command[j].option);
			// for (k = 0; k < a->total_struct_command[j].argument_count ; k++){
				// printf("%s ", a->total_struct_command[j].arguments[k]);
			// }
			
		// }
	// }
	
	// for (j = 0; j < a->total_struct_count; j++){
		// memset(a->total_struct_command[j].scriptname, 0, strlen(a->total_struct_command[j].scriptname));
		// memset(a->total_struct_command[j].IO, 0, strlen(a->total_struct_command[j].IO));
		// a->total_struct_command[j].argument_count = 0;
		// memset(a->total_struct_command[j].option, 0, strlen(a->total_struct_command[j].option));
		// for (k = 0; k < a->total_struct_command[j].argument_count ; k++){
				// memset(a->total_struct_command[j].arguments[k], 0, strlen(a->total_struct_command[j].arguments[k]));
			// }
		
		
	// }
			
		
		
		// //%s %s %s %d\n",a->total_command, a->total_struct_command[j].IO, a->total_struct_command[j].scriptname, a->total_struct_command[j].option, a->total_struct_command[j].argument_count);
		
	
	// printf("\n");	
// }
// void parse2( char *command, struct commandLine* a){
	// char str[10000];
	
	// memset(str, 0, strlen(str));
	
	// strcpy(str, command);
	// str[strlen(command)-1] = '\0';
	// //struct commandLine a;
	// a->input_re_count = 0;
	// a->total_argument = 0;
	// a->total_command = 1;
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
			// if (str[i] == ' ' && strlen(buf) == 0){
				// printf("empty space\n");
				// count = 0;
				// continue;
			// }
			// if (str[i] == ' '){
				
				// sprintf(a->command_arg[a->total_argument++], "'%s'", buf);
				// memset(buf, 0, strlen(buf));
				// count = 0;
				// continue;
			// }
			
			
			// if (str[i] == '|' || str[i] == '<' || str[i] == '>' || str[i] == '\0'){
				
				// sprintf(a->command_arg[a->total_argument++], "'%s'", buf);
				// memset(buf, 0, strlen(buf));
				// if (str[i] != '\0'){
					// sprintf(a->command_arg[a->total_argument++], "%c", str[i]);
				// }
			// }
			// if (str[i] == '|'){
				// a->total_command++;
			// }
			// count = 0;
		// }
	// }
	
	// int j;
	// for (j = 0; j < a->total_argument; j++){
		// printf(" pipe %d %d %s\n",a->total_command, a->total_argument, a->command_arg[j]);
	// }
	// printf("\n");	
// }


// void parse(char* command, struct commandLine *a){
	// char command1[10000];
	// command[strlen(command)-1] = '\0';
	// //struct commandLine a;
	// a->input_re_count = 0;
	// a->total_argument = 0;
	// printf("Parse: %s\n", command);
	
	// char t[2] = " ";
	// char *token;
	
	// int count = 1;
	
	// token = strtok(command, t);
	// while (token != NULL){
		
		// if (a->input_re_count == 1){
			// sprintf(a->input_re[1], "%s", token);
			// a->input_re_count = 2;
			
		// }
		// else if (*token == '|'){
			// count++;
			// sprintf(a->command_arg[a->total_argument++], "%s", token);
		// }
		// else if (*token == '>'){
			// sprintf(a->command_arg[a->total_argument++], "%s", token);
		// }
		// else if (*token == '<'){
			// sprintf(a->input_re[0], "%s", token);
			// a->input_re_count = 1;
			
		// }
		// else
			// sprintf(a->command_arg[a->total_argument++], "'%s'", token);

		
		// token = strtok(NULL, t);
	// }
	
	// a->total_command = count;
	
	// int i;
	// printf("%d:", a->total_command);
	// for (i = 0; i < a->input_re_count; i++){
		// printf("%s ", a->input_re[i]);
		
	// }
	// for (i = 0; i < a->total_argument; i++){
		// printf("%s ", a->command_arg[i]);
	// }
	// printf("\n");

// }