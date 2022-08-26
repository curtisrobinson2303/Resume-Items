/*
 * File Name:      dataset2.h
 * Author:         Curtis Robinson
 * Date:           March 9, 2022
 * Description: This is the header file that contains the structer
 * and function definitions used in the dataset2.c and college2.c files. 
 */

//Essential Structure Definitions:
typedef struct set SET;

//Esential Function Definitions:
SET *createDataSet(int maxElts);
extern void destroyDataSet(SET *sp);
extern void searchID(SET *sp, int studentID);
extern void insertion(SET *sp, int studentID, int age);
extern void deletion(SET *sp, int age);
