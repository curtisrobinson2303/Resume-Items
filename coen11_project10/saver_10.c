/*
********************************************************************************************
********************************************************************************************
*     Lab 10 - Curtis Robinson - November 18,2021     COEN 11 - Array of Linked Lists       *
* 	                                                                                       *		  
* 				LAB 10               	                                                   *
* 					                                                                       *
* This program runs a phone book where the user can add, delete, showAll entries,          *
* showCoressponding entries, write the list into a binary file, show the binary file,      *
* print the entries in reverse order, and quit the program.                                *
*                                                                                          *
* There is the added funcitonality of the 2nd thread that was added to run the sleep and   *
* auto save program on the second thread.                                                  *
*                                                                                          * 
********************************************************************************************
********************************************************************************************
*/
//Make sure memory is allocated for the pointer inside of the 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>
#include "variables_10.h"
#include<unistd.h>
#include<pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 
void *autoSaver(void *arg){
            char *string = (char *)arg;
            while(1){
                sleep(5);
                pthread_mutex_lock (&mutex);
                saveBinaryFile(string);
                pthread_mutex_unlock (&mutex); 
            }
            return NULL;
}


//argc is the number of arguments passed
// argv is the string itself
//arg is just the argument that is passed in the function
