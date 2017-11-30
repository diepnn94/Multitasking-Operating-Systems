#include <stdio.h>
#include <string.h>


int main(int argc, char* argv[]){

	printf("The number of argument is %d\n", argc);
	for (int i = 0; i < argc; i++){
		printf("argument is %s\n",argv[i] );
	}

	return 0;

}
