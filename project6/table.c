/*
 * File:        table.c
 * Author: by Curtis Robinson 
 * Modification: Implentation of a quick sort algorithm to efficiety sort.
 * 
 * 
 * 
 * Description: This file contains the public and private function and type
 *              definitions for a set abstract data type for generic
 *              pointer types.  A set is an unordered collection of unique
 *              elements.
 *
 *              This implementation uses a hash table to store the
 *              elements, with linear probing to resolve collisions.
 *              Insertion, deletion, and membership checks are all average
 *              case constant time.
 */

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <assert.h>
# include <stdbool.h>
# include "set.h"

# define EMPTY   0
# define FILLED  1
# define DELETED 2

struct set {
    int count;                  /* number of elements in array */
    int length;                 /* length of allocated array   */
    void **data;                /* array of allocated elements */
    char *flags;                /* state of each slot in array */
    int (*compare)();		/* comparison function         */
    unsigned (*hash)();		/* hash function               */
};


//Quicksort function definitions:
int partition(void **elts , int low, int high,int (*compare)());
void quicksort(void **elts, int low, int high,int (*compare)());


/*
 * Function:    search
 *
 * Complexity:  O(1) average case, O(n) worst case
 *
 * Description: Return the location of ELT in the set pointed to by SP.  If
 *		the element is present, then *FOUND is true.  If not
 *		present, then *FOUND is false.  The element is first hashed
 *		to its correct location.  Linear probing is used to examine
 *		subsequent locations.
 */

static int search(SET *sp, void *elt, bool *found)
{
    int available, i, locn, start;


    available = -1;
    start = (*sp->hash)(elt) % sp->length;

    for (i = 0; i < sp->length; i ++) {
        locn = (start + i) % sp->length;

        if (sp->flags[locn] == EMPTY) {
            *found = false;
            return available != -1 ? available : locn;

        } else if (sp->flags[locn] == DELETED) {
            if (available == -1)
		available = locn;

        } else if ((*sp->compare)(sp->data[locn], elt) == 0) {
            *found = true;
            return locn;
        }
    }

    *found = false;
    return available;
}


/*
 * Function:    createSet
 *
 * Complexity:  O(m)
 *
 * Description: Return a pointer to a new set with a maximum capacity of
 *		MAXELTS.
 */

SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)())
{
    int i;
    SET *sp;


    assert(compare != NULL && hash != NULL);

    sp = malloc(sizeof(SET));
    assert(sp != NULL);

    sp->data = malloc(sizeof(char *) * maxElts);
    assert(sp->data != NULL);

    sp->flags = malloc(sizeof(char) * maxElts);
    assert(sp->flags != NULL);

    sp->compare = compare;
    sp->hash = hash;
    sp->length = maxElts;
    sp->count = 0;

    for (i = 0; i < maxElts; i ++)
        sp->flags[i] = EMPTY;

    return sp;
}


/*
 * Function:    destroySet
 *
 * Complexity:  O(m)
 *
 * Description: Deallocate memory associated with the set pointed to by SP.
 *		The elements themselves are not deallocated since we did not
 *		allocate them in the first place.  That's the rule: if you
 *		didn't allocate it, then you don't deallocate it.
 */

void destroySet(SET *sp)
{
    assert(sp != NULL);

    free(sp->flags);
    free(sp->data);
    free(sp);
}


/*
 * Function:    numElements
 *
 * Complexity:  O(1)
 *
 * Description: Return the number of elements in the set pointed to by SP.
 */

int numElements(SET *sp)
{
    assert(sp != NULL);
    return sp->count;
}


/*
 * Function:    addElement
 *
 * Complexity:  O(1) average case, O(n) worst case
 *
 * Description: Add ELT to the set pointed to by SP.
 */

void addElement(SET *sp, void *elt)
{
    int locn;
    bool found;


    assert(sp != NULL && elt != NULL);
    locn = search(sp, elt, &found);

    if (!found) {
	assert(sp->count < sp->length);

	sp->data[locn] = elt;
	sp->flags[locn] = FILLED;
	sp->count ++;
    }
}


/*
 * Function:    removeElement
 *
 * Complexity:  O(1) average case, O(n) worst case
 *
 * Description: Remove ELT from the set pointed to by SP.  A element is
 *		deleted by changing the state of its slot.
 */

void removeElement(SET *sp, void *elt)
{
    int locn;
    bool found;


    assert(sp != NULL && elt != NULL);
    locn = search(sp, elt, &found);

    if (found) {
	sp->flags[locn] = DELETED;
	sp->count --;
    }
}


/*
 * Function:    findElement
 *
 * Complexity:  O(1) average case, O(n) worst case
 *
 * Description: If ELT is present in the set pointed to by SP then return
 *		it, otherwise return NULL.
 */

void *findElement(SET *sp, void *elt)
{
    int locn;
    bool found;


    assert(sp != NULL && elt != NULL);

    locn = search(sp, elt, &found);
    return found ? sp->data[locn] : NULL;
}


/*
 * Function:	getElements
 *
 * Complexity:	O(m)
 *
 * Description:	Allocate and return an array of elements in the set pointed
 *		to by SP.
 */



//Modify and implement a quicksort algorithm

// use the partition function and make the quicksort recursive 
void *getElements(SET *sp)
{
    assert(sp != NULL);
    int i, j;
    void **elts;
    

    elts = malloc(sizeof(void *) * sp->count);
    assert(elts != NULL);

    for (i = 0, j = 0; i < sp->length; i ++){
	    if (sp->flags[i] == FILLED){
	    elts[j ++] = sp->data[i];
        }
    }

    //Call of the quicksort function.
    quicksort(elts, 0, sp->count - 1, sp->compare);
    return elts;
}

/*
 * Function:	quickSort
 *
 * Complexity:	Big O(n^2)
 *
 * Description:	Splits the Array into smaller sections for sorting using recursion.
 *		
 */

void quicksort(void **elts, int low, int high,int (*compare)()){
    int pivotIndex;
    if(low < high)
    {
        pivotIndex = partition(elts, low, high, compare); // using the partition function the pivotIndex in inputted.
        //recursive call on the left pivot
        quicksort(elts, low, pivotIndex - 1, compare);
        //recursive call on the right pivot
        quicksort(elts, pivotIndex + 1, high, compare);
    }
    return;
}

/*
 * Function:	partition
 *
 * Complexity:	Big O(n) 
 *
 * Description:	Returns the number of the index where the partiotion took place. 
 *		
 */

int partition(void **elts, int low, int high, int(*compare)()) 
{
    //Necessary variable declarations.
    void*t;
    int a = low;
    int b = high;
    int pivot = low;
    while(a <= b)
    {
        while(compare(elts[a],elts[pivot]) <= 0 && a <= b)
        {
            a = a + 1;
        }
        while(compare(elts[pivot],elts[b]) < 0 && a <= b)
        {
            b = b - 1;
        }
        if(compare(elts[a],elts[b]) > 0 && a <= b)
        {
            //Swaping the values. 
            t = elts[a];
            elts[a] = elts[b];
            elts[b] = t;
        }
    }
    //Reseting the pivot.
    t = elts[pivot];
    elts[pivot] = elts[b];
    elts[b] = t;
    return b;
}
