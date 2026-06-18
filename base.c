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
    ssize_t read;
    char *pathArray[100]; //array for paths
    int pathSum = 2; //sum of the paths inside the array. Gets increased everytime path command adds more paths
    int accessCode = 0; //variable to hold access rights result
    //let's set the base paths in the pathArray
    pathArray[0] = "/usr/bin/";
    pathArray[1] = "/bin/";

    //no arguments (except program name) = interactive mode
    if (argc < 2) {
        while (kytkin == 1) {
            printf("lush> ");
            getline(&rivi, &len, stdin);
            
            if (strstr(rivi, " ") == NULL) { //IF there are no spaces (parameters) given for a command
                char *options[100];
                char path[100];

                rivi[strlen(rivi)-1] = '\0'; //adding null-terminator to the command
            
                for (int i = 0; i < pathSum; i++) {
                    snprintf(path, 100, "%s%s", pathArray[i], rivi); //concatenating the strings to create final paths
                    //printf("%s", path);

                    accessCode = access(path, X_OK); //check for execution
                    if (accessCode == -1) {
                        printf("Not executable. Error number %d\n", errno);
                        if (i == pathSum-1) {
                            printf("Error: Command not found in any path.\n");
                            exit(1);
                        }
                        continue;
                    } else {
                        printf("Executable.\n");
                        break;
                    }
                }

                //adding the arguments to the options
                options[0] = path;
                options[1] = '\0';
                

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
                char path[100];
                
                char *pathPtr = delimiter; //let's save the actual name of the command into its own variable
               
            
                for (int i = 0; i < pathSum; i++) {
                    snprintf(path, 100, "%s%s", pathArray[i], pathPtr); //concatenating the strings to create final paths

                    //printf("%s", path);

                    accessCode = access(path, X_OK); //check for execution
                    if (accessCode == -1) {
                        printf("Not executable. Error number %d\n", errno);
                        if (i == pathSum-1) {
                            printf("Error: Command not found in any path.\n");
                            exit(1);
                        }
                        continue;
                    } else {
                        printf("Executable.\n");
                        break;
                    }
                }

                options[0] = path; //execv wants the first argument to be the executable command

               //in this while loop we grab the command arguments
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
    } else if (argc == 2) // two arguments = batch mode
    {
        FILE *Tiedosto = NULL;
        char *fname = argv[1];
        if ((Tiedosto = fopen(fname, "r")) == NULL) {
            printf("Error while opening file. Check filename.\n");
            exit(1);
        }
        while ((read = getline(&rivi, &len, Tiedosto)) != -1) { //the code for the next if-else statement is the same as in interactive mode
            if (strstr(rivi, " ") == NULL) { //IF there are no spaces (parameters) given for a command
                char *options[100];

                char path[100];

                rivi[strlen(rivi)-1] = '\0'; //adding null-terminator to the command
            
                for (int i = 0; i < pathSum; i++) {
                    snprintf(path, 100, "%s%s", pathArray[i], rivi); //concatenating the strings to create final paths
                    //printf("%s", path);

                    accessCode = access(path, X_OK); //check for execution
                    if (accessCode == -1) {
                        printf("Not executable. Error number %d\n", errno);
                        if (i == pathSum-1) {
                            printf("Error: Command not found in any path.\n");
                            exit(1);
                        }
                        continue;
                    } else {
                        printf("Executable.\n");
                        break;
                    }
                }

                //adding the arguments to the options
                options[0] = path;
                options[1] = '\0';

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
                char path[100];
                
                char *pathPtr = delimiter; //let's save the actual name of the command into its own variable
               
            
                for (int i = 0; i < pathSum; i++) {
                    snprintf(path, 100, "%s%s", pathArray[i], pathPtr); //concatenating the strings to create final paths

                    //printf("%s", path);

                    accessCode = access(path, X_OK); //check for execution
                    if (accessCode == -1) {
                        printf("Not executable. Error number %d\n", errno);
                        if (i == pathSum-1) {
                            printf("Error: Command not found in any path.\n");
                            exit(1);
                        }
                        continue;
                    } else {
                        printf("Executable.\n");
                        break;
                    }
                }

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
    } else {
        printf("Error. The shell can be invoked with only 1 or 2 arguments i.e. ./lush or ./lush filename.txt");
        exit(1);
    }
    
}