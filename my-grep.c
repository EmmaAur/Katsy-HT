//colored text references : https://iq.opengenus.org/print-text-in-color-in-c/ & https://stackoverflow.com/questions/27469616/coloring-matching-substring-in-strstr
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>



int main(int argc, char* argv[]) {
    FILE *Tiedosto = NULL;
    char *rivi = NULL;
    size_t len = 0;
    __ssize_t luettu;

    if (argc < 2) {
        printf("my-grep: searchterm [file ...]\n");
        return(1);
    }

    if (argc == 2) {
        while((luettu = getline(&rivi, &len, stdin)) != -1) {
            char *matchi = strstr(rivi, argv[1]);
            if (matchi) {
                //logic for coloring text
                char *print_head = rivi;
                char *color_end = matchi + strlen(argv[1]) - 1;
                while (*print_head) {
                    if (print_head == matchi) {
                        //color
                        while (print_head < color_end + 1) {
                            printf("\033[1;31m%c\033[0m",*print_head);
                            print_head++;
                        }
                    } else if (print_head == color_end)
                    {
                        //no color
                        printf("\033[1;0m%c\033[0m",*print_head);
                        print_head++;
                    } else {
                        //no color for the string before the matching substring
                        printf("\033[1;0m%c\033[0m",*print_head);
                        print_head++;
                    }  
                }
            }
        }
    }

    if (argc > 2) {
        for(int i=2; i<argc; i++) {   
            if ((Tiedosto = fopen(argv[i], "r")) == NULL) {
                printf("my-grep: cannot open file\n");
                continue;
            }
            while ((luettu = getline(&rivi, &len, Tiedosto)) != -1)
            {
                char *matchi = strstr(rivi, argv[1]);
                if (matchi) {
                    //logic for coloring text
                    char *print_head = rivi;
                    char *color_end = matchi + strlen(argv[1]) - 1;
                    while (*print_head) {
                        if (print_head == matchi) {
                            //color
                            while (print_head < color_end + 1) {
                                printf("\033[1;31m%c\033[0m",*print_head);
                                print_head++;
                            }
                        } else if (print_head == color_end)
                        {
                            //no color
                            printf("\033[1;0m%c\033[0m",*print_head);
                            print_head++;
                        } else {
                            //no color for the string before the matching substring
                            printf("\033[1;0m%c\033[0m",*print_head);
                            print_head++;
                        }  
                    }
                }
            }
            fclose(Tiedosto);
        }
        free(rivi);
    }


    return(0);
    
}