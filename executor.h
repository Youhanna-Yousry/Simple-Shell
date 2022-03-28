//
// Created by ubuntu on 3/23/22.
//

#ifndef LAB1_EXECUTOR_H
#define LAB1_EXECUTOR_H
#include "parserAndEvaluator.h"
#include <unistd.h>
#include <sys/wait.h>

void handleExport();

/**
 * Covering built-in shell cases --> cd/cd ~/cd ../cd relative path/cd absolute path/echo/export
 */
void execute_shell_builtin(){
    if(numOfWords < 3){
        if((numOfWords == 1 && ((strcmp("cd",input_parsed[0]) == 0)))
                || ((strcmp("cd",input_parsed[0]) == 0) && (strcmp("~",input_parsed[1]) == 0)) ){ //case 1:
            chdir(getenv("HOME"));                                                     //cd //cd~
        } else if(strcmp("cd",input_parsed[0]) == 0){ //case 3: cd relative path / absolute path
            if(chdir(input_parsed[1]) == -1){
                printf("bash: cd: %s: No such file or directory\n",input_parsed[1]);
            }
        } else if(strcmp("echo",input_parsed[0]) == 0){ //case 4: echo
            if(numOfWords == 1){
                printf("\n");
            } else {
                printf("%s\n",input_parsed[1]);
            }
        } else if(numOfWords == 2) { //case 5: export
            handleExport();
        }
        return;
    }
    printf("bash: %s: Too many arguments\n", input_parsed[0]);
}

/**
 * Forking for executing non built-in commands (running a child process)
 */
void execute_command() {
    pid_t child_id;
    if ((child_id = fork()) == 0) { //creating a child process and assigning its id to child_id
        execvp(input_parsed[0], input_parsed);
        printf("Error\n");
        exit(EXIT_FAILURE);
    } else if (foreground) {
        waitpid(child_id,NULL, 0);
    }
}

/**
 * Checks the validity of export command then saves the key and value tuple to the data[][][] array
 * In case of pre-defined key, its value is replaced with the new one
 * Called at execute_shell_builtin()
 */
void handleExport(){
    int counter1 = 0;
    int counter2 = 0;
    char key[MAX];
    char value[MAX];

    while(input_parsed[1][counter1] !=  '='){
        if(input_parsed[1][counter1] == '\0'){
            printf("Invalid command!\n");
            return;
        }
        key[counter1] = input_parsed[1][counter1];
        ++counter1;
        ++counter2;
    }
    key[counter1] = '\0';

    counter1 = 0;
    ++counter2;

    while(input_parsed[1][counter2] !=  '\0'){
        value[counter1] = input_parsed[1][counter2];
        ++counter1;
        ++counter2;
    }
    value[counter1] = '\0';

    counter1 = 0;
    counter2 = 0;

    for(int i = 0 ; i < numOfVariables ; ++i){
        if(strcmp(data[i][0], key) == 0){
            while(value[counter1] != '\0' && value[counter1] != '\n'){
                data[i][1][counter1] = value[counter1];
                ++counter1;
            }
            data[i][1][counter1] = '\0';
            return;
        }
    }

    while(key[counter1] != '\0'){
        data[numOfVariables][0][counter1] = key[counter1];
        ++counter1;
    }
    data[numOfVariables][0][counter1] = '\0';

    while(value[counter2] != '\0' && value[counter2] != '\n'){
        data[numOfVariables][1][counter2] = value[counter2];
        ++counter2;
    }
    data[numOfVariables][1][counter2] = '\0';
    ++numOfVariables;
}
#endif //LAB1_EXECUTOR_H