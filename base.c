//srcs: 
// https://www.geeksforgeeks.org/c/snprintf-c-library/
// https://www.geeksforgeeks.org/linux-unix/access-command-in-linux-with-examples/
// http://calsnotes.com/posts/how-to-use-fork-wait-and-exec/how-to-use-fork-wait-and-exec/
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
            
            if (strstr(rivi, " ") == NULL) { //IF there are no spaces (parameters) given for a command
                pathSize = strlen(basePath) + strlen(rivi) + 1; //'+1' for '\0'
                char path[pathSize]; //path to the specific command
                snprintf(path, sizeof(path), "%s%s", basePath, rivi); //concatenating the strings into one specific path
                printf("%s", path);
                path[strlen(path)-1] = '\0'; //stripping the newline-character from the path
            
                accessCode = access(path, X_OK); //check for execution
                if (accessCode == -1) {
                    printf("Not executable. Error number %d\n", errno);
                    continue;
                } else {
                    printf("Executable.\n");
                }
                system(path); //system executes the program residing in the path
            } else { //IF there are spaces, meaning there are parameters given
                //first-time initiliazations and such are done before the while-loop
                char *delimiter = strtok(rivi, " ");
                printf("Delimiter on %s\n", delimiter);
                int kytkin = 1;
                
                char *options[100]; //we don't know how many options are given, but it is very unlikely the size 100 is exceeded
                int a = 0;
                
                char *pathPtr = delimiter; //let's save the actual name of the command into its own variable
                char path[100];
                snprintf(path, sizeof(path), "%s%s", basePath, pathPtr); //so this is the actual command path without any options
               
                while (kytkin == 1) {
                    if ((delimiter = strtok(NULL, " ")) == NULL) {
                        kytkin = 0;
                    }
                    options[a] = delimiter;
                    printf("Options %d on %s\n", a, options[a]);
                    a++;
                }
                options[a] = '\0';

                printf("%s", path);
                for (int i=0; i<a; i++) {
                    printf("%s", options[i]);
                }

            }
            
            

        }
    }
}