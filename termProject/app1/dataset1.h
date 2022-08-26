/*
 * File Name:      dataset1.h
 * Author:         Curtis Robinson
 * Date:           March 9, 2022
 * Description: This is the header file that contains the structer
 * and function definitions used in the dataset.c and college.c files. 
 */

# ifndef LIST_H
# define LIST_H

//Essential Structure Definitions:
typedef struct list LIST;
typedef struct set SET;
typedef struct node NODE;
//Esential Function Definitions:
SET *createDataSet(int maxElts);
extern void destroyDataSet(SET *sp);
extern void searchAge(SET *sp, int age);
extern void insertion(SET *sp, int studentID, int age);
extern void deletion(SET *sp, int age);
extern void maxAgeGap(SET *sp);

# endif 