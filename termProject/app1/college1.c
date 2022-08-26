/*
File Name:      college1.c
Author:         Curtis Robinson
Date:           March 9, 2022
Description:    This is the main driver files which creates the data set and takes the users file input.
*/

/*
Notes for the edevelopment of this program.
Implementation: 
            Driver program (college.c):
        – Main function
        – Create your data set
        – Generate student records and insert
        – Search
        – Deletion
        – Get max age gap for scenario 1 and 3
        – Destroy data set
*/

# include <stdio.h>
# include <stdlib.h>
# include <assert.h>
# include <time.h>
# include <stdbool.h>
# include "dataset1.h"

// Definition: Max number of students allows in the Set.
# define MAX_STUDENTS 3000

/*
* Function: main
* Description: This main function creates a new data set and then adds entries to the set. It uses the searchAge 
* MaxAgeGap and destroyDataSet functions to fufill this users intentions. 
*/

int main(int argc, char *argv[]){ 
    
    printf("Debugging.........");
    SET *college = createDataSet(MAX_STUDENTS);
    int newID, age, i, j;
    srand(time(0));
    for(i = 0; i < 1000; i++){
        age = (rand() % 13) + (18);
        j = (rand() % 2000) + (1); 
        newID  = newID + j;
        insertion(college, newID, age);
    }


    //Test Cases 
    int tAge = (rand() % 13) + (18);
    int tID = (rand() % 2000) + (1);
    insertion(college, tID, tAge);
    
    searchAge(college, tAge);
    maxAgeGap(college);
    destroyDataSet(college);
}









