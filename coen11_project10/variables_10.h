/*
************************************************************************************************
************************************************************************************************
* This is the variables.h file that holds all of the global variables and functinos            *
* declartaionswhich are utilizied in the files.c list.c main.c files.                	       * 
*                                      LAB 10                                                  *
************************************************************************************************
************************************************************************************************
*/

//These are the package import statements necessary to making the code work
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>
#include<pthread.h>

//This is the node structure which hold the variables nameArray and phoneNumArray which are for each person added to the phonebook.
typedef struct node{
	char nameArray[20];
	char phoneNumArray[20];
    struct node *next;
	struct node *previous;
}NODE;

//There are the global variables necessary to running the program.
extern char *dashString;
extern NODE *nodeArray[26];
extern NODE *nodeArrayTail[26];
extern pthread_mutex_t mutex; 

void showAll();
void insert(char inputName[20], char phoneNumArray[20], char inputLetter);
int duplicate(char inputName[20]);
void delete(char entryName[20], char currentLetter);
void showCorresponding(char inputLetter);
void readTextFile(char *storedInfo);
void saveTextFile(char *storedInfo);
void readBinaryFile(char *storedInfo);
void saveBinaryFile(char *storedInfo);
void reverse();
void recursion(NODE *inputNode, int index);
void *autoSaver(void *arg);

