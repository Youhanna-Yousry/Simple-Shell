//
// Created by ubuntu on 3/23/22.
//

#ifndef LAB1_MANAGER_H
#define LAB1_MANAGER_H
#include <signal.h>
#include <sys/resource.h>
#include "executor.h"

FILE *fp; //output.txt pointer

void printcwd();
void reset();

/**
 * Changing the directory to the current one
 */
void setup_environment(){
    char buffer[100];
    getcwd(buffer, 100);
    chdir(buffer);
}

/**
 * Reaping zombies and writing in output.txt file -> "Child has terminated"
 */
void on_child_exit(){
    waitpid(-1, NULL, WNOHANG);
    fp = fopen("/home/ubuntu/University/lab1/output.txt","a+");
    fprintf(fp,"Child has terminated\n");
    fclose(fp);
}

/**
 * A signal is sent when the child process terminates and the handler (fn on_child_exit()) is called
 */
void register_child_signal(){
    signal(SIGCHLD,on_child_exit);
}

void shell(){
    while(true){
        printcwd();
        if(parse_input()){ //in case of odd number of quotations the fn returns false
            if(evaluate_input()){ //in case of finding a non-built-in command the function return false
                if(!command_is_not_exit){
                    reset();
                    exit(0);
                }
                execute_shell_builtin();
            } else {
                execute_command();
            }
        }
        reset();
    }
}

/**
 * Prints the current working directory
 */
void printcwd(){
    char buffer[MAX];
    getcwd(buffer, MAX);
    printf("%s$ ", buffer);
}

/**
 * Releases the memory stored for parsed input after the end of each loop and after exiting
 */
void reset(){
    for(int i = 0 ; i < 10 ; ++i){
        free(input_parsed[i]);
    }
    free(input_parsed);
    numOfWords = 0;
}
#endif //LAB1_MANAGER_H