/*
Sources:
1. Turn char into an integer: sentry.io/answers/char-to-int-in-c-and-cpp/
*/


#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdbool.h>
#include <ctype.h>


int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("my-unzip: file1 [file2...]\n");
		exit(1);
	}

	// Initializing variables for reading the file
	FILE *fp_r;
	char buff_char[1];
	int buff_int[1];
	size_t read_amount;
    
	for (int i = 1; i < argc; i++) {
		// Open file and check if it exists
		if ((fp_r = fopen(argv[i], "rb")) == NULL) {
			printf("Cannot open file [%s].\n", argv[i]);
			exit(1);
		}
		
		// Unzip logic
		while ((read_amount = fread(buff_char, sizeof(char), 1, fp_r) > 0) 
				&& fread(buff_int, sizeof(int), 1, fp_r)) 
		{
			for (i = 0; i < buff_int[0]; i++) {
				printf("%c", buff_char[0]);
			}
			//printf("%c%d", buff_char[0], buff_int[0]);
		}
	}
    printf("\n"); // This is here to make the output look prettier
	return (0);
}


