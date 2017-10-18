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
void clean(int i);

char *args[256];
pid_t pid;
char str[256];
int count = 0;

int main(int argc, char const *argv[]) {
    printf("C Shell. Type 'exit' to quit.\n");
    while (1){
        printf("$> ");
        fflush(NULL); //print prompt and flush everything out.

        int returnValue = 0; //will be used for previous command value, 0 initially.
        int isFirst = 1; //check if command is first in sequence (piping).

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

            //check if any args
            if (args[0] != NULL){
                if (strcmp(args[0], "exit") == 0){
                    //if exit is typed.
                    exit(0);
                }
                count = count + 1; // new process created
                returnValue = executeCommand(returnValue, isFirst, 0);
            } else {
                returnValue = 0;
            }
            command = pipe + 1; //increment to get next command.
            pipe = strchr(command, '|'); //find next
            isFirst = 0;
        }

        //deal with last command (or only command if no piping).
        setupCommand(command);
        if (args[0] != NULL){
            if (strcmp(args[0], "exit") == 0){
                exit(0);
            }
            returnValue = executeCommand(returnValue, isFirst, 1);
        } else {
            returnValue = 0;
        }
        clean(count); //make sure all processes are terminated.
        count = 0;
        fflush(NULL);
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
    int count = 0;

    command = removeSpace(command); //get rid of any leading white space
    char* space = strchr(command, ' '); //find end of command.

    while (space != NULL) { //While there exists another space
        space[0] = '\0';
        args[count] = command;
        count++;
        command = removeSpace(space + 1); //get new command (next param)
        space = strchr(command, ' '); //reset next space
    }
    //printf("COMMAND: %s\n", command);
    if (command[0] != '\0'){
        args[count] = command;
        space = strchr(command, '\n');
        space[0] = '\0';
        count++;
    }

    // last argument must be set to NULL.
    args[count] = NULL;

}

void clean(int i){
    for (int k = 0; k < i; k++){
        wait(NULL);
    }
}

int executeCommand(int returnValue, int isFirst, int test){
    //how to handle pipe?
    //http://codewiki.wikidot.com/c:system-calls:dup2
    //dup2 is a system call similar to dup in that it duplicates one file descriptor,
    //making them aliases, and then deleting the old file descriptor. This becomes very
    //useful when attempting to redirect output, as it automatically takes care of closing
    //the old file descriptor, performing the redirection in one elegant command. For example,
    //if you wanted to redirect standard output to a file, then you would simply call dup2,
    //providing the open file descriptor for the file as the first command and 1 (standard output)
    //as the second command.

    // int file = open("myfile.txt", O_APPEND | O_WRONLY);
    // f(dup2(file,1) < 0)


    int pipes[2];
    pipe(pipes);
    pid = fork();
    if (pid == 0) { //child process
        if (isFirst == 0 && test == 0 && returnValue != 0){
            dup2(returnValue, STDIN_FILENO);
            dup2(pipes[1], STDOUT_FILENO); //writing pipe
        }
        else if (returnValue == 0 && isFirst == 1 && test == 0){
            dup2(pipes[1], STDOUT_FILENO); //writing pipe
        }
        else {
            dup2(returnValue, STDIN_FILENO);
        }

        //run command
        int run = execvp(args[0], args);
        if (run == -1) {
            _exit(2);
        }
    }

    // close if needed
    if (returnValue != 0){
        close(returnValue);
    }

    close(pipes[1]); //no more writing to be done.

    if (test == 1){
        close(pipes[0]); //nothing more to be read.
    }

    return pipes[0]; //fix

}
