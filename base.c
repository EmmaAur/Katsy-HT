//srcs: 
// https://www.geeksforgeeks.org/c/snprintf-c-library/
// https://www.geeksforgeeks.org/linux-unix/access-command-in-linux-with-examples/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

extern int errno;

int main(int argc, char* argv[]) {
    int kytkin = 1; //variable for while-loop
    char *rivi = NULL;
    size_t len = 0;
    char basePath[] = "/usr/bin/"; //path to the executables
    int pathSize = 0;
    int accessCode = 0; //variable to hold access rights result

    //no arguments (except program name) = interactive mode
    if (argc < 2) {
        while (kytkin == 1) {
            printf("lush> ");
            getline(&rivi, &len, stdin);
            
            pathSize = strlen(basePath) + strlen(rivi) + 1; //'+1' for '\0'
            char path[pathSize]; //path to the specific command
            snprintf(path, sizeof(path), "%s%s", basePath, rivi); //concatenating the strings into one specific path
            printf("%s", path);
            path[strlen(path)-1] = '\0'; //stripping the newline-character from the path
            
            accessCode = access(path, X_OK); //check for execution
            if (accessCode == -1) {
                printf("Not executable. Error number %d\n", errno);
            } else {
                printf("Executable.\n");
            }


        }
    }
}