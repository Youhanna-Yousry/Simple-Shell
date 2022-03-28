//
// Created by ubuntu on 3/22/22.
//

#ifndef LAB1_PARSERANDEVALUATOR_H
#define LAB1_PARSERANDEVALUATOR_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define MAX 100

int numOfWords = 0; //number of words (all arguments in user's command)
char **input_parsed; //Array of strings for parsed input

int numOfVariables = 0; //total number of saved variable
char data[MAX][2][MAX];  // saving both keys and values of variables data[][x][] ----> x=0 for keys / x=1 for values

bool command_is_not_exit = true;
bool foreground = true;

void replaceVariables(char *input, int *replacingIndex);

/**
 * Allocating an array of strings to hold the user's input after replacing the variables and after parsing
 * @return False in case of odd number of quotations and true otherwise
 */
bool parse_input(){
    int inCharCounter = 0, outCharCounter = 0, outArgCounter = 0,replacingIndex = 0;
    bool  quotationsFound = false;
    char input[MAX];
    input_parsed = (char** ) malloc(10* sizeof(char* ));
    for(int i = 0 ; i < 10 ; ++i){
        input_parsed[i] = (char*) malloc(MAX*sizeof (char));
    }
    fgets(input, 100, stdin);
    while(replacingIndex != -1 && input[replacingIndex] != '\0' && input[replacingIndex] != '\n'){
        replaceVariables(input, &replacingIndex);
    }
    while(true){
        while(input[inCharCounter] == ' ' && !quotationsFound){
            if (input[inCharCounter+1] == '"' || input[inCharCounter+1] == '\''){
                quotationsFound = !quotationsFound;
                ++inCharCounter;
            }
            ++inCharCounter;
        }
        if(input[inCharCounter] != '\n'){
            input_parsed[outArgCounter][outCharCounter] = input[inCharCounter];
            ++outCharCounter;
        }
        if(input[inCharCounter+1] == '\0' || input[inCharCounter+1] == '\n' || input[inCharCounter] == '\n' ||
                input[inCharCounter] == '\0'){
            input_parsed[outArgCounter][outCharCounter] = '\0';
            if(quotationsFound){
                printf("Make sure to close the quotations!\n");
                return false;
            } else if(input[inCharCounter-1] == '\'' || input[inCharCounter-1] == '\"'
                        || (input[inCharCounter-1] == ' ' && input[inCharCounter+1] != '\n')
                        || input[inCharCounter] == '\0'){
                break;
            }
            ++numOfWords;
            break;
        }
        else if(input[inCharCounter+1] == ' ' && !quotationsFound){
            input_parsed[outArgCounter][outCharCounter] = '\0';
            ++outArgCounter;
            outCharCounter = 0;
            ++numOfWords;
        }
        if (input[inCharCounter+1] == '"' || input[inCharCounter+1] == '\''){
            if(quotationsFound){
                ++numOfWords;
            }
            quotationsFound = !quotationsFound;
            ++inCharCounter;
        }
        ++inCharCounter;
    }
    input_parsed[numOfWords] = NULL;
    return true;
}

/**
 * Checking after parsing whether the command is a built-in one or no
 * Also setting foreground boolean if the command ends with &
 * @return false is case of built-in commands and true otherwise
 */
bool evaluate_input(){
    if(strcmp(input_parsed[numOfWords-1],"&") == 0){
        foreground = false;
    } else {
        foreground = true;
    }
    if(strcmp("export",input_parsed[0]) == 0 || strcmp("cd",input_parsed[0]) == 0 ||
            strcmp("echo",input_parsed[0]) == 0) {
        return true;
    } else if(strcmp("exit", input_parsed[0]) == 0){
        command_is_not_exit = false;
        return true;
    }
    return false;
}

/**
 * Deletes input[index] and shifts all other characters to the left
 * Called at replaced_variable(char *input)
 * @param input User's input string (before parsing)
 * @param index Character's index to be deleted
 */
void removeChar(char *input, int index){
    while(input[index] != '\0' && input[index] != '\n'){
        input[index] = input[index+1];
        ++index;
    }
    input[index-1] = '\0';
}

/**
 * Shifts all characters after the index (starting from the end) to the right and then adding
 * the insertedChar to input[index]
 * Called at replaced_variable(char *input)
 * @param input User's input string (before parsing)
 * @param index An integer that points to the addition location
 * @param insertedChar
 */
void addChar(char *input, int index, char insertedChar ){
    int stringLength = index;
    while(input[stringLength] != '\0'){
        ++stringLength;
    }
    while((stringLength+1) != index){
        input[stringLength+1] = input[stringLength];
        --stringLength;
    }
    input[index] = insertedChar;
}

/**
 * Replaces variables mentioned in User's input with their appropriate value
 * Called at parse_input()
 * @param input User's input string (before parsing)
 */
void replaceVariables(char *input, int *replacingIndex){
    int counter = *replacingIndex, variable_counter = 0;
    while(input[counter] != '$'){
        if(input[counter] == '\0'){
            *replacingIndex = -1;
            return;
        }
        ++counter;
    }
    removeChar(input, counter);
    char variable[MAX];
    while(input[counter] != '\0' && input[counter] != ' '
          && input[counter] != '\n' && input[counter] != '"' && input[counter] != '\''){
        variable[variable_counter] = input[counter];
        removeChar(input, counter);
        ++variable_counter;
    }
    variable[variable_counter] = '\0';
    for(int i = 0 ; i < numOfVariables ; ++i){
        if(strcmp(variable,data[i][0]) == 0){
            int counter2 = 0;
            while(data[i][1][counter2] != '\0'){
                addChar(input,counter,data[i][1][counter2]);
                ++counter2;
                ++counter;
            }
            *replacingIndex = counter;
            break;
        }
    }
}
#endif //LAB1_PARSERANDEVALUATOR_H