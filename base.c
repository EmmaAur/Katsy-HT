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
#include <sys/wait.h>

extern int errno;

int main(int argc, char* argv[]) {
    int kytkin = 1; //variable for while-loop
    char *rivi = NULL;
    size_t len = 0;
    char basePath[] = "/usr/bin/"; //path to the executables
    int pathSize = 0;
    int accessCode = 0; //variable to hold access rights result
    
    /* ----------------------------------------------------------------------- */
    char error_message[30] = "An error has occurred.\n";
    
    // For built in commands
    char* cd_str = "cd";
    char* path_str = "path";
    char* exit_str = "exit";
    int cd_cmp;
    int path_cmp;
    int exit_cmp;
    /* ----------------------------------------------------------------------- */

    //no arguments (except program name) = interactive mode
    if (argc < 2) {
        while (kytkin == 1) {
            printf("lush> ");
            getline(&rivi, &len, stdin);
            
            if (strstr(rivi, " ") == NULL) { //IF there are no spaces (parameters) given for a command
                char *options[100];

                /* ----------------------------------------------------------------------- */
                // Built in commands

                rivi[strlen(rivi)-1] = '\0';
                
                // Check if the user wants to exit the shell program
                exit_cmp = strcmp(exit_str, rivi);
                if (exit_cmp == 0) {
					exit(0);
				}
                
                // Check if cd was called without parameters
                cd_cmp = strcmp(cd_str, rivi);
                if (cd_cmp == 0) {
					write(STDERR_FILENO, error_message, strlen(error_message));
                    continue;
				}
                
                rivi[strlen(rivi)] = '\n';
                /* ----------------------------------------------------------------------- */

                pathSize = strlen(basePath) + strlen(rivi) + 1; //'+1' for '\0'
                char path[pathSize]; //path to the specific command
                snprintf(path, sizeof(path), "%s%s", basePath, rivi); //concatenating the strings into one specific path
                //printf("%s", path);
                path[strlen(path)-1] = '\0'; //stripping the newline-character from the path
                options[0] = path;
                options[1] = '\0';
            
                accessCode = access(path, X_OK); //check for execution
                if (accessCode == -1) {
                    printf("Not executable. Error number %d\n", errno);
                    continue;
                } else {
                    printf("Executable.\n");
                }

                pid_t child_pid = fork();
                if (child_pid == -1) {
                    perror("Error creating process.\n");
                }

                if (child_pid == 0) {
                    //child process code
                    int statusCode = execv(path, options);

                    perror("Execv has failed.\n"); //if reaches this code, execv has failed
                    exit(1);
                } else {
                    //parent process code
                    waitpid(child_pid, NULL, 0);
                    printf("Child process is done.\n");
                }
            
            } else { //IF there are spaces, meaning there are parameters given
                //first-time initiliazations and such are done before the while-loop
                char *delimiter = strtok(rivi, " ");
                //printf("Delimiter on %s\n", delimiter);
                int kytkin2 = 1;
                
                char *options[100]; //we don't know how many options are given, but it is very unlikely the size 100 is exceeded
                int a = 1;
                int statusCode = 0;
                
                char *pathPtr = delimiter; //let's save the actual name of the command into its own variable
                char path[100];
                snprintf(path, sizeof(path), "%s%s", basePath, pathPtr); //so this is the actual command path without any options

                options[0] = path; //execv wants the first argument to be the executable command
               
                while (kytkin2 == 1) {
                    if ((delimiter = strtok(NULL, " ")) == NULL) {
                        kytkin2 = 0;
                        continue;
                    }
                    //replace the newline if this is the last argument
                    if (strstr(delimiter, "\n")) {
                        delimiter[strcspn(delimiter, "\n")] = '\0';
                    }

                    options[a] = delimiter;
                    //printf("Options %d on %s\n", a, options[a]);
                    a++;
                }

                /* ----------------------------------------------------------------------- */
                // Built in commands

                cd_cmp = strcmp(pathPtr, cd_str);
				if ((cd_cmp == 0) && (a == 2)) {
                    int chd = chdir(options[1]);
                    if (chd != 0) {
                        write(STDERR_FILENO, error_message, strlen(error_message));
                    }
                    continue;
				} else if ((cd_cmp == 0) && (a != 2)) {
					write(STDERR_FILENO, error_message, strlen(error_message));
                    //printf("Error reading [cd] command. [cd] requires 1 argument: cd /new/path\n");
                    continue;
                }
                
                path_cmp = strcmp(pathPtr, path_str);
				if ((path_cmp == 0) && (a == 1)) {
					printf("path successful!\n");
                    continue;
				} else if ((path_cmp == 0) && (a != 1)) {
                    write(STDERR_FILENO, error_message, strlen(error_message));
					//printf("Error reading [path] command.\n");
				    continue;
                }
                /* ----------------------------------------------------------------------- */

                options[a] = '\0'; //execv requires null terminator at the end of array

                /**for (int i=0; i<a; i++) {
                    printf("%s", options[i]);
                }*/
                pid_t child_pid = fork();
                if (child_pid == -1) {
                    perror("Error creating process.\n");
                }

                if (child_pid == 0) {
                    //child process code
                    statusCode = execv(path, options);

                    perror("Execv has failed.\n"); //if reaches this code, execv has failed
                    exit(1);
                } else {
                    //parent process code
                    waitpid(child_pid, NULL, 0);
                    printf("Child process is done.\n");
                }
            }
        }
    }
}
