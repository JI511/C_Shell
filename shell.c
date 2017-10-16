//C shell

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

void setupCommand(char* command);

char *args[256];
pid_t pid;
char str[256];

int main(int argc, char const *argv[]) {
    printf("C Shell. Type 'exit' to quit.\n");
    while (1){
        printf("$>");
        fflush(NULL); //print prompt and flush everything out.

        //char *fgets(char *str, int n, FILE *stream);
        if (!fgets(str, 256, stdin)){
            return 0; //end program if no command entered.
            //maybe add message or rerun loop?
        }
        char* command = str; //entire command
        //char *strchr(const char *str, int c) get location of pipe
        char* pipe = strchr(command, '|'); //returns pointer to location of pipe. NULL if none.

        while (pipe != NULL){ //While there exists another pipe.
            *pipe = '\0'; //set the current location to null terminator.



        }

    }
    return 0;
}

char* removeSpace(char* string){
    while (isspace(*string)){
        ++string;
    }
    return string;
}

void setupCommand(char* command){
    command = removeSpace(command); //get rid of any leading white space


}
