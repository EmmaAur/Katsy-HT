#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *Tiedosto = NULL;
    char rivi[50] = "";

    if (argc < 2) {
        return(0);
    }


    for (int i = 1; i < argc; i++) {
        
        if ((Tiedosto = fopen(argv[i], "r")) == NULL) {
            printf("my-cat: cannot open file\n");
            return(1);
        }

        while (fgets(rivi, 50, Tiedosto) != NULL) {
            printf("%s", rivi);
        }
    
        fclose(Tiedosto);
    }

    return(0);
}