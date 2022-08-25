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
#include<pthread.h>
#include<unistd.h> 
//The Char array dashsting is the header in the text file. 
char *dashString = "Phonebook \n -------------------------\n";
NODE *nodeArray[26];
NODE *nodeArrayTail[26];
 

/**************/
//possiblity that we did not omake the header or tail = null
/**************/

// Main method which hold the switch statement which allows the user to choose the 1-4 options.
int main(int argc, char *argv[]){
        if(argc == 1)
        {   
                printf("Error the name of the file is missing \n");
                exit(1);
        }
//This else-statement's purpose is to create a text file whenever argc != 1 (aka when the file alread exists).
        else
        {   
//reading of the text file called by readTextFile(argv[1]); 
                readTextFile(argv[1]);
	}
                pthread_t autoSaverThread;
                pthread_create(&autoSaverThread,NULL,autoSaver, (void *) argv[2]);
        
// selectorValue varible keeps track of the user input for the switch method.
        int selectorValue = 0;
//Current letter is the variable used for the first letter of the name.
        char letterFinder;
//While loop makes it possible for the user to continue to add things to the phonebook.
        while(1) 
        {
                printf("\n -------- Options 1. Insert   2. Delete   3. Show All Entries   4. Show Corresponding Entry   5. Write List into Binary File   6. Print Binary File into Text   7. Print the Phone Book in Reverse Order.  8. Quit The Program -------- ");
                scanf("%d",&selectorValue);
                char inputName[20];
                char inputNum[20];
                switch(selectorValue){
                        case 1:
                        //Creates a new entry in the phonebook using the insert functino which take a name and a phone number as the parameter.

                                printf("Enter the Name: \n");
                                scanf("%s",inputName);
                                 char currentLetter = inputName[0];
                                printf("\nPlease enter phone number: \n");
                                scanf("%s",inputNum);
                                printf("\n");
                                pthread_mutex_lock (&mutex);
                                insert(inputName, inputNum, currentLetter);
                                pthread_mutex_unlock (&mutex);
                                break;	
                        //Deletes the corresponding entry from the linked list using the inputted name to find the entry.			
                        case 2:
                                printf("\nEnter the Name of the person that you want to delete: ");
                                scanf("%s",inputName);
                                letterFinder = inputName[0];
                                printf("\n");
                                pthread_mutex_lock (&mutex);
                                delete(inputName,letterFinder);
                                pthread_mutex_unlock (&mutex);
                                break;
                            //Shows all of the elements of the linked list in alphabetical order.
                        case 3:
                                showAll();
                                break;
                            //Show the linked list based on the correspoding letting inputted by the user.
                        case 4:
                                printf("\nEnter the Letter of the phonebook you want to show: ");
                                scanf("%s",inputName);
                                letterFinder = inputName[0];
                                printf("\n");
                                showCorresponding(letterFinder);
                                break;
                            //Write the current linked list into the Binary File.
                        case 5:
                                saveBinaryFile(argv[2]);   
                                break;
                            //Print Binary File content into human readable text in console.
                        case 6:
                                printf("Printing content from the binary file: \n");
                                readBinaryFile(argv[2]);
                                break;

                        case 7: 
                                printf("Printing the reverse string:\n");
                                reverse();
                                printf("\nReversed list has been printed.\n");
                                break;
                        case 8: // This case quits the file and saves the binary and test file
                                printf("Phonebook saved!");
                                pthread_mutex_lock (&mutex);
                                pthread_cancel(autoSaverThread);
                                pthread_mutex_unlock (&mutex);
                                saveTextFile(argv[1]);
                                return 0;

                        // This is the default case that requests the user to re-enter a number 
                        default:
                                printf("Please re-enter a number from 1-8.");
                                printf("\n");
                }
        }
        free(nodeArray[1]);
}
