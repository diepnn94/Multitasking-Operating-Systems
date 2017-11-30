#ifndef PARSE_H
#define PARSE_H

//cat<infile | grep |wc
struct command{
	char IO[5];
	char scriptname[200];
	char option[20][100];
	char arguments[30][200];
	int argument_count;
	int option_count;
	
};

struct commandLine{
	int total_command;
	struct command total_struct_command[30];
	int total_struct_count;
	char IO_in[5][200];
	char IO_out[5][200];
	int IO_in_n;
	int IO_out_n;
	int current_IO_out;
	char IO_recent[5];
};

// struct commandLine{
	// int total_command;
	// char command_arg[30][500];
	// char input_re[10][500];
	// int input_re_count;
	// int total_argument;
// };


void parse(char * line, struct commandLine *command);
void parse2(char *line, struct commandLine *command);

#endif