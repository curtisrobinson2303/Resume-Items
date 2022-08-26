/*
 *File Name:       dataset1.c
 * Author:         Curtis Robinson
 * Date:           March 9, 2022
 * Description:    This is application 1 for the term project that 
 * consists of the functions createDataSet(), destroyDataSet, searchAge,
 * Insertion, Deletion, MaxAgeGap.
 */

/*
Notes for the development of this program.
Implementation: 
    Data Set (dataset.c):
    – Define data structure
    – Define all functions
    – Search and delete:
    • Print out ID, age, and if found or deleted
    – maxAgeGap:
    • Print out the result
    – Hash table:
    • Prime number as the table length (1009)
*/

# include <stdio.h>
# include <stdlib.h>
# include <assert.h>
# include <stdbool.h>
# include <string.h>
# include "dataset1.h"

//Structre Definitions 
struct set{
    LIST **lists;
    int totalCount;
    int length;
    int oldest;
    int youngest;
};

struct node {
    int studentID;
    int age; 
    struct node *next;
    struct node *prev;
};

struct list{
    NODE *head; 
    int count;
};

/*
Function Name: createDataSet
Descprition: Creates a data set the can consist of students.
Runtime:  Big O(n)
*/
SET *createDataSet(int maxElts){
    SET *sp = malloc(sizeof(SET));
    assert(sp != NULL);
    assert(maxElts!=0);
    sp->totalCount = 0;
    sp->oldest = 0;
    sp->youngest = 0;
    sp->length = maxElts;
    sp->lists = malloc(sizeof(LIST*) * sp->length);
    int i;
    for( i = 0; i < sp->length; i++){
        LIST *lp;
        lp = malloc(sizeof(LIST));
        assert(lp != NULL);
        lp->count = 0;
        NODE *tp = malloc(sizeof(NODE));
        assert(tp!=NULL);
        lp->head = tp;
        tp->prev = tp;
        tp->next = tp;
        sp->lists[i] = lp;
        }
    return sp;
}

/*
Function Name: destoryDataSet
Descprition: Destorys the existing data set. 
Runtime:  Big O(n)
*/
void destroyDataSet(SET *sp){
    assert(sp != NULL);
    assert(sp->lists != NULL);
    int i;
    for(i = 0; i<sp->length; i++){
        LIST *lp = sp->lists[i];
        assert(lp != NULL);
        while(lp->count>0){
            NODE* tp = malloc(sizeof(NODE));
            assert(tp != NULL);
            assert(lp->head->next != NULL);
            tp = lp->head->next;
            lp->head->next = tp->next;
            tp->next->prev = lp->head;
            //void*temp = tp->data;
            free(tp);
            lp->count--;
        }
        free(lp->head);
        free(lp);
    }
    free(sp->lists);
    free(sp);
}

/*
Function Name: searchAge
Descprition: Searchs for the students with the corresponding age and prints to the console.
Runtime:  Big O(n)
*/
void searchAge(SET *sp, int age){
    assert(sp != NULL && age > 17);
    int index = age - 18;
    LIST* lp = sp->lists[index];
    assert(lp != NULL);

    printf("Searching for Students of Age %d...\n", age);
    if(lp->count == 0){
        printf("No Students of Age: %d\n", age);
        return;
    }
    NODE *pc = lp->head->next;
    int i;
    for(i = 0; i < lp->count; i++){
        printf("Student ID: #%d\n", pc->studentID);
        pc = pc->next;
    }
}

/*
Function Name: insertion
Descprition: Inserts new student entires to from the user and adds them to the set. 
Runtime:  Big O(1)
*/
void insertion(SET *sp, int studentID, int age){
    assert(sp != NULL && age > 17);
    assert(studentID != 0);
    assert(sp->totalCount < sp->length);
    int index = age - 18;
    // CHECKING FOR THE MAX AGE GAP
    if(sp->youngest == 0)
    {
        sp->youngest = age;
        sp->oldest = age;
    }
    if(age > sp->oldest)
    {
        sp->oldest = age;
    }
    else if(age < sp->youngest)
    {
        sp->youngest = age;
    }
    //ADDING ENTRY
    LIST *lp = sp->lists[index];
    assert(lp != NULL);
    NODE *np = malloc(sizeof(NODE));
    assert(np != NULL);
    np->studentID = studentID;
    np->age = age;
    printf("Student ID: #%d Student Age: %d\n", studentID, age);
    NODE *curr = lp->head->next;
    np->prev = lp->head;
    np->next = curr;
    curr->prev = np;
    lp->head->next = np;

    //ENTRY COUNTER
    lp->count++;
    sp->totalCount++;
    return;
}

/* 
Function Name: deletion
Descprition: Deletes an existing student based on the age and
if there are no students it will tell the user and return out of the function. 
Runtime:  Big O(n)
*/
void deletion(SET *sp, int age){
    assert(sp != NULL && age>17);
    int index = age - 18;
    assert(sp->totalCount > 0);
    assert(sp->lists != NULL);
    LIST *lp = sp->lists[index];
    assert(lp != NULL);

    if(lp->count == 0)
    {
        printf("Unable to delete. There are no students of Age: %d\n", age);
        return;
    }
    int tAge = age;
    if(tAge == sp->youngest && tAge == sp->oldest)
    {
        sp->youngest = 0;
        sp->oldest = 0;
    }
    else if(tAge == sp->youngest)
    {
        tAge++;
        while(sp->lists[age-18]->count == 0 && tAge <= 30)
        {
            tAge++;
        }
        sp->youngest = tAge;
    }
    else if(tAge == sp->oldest)
    {
        age--;
        while(sp->lists[tAge-18]->count==0 && tAge>=18)
        {
            tAge--;
        }
        sp->youngest = tAge;
    }
    NODE *tp;
    printf("Deleting Students of AGE %d...\n", age);
    while(lp->count != 0)
    {
        tp = malloc(sizeof(NODE));
        assert(tp != NULL);
        assert(lp->head->next != NULL);
        tp = lp->head->next;
        printf("Student ID: #%d\n", tp->studentID);
        lp->head->next = tp->next;
        tp->next->prev = lp->head;
        free(tp);
        lp->count--;
        sp->totalCount--;
    }
    printf("Deletion completed.\n");
    return;
}

/*
Function Name: maxAgeGap
Descprition: Finds the max age gap between 2 students in the given set. 
Runtime:  Big O(1)
*/
void maxAgeGap(SET *sp){
    int MAG = (sp->oldest - sp->youngest);
    printf("The Max Age Gap is: %d \n", MAG);
}








