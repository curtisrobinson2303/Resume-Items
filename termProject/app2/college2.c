/*
File Name:      college2.c
Author:         Curtis Robinson
Date:           March 9, 2022
Description:    This is the main driver files which creates the data set and takes the users file input.
*/

# include <stdio.h>
# include <stdlib.h>
# include <assert.h>
# include <time.h>
# include "dataSet2.h"

// Definition: Max number of students allows in the Set.
# define MAX_STUDENTS 3000

/*
* Function: main
* Description: This main function creates a new data set and then adds entries to the set. It uses the searchAge 
* MaxAgeGap and destroyDataSet functions to fufill this users intentions. 
*/

int main(int argc, char *argv[]){ 
    SET *college = createDataSet(MAX_STUDENTS);
    int newID, age, i, j;
    srand(time(0));
    for(i = 0; i < 1000; i++){
        age = (rand() % 13) + (18);
        j = (rand() % 2) + (1); 
        newID  = newID + j;
        insertion(college, newID, age);
    }
    age = (rand() % 13) + (18);
    newID = (rand() % 2100);
    searchID(college, newID);

    newID = (rand() % 2100);
    deletion(college, newID);
    destroyDataSet(college);
}









