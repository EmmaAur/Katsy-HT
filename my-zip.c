/*
Sources: 
1. 
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>


void write_characters(char, int);


int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("my-zip: file1 [file2...]\n");
		exit(1);
	}

	// Initializing variables for reading the file
	FILE *fp_r;
	
	// Initializing variables for the compression
	int amount;
	char prev_char;
	char curr_char;
	
	// For-loop loop-de-looping through command line arguments
	for (int i = 1; i < argc; i++) {
		
		// Open file to read
		if ((fp_r = fopen(argv[i], "r")) == NULL) {
			printf("Cannot open file [%s].\n", argv[i]);
			exit(1);
		}
	
		// Initializing variables for the compression
		amount = 0;
		prev_char = '\0';
		curr_char = '\0';
		
		while ((curr_char = fgetc(fp_r)) != EOF) {
			// First round
			if (prev_char == '\0') {
				prev_char = curr_char;
			}
			
			// Compression logic
			if (curr_char == prev_char) {
				amount += 1;
			} else {
				//printf("%c%d", prev_char, amount);
				write_characters(prev_char, amount);
				amount = 1;
			}
			prev_char = curr_char;
		}
		
		/*
		   Using the system this code was written and tested with, it seems that
		   there is always a newline character at the end of each file before the EOF occurs.
		   Therefore this code assumes that there is a newline at the end of each file it
		   reads.
		*/
		
		amount -= 1;
		if (amount > 0) {
			//printf("%c%d", prev_char, amount);
			write_characters(prev_char, amount);
		}
    	fclose(fp_r);
    }
	return (0);
}



void write_characters(char character, int amount) {
	size_t w1 = fwrite(&character, sizeof(char), 1, stdout);
	if (w1 < 1) {
		printf("Error writing a character to stdout.\n");
		exit(1);
	}
	size_t w2 = fwrite(&amount, sizeof(int), 1, stdout);
	if (w2 < 1) {
		printf("Error writing an integer to stdout.\n");
		exit(1);
	}
}

