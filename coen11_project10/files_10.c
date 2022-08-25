/*
************************************************************************************************
************************************************************************************************
* This is the files.c file that holds all of the necessary functions allowing the phonebook to * 
* read and write the phonebooks information to be saved in binary and text files.              * 
*                                                                                              *
*                                         LAB 10                                               *
*                                                                                              *
************************************************************************************************
************************************************************************************************
*/

// These are the package import statements necessary to making the code work
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
// This is used for the case compare function
#include<strings.h>
#include "variables_10.h"
// If there is no previous file, then the program will create a new file for the program. 
void readTextFile(char *storedInfo){
        char newName[20];
        char newNum[20];
        char tempChar = newName[0];
        FILE *readingFile = fopen(storedInfo, "r");
        if (readingFile == NULL)
        {
                printf("The text file does not exist and will be created.\n");
                return;
        }
        printf("The file was read successfully\n");
        fseek(readingFile,strlen(dashString),SEEK_SET);
        while(fscanf(readingFile,"%s%s",newName, newNum) == 2 )
        {
                tempChar = newName[0];
                insert(newName, newNum, tempChar);			
        }
        fclose(readingFile);
}
void readBinaryFile(char *storedInfo)
{
    FILE *readingFile = fopen(storedInfo, "rb");
    if (readingFile == NULL)
	{
		    printf("The binary file does not exist and will be created.\n");
	 	    return;
	}
    NODE *u;
    if ((u = (NODE *)malloc(sizeof(NODE))) == NULL)
    {
            printf("Malloc error \n");
            exit(1);
    }
    printf("The file was read successfully\n");
    // this while loops adds the current list to the binary list 
    while(fread(u, sizeof(NODE), 1, readingFile) > 0 )
	{
            printf ("The name is: %s ", u->nameArray);
            printf("and the phone number is: %s \n ", u->phoneNumArray);
	}
	fclose(readingFile);
}

// This fuction saves the information into text file.
void saveTextFile(char *storedInfo){
        FILE *writingFile = fopen(storedInfo, "w");
        if(writingFile == NULL){
                printf("That file cannot be saved\n");
                return;	
        } 
        printf("The file was saved successfully");
        fprintf(writingFile, "%s",dashString); // this is printing the header into the file each time the file is saved 
        //Declaration of the temporary node *u.
        NODE *u; 
        int i;
        for(i = 0; i< 26;i++)
        {
                u = nodeArray[i];
                while(u!=NULL)
                {
                    fprintf(writingFile,"%s\t%s \n",u->nameArray, u->phoneNumArray);
                    u = u->next;
                }
        }
        //This closes the text file that is being writien into.
        fclose(writingFile);
}	
//Function for saving the binary file
void saveBinaryFile(char *storedInfo){
    FILE *writingFile = fopen(storedInfo, "wb");
    //Checking to see is the file cannot be saves or does not exist.
	if(writingFile == NULL){
		printf("That file cannot be saved\n");
		return;	
	} 
	printf("The file was saved successfully");
    //Declaration of the temporary node u and allocating the memory for the node
    NODE *u; 
    if ((u = (NODE *)malloc(sizeof(NODE))) == NULL)
    {
        printf("Malloc error \n"); 
        exit(1);
    }
	int i;
    // for loop to iterate throughout the nodes and the prints
	for(i = 0; i < 26;i++){
		    u = nodeArray[i];
            while(u!=NULL)
            {
                fwrite(u, sizeof(NODE), 1, writingFile);
                u = u->next;
            }
    }
    fclose(writingFile);
}
