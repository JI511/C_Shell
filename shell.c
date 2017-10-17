//C shell

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

void setupCommand(char* command);
char* removeSpace(char* string);
int executeCommand(int returnValue, int isFirst, int test);

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
            setupCommand(command);
            int returnValue = 0; //will be used for previous command value, 0 initially.
            int isFirst = 1; //check on if command is first in sequence (piping).
            //check if any args
            if (args[0] != NULL){
                if (strcmp(args[0], "exit") == 0){
                    //if exit is typed.
                    exit(0);
                }
                returnValue = executeCommand(returnValue, isFirst, 0);
            } else {
                returnValue = 0;
            }


        }

    }
    return 0;
}

//iterate through string removing any white space
char* removeSpace(char* string){
    while (isspace(*string)){
        ++string;
    }
    return string;
}

void setupCommand(char* command){
    command = removeSpace(command); //get rid of any leading white space
    char* space = strchr(command, ' ');
    int count = 0;
    while (space != NULL) { //While there exists another space
        space[0] = '\0';
        args[count] = command;
        count++;
        command = removeSpace(space + 1); //get new command (next param)
        space = strchr(command, ' '); //reset next space
    }
    printf("%c", *command);
    if (command[0] != '\0'){
        args[count] = command;
        space = strchr(command, '\n');
        space[0] = '\0';
        count++;
    }

    // last argument must be set to NULL.
    args[count] = NULL;

}

int executeCommand(int returnValue, int isFirst, int test){
    //how to handle pipe?

}
