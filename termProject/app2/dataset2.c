/*
 *File Name:       dataset2.c
 * Author:         Curtis Robinson
 * Date:           March 9, 2022
 * Description:    This is application 1 for the term project that 
 * consists of the functions createDataSet(), destroyDataSet, searchID,
 * Insertion, Deletion.
 */

# include <stdio.h>
# include <stdlib.h>
# include <assert.h>
# include <stdbool.h>
# include <string.h>
# include "dataSet2.h"

//Structre Definitions 
struct set{
    int totalEntries;
    int studentData[3000];
};

/*
Function Name: createDataSet
Descprition: Creates a data set the can consist of students.
Runtime:  Big O(n)
*/
SET *createDataSet(int maxElts){
    //allocate memory for the new set 
    SET *sp = malloc(sizeof(SET));
    //Check that there is a set and that the student range is not 0.
    assert(sp != NULL);
    assert(maxElts != 0);

    sp->totalEntries = maxElts;
    int i ;
    for(int i = 0; i< maxElts; i++){
            sp->studentData[i] = 0;
    }
    return sp;
}

/*
Function Name: destoryDataSet
Descprition: Destorys the existing data set. 
Runtime:  Big O(n)
*/
void destroyDataSet(SET *sp){
    //Check that there is a set.
    assert(sp != NULL);

    //Free the memeory the set was taking up 
    free(sp);

}

/*
Function Name: searchAge
Descprition: Searchs for the students with the corresponding ID and prints to the console.
Runtime:  Big O(n)
*/
void searchID(SET *sp, int studentID){
    //Check that there is a set and that the student ID is no out of range.
    assert((sp != NULL) && (studentID > 0));


    printf("Searching for Student with ID #%d...\n", studentID);
    if(sp->totalEntries == 0){
        printf("There are no students of with the StudentID: #%d\n", studentID);
        return;
    }
    else if(sp->studentData[studentID+1] == 0){
        printf("The student does not exist.\n");
        return;
    }
    else{
        printf("Here is the data of the student. StudentID: %d  Age: %d...\n", studentID, sp->studentData[studentID + 1]);
    }
}

/*
Function Name: insertion
Descprition: Inserts new student entires to from the user and adds them to the set. 
Runtime:  Big O(1)
*/
void insertion(SET *sp, int studentID, int age){
    //Check that there is a set.
    assert(sp != NULL);

    if(studentID == 0){
        printf("The entry cannot be added. Student ID is out of allowed range.\n");
        return;
    }
    else {
        sp->studentData[studentID + 1] = age;
    }
    return;
}


/* 
Function Name: deletion
Descprition: Deletes an existing student based on their studentID and
if there are no students it will tell the user and return out of the function. 
Runtime:  Big O(n)
*/
void deletion(SET *sp, int studentID){
    //Check that there is a set and that the student ID is no out of range.
    assert(sp != NULL && studentID != 0);


    printf("Deleting Student with StudentID %d...\n", studentID);
    sp->studentData[studentID + 1 ] = 0;
    printf("Deletion completed.\n");
    return;
}









