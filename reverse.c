/*
Sources:
1. get_line() funktion käyttö: zetcode.com/clang/getline/
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef struct list_node {
    char *content;
	struct list_node* previous;
}NODE;


NODE* add_node(NODE*, char*, int);
void reverse_array(NODE*, FILE*);
NODE* read_file(char*, NODE*);
NODE* read_user_input(NODE*);
void free_memory(NODE*);


int main (int argc, char* argv[]) {
    NODE* head_node = NULL;
    FILE* out_file;
    int same_file;   

	if (argc == 1) {
        // Ask for user input and reverse that
        out_file = stdout;
		head_node = read_user_input(head_node);
    	reverse_array(head_node, out_file);
		free_memory(head_node);
	} else if (argc == 2) {
        // Print the contents of the input file
        out_file = stdout;
		head_node = read_file(argv[1], head_node);
    	reverse_array(head_node, out_file);
		free_memory(head_node);
	} else if (argc == 3) {
        // Read contents from a file and write the reversed output to another file
        same_file = strcmp(argv[1], argv[2]);
        if (same_file == 0) {
            fprintf(stderr, "Input and output file must differ\n");
        }
        // Check if opening file succeeded
		out_file = fopen(argv[2], "w");
        if (out_file == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[2]);
		    exit(1);
        }
        head_node = read_file(argv[1], head_node);
    	reverse_array(head_node, out_file);
		free_memory(head_node);
		fclose(out_file);
    } else {
        // Too many arguments
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    };
	return (0);
}



NODE* read_file(char* file_in, NODE* head_node) {
    // Read lines from a line and add them to a linked list
	char *line_buffer = NULL;
	size_t length = 0;
	FILE* file;
	ssize_t line;
    // Check if opening file succeeded
	file = fopen(file_in, "r");
	if (file == NULL) {
        fprintf(stderr, "error: cannot open file '%s'\n", file_in);
		exit(1);
	};
	// Create linked list to save file contents
	while ((line = getline(&line_buffer, &length, file)) != -1) {
		head_node = add_node(head_node, line_buffer, length);	
	};
	fclose(file);
	free(line_buffer);
	return (head_node);
}


NODE* read_user_input(NODE* head_node) {
    // Ask for user input and add each input to a linked list
    char *line_buffer = NULL;
	size_t length = 0;
	ssize_t line;
    int end_loop;
    char end_str[] = "-1\n";
	
    printf("Give input (-1 ends): ");
	while ((line = getline(&line_buffer, &length, stdin)) != -1) {
        end_loop = strcmp(end_str, line_buffer);
        if (end_loop == 0) {
            break;
        }
		head_node = add_node(head_node, line_buffer, length);	
    	printf("Give input (-1 ends): ");
    };
	free(line_buffer);
    return (head_node);
}


NODE* add_node(NODE *head_node, char* line_buffer, int size) {
    // Add new nodes to the linked list
	NODE* new_node;
	new_node = (NODE*) malloc(sizeof(NODE));
    if (new_node == NULL) {
        fprintf(stderr, "Malloc failed.\n");
    }
	new_node->content = malloc(size+1);
	strcpy(new_node->content, line_buffer);
	new_node->previous = head_node;
	return new_node;
}


void reverse_array(NODE* head_node, FILE* out_file) {
    // Reverse the linked list and write its contents to out_file
	NODE *current_node = head_node;
	while (current_node != NULL) {
		fprintf(out_file, "%s", current_node->content);
		current_node = current_node->previous;
	}
}


void free_memory(NODE* head_node) {
    // Free memory used in the linked list 
	NODE *current_node = head_node;
	NODE *previous = current_node;
	
	while (current_node != NULL) {
		previous = current_node->previous;
		
		free(current_node->content);
		current_node->content = NULL;
		current_node->previous = NULL;
		free(current_node);
		
		current_node = previous;
	}
}

// EOF

