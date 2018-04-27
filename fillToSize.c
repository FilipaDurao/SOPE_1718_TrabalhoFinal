#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* fillToSize(char* value, int maxSize){

    unsigned int counter = strlen(value);
    char* answer = malloc(maxSize*sizeof(char));

    for(unsigned int i = counter; i>0; i--){
        strcat(answer, "0");    
    }
    strcat(answer, value);   
    return answer;
}
