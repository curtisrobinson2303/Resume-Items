/*
************************************************************************************************
************************************************************************************************
*  This is the list.c file that holds all of the necessary function that allows the phonebook  * 
*  to run smoothly. In this file the following functions exist:                                *
*                                                                                              *
*  insert(), delete(), showAll(), showCorresponding(), reverse(), recursion().                 * 
*                                                                                              *
*                                 LAB 10                                                       *
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
#include<pthread.h>

// this function inserts an entry in to the phonebook
void insert(char inputName[20], char phoneNumArray[20], char inputLetter)
{

        int value;
        //this finds the index needed to input the node into nodeArray
        if(inputLetter >= 'A' && inputLetter<= 'Z'){
            value  = inputLetter - 'A';
        }
        else if(inputLetter >= 'a' && inputLetter<= 'z'){
            value  = inputLetter - 'a';
        }
        else{
            printf("The character is invalid!");
            return;
        }
    //this finds the index needed to input the node into nodeArray
    
    NODE *temp, *prev, *new_node;

    if (((temp = (NODE *)malloc(sizeof(NODE))) == NULL) || ((prev = (NODE *)malloc(sizeof(NODE))) == NULL) || ((new_node = (NODE *)malloc(sizeof(NODE))) == NULL))
    {
        printf("Malloc error \n"); 
        exit(1);
    }
    // strcpy --  copies the name and the phone number into the new_node variables for nameArray and phoneNumArray
    strcpy(new_node->nameArray, inputName);
    strcpy(new_node->phoneNumArray, phoneNumArray);
    // makes temp at the spot in the total node array for the specific alphabetical locaiton
    temp = nodeArray[value];
    //checks to see if there is a linked list at the specific spot in the array   
    if (nodeArray[value] == NULL ) 
    {
                new_node->previous = new_node->next = NULL;
                nodeArray[value] =  nodeArrayTail[value] = new_node; 
                return;
    }
    else if(strcasecmp(temp->nameArray, new_node->nameArray) > 0)
    {
                new_node->next = nodeArray[value];
                new_node->previous = NULL;
                new_node->next->previous = new_node;
                nodeArray[value] = new_node;
                return;
    }
    while (temp->next != NULL && (strcasecmp(temp->nameArray, new_node->nameArray) < 0)) 
    {
                temp = temp->next;
    } 
    if (temp->next == NULL && (strcasecmp(temp->nameArray, new_node->nameArray) < 0)) 
    {
                new_node->previous = temp;
                temp->next = new_node;
                new_node->next = NULL;
                nodeArrayTail[value] = new_node;
                return;
    }
    else if (strcasecmp(temp->nameArray, new_node->nameArray) > 0)
    {
                new_node->next = temp;
                new_node->previous = temp->previous;
                temp->previous->next = new_node; 
                temp->previous = new_node;
                return;
    } 
    // If statement check to see if there is a duplicate in the list. If there is it will print that the entry is already in the phonebook.
    else if (temp != NULL && (strcasecmp(inputName, temp->nameArray) == 0))
    {
                printf("Sorry, this name and/or number is already in the phonebook.\n");
                return;
    }
}
// This function deletes entries from the phone book 
void delete(char entryName[20], char inputLetter)
{
        NODE *temp, *t;
        int value;
        //this finds the index needed to input the node into nodeArray
        if(inputLetter >= 'A' && inputLetter<= 'Z'){
            value  = inputLetter - 'A';
        }
        else if(inputLetter >= 'a' && inputLetter<= 'z'){
            value  = inputLetter - 'a';
        }
        else{
            printf("The character is invalid!");
            return;
        }
        temp = nodeArray[value];
        if(nodeArray[value] == NULL)
        {
                printf("The list is empty. \n");
                return;
        }
        if(temp->next == NULL && strcasecmp(temp->nameArray, entryName) == 0)
        {
                nodeArray[value]  =  nodeArrayTail[value]= NULL;
                return;
        }
        if(strcmp(temp->nameArray, entryName)==0)
        {
                nodeArray[value] = temp->next;
                nodeArray[value]->previous = NULL;
                return;
        }
        while(temp->next != NULL && strcmp(temp->nameArray, entryName) < 0)
        {
                temp = temp->next;    
        }
        if(temp->next == NULL)
        {
                nodeArrayTail[value] = temp->previous;
                nodeArrayTail[value]->next = NULL;
                return;
        }
        else
        {
                temp->previous->next = nodeArrayTail[value];
                nodeArrayTail[value]->previous  = temp->previous;
                return;
        }
        if(temp == NULL)
        {
                printf("Error... No name was found. \n");
                return;
        }
        temp->previous->next = temp->next;
        free(temp);
}
// The show() function displays the information of each entry that is in the phone book in total.
void showAll()
{  
        NODE *u;    
        int i;
        for(i = 0; i<26;i++)
        {
                u = nodeArray[i];
                while(u != NULL)
                {
                        printf ("The name is: %s ", u->nameArray);
                        printf("and the phone number is: %s \n ", u->phoneNumArray);
                        u = u->next;
                }
        }
}
// This function shows the array with the letter that the user inupts. All of the phonebook inputs.
void showCorresponding(char inputLetter)
{  
        NODE *u;
        int value;
        //this finds the index needed to input the node into nodeArray
        if(inputLetter >= 'A' && inputLetter<= 'Z'){
            value  = inputLetter - 'A';
        }
        else if(inputLetter >= 'a' && inputLetter<= 'z'){
            value  = inputLetter - 'a';
        }
        else
	{
            printf("The character is invalid please re-enter a letter");
            return;
        }
        if(nodeArray[value] == NULL)
        {   
                printf("The list is empty.\n ");
        }
        u = nodeArray[value];
        while(u != NULL)
        {
                printf ("The name is: %s ", u->nameArray);
                printf("and the phone number is: %s \n ", u->phoneNumArray);
                u = u->next;
        }
    
}
// reverse methods for the linked lists 
void reverse(){
        NODE *u;
        int i;
        for(i = 25; i>=0;i--)
        {
                //reverse the linked list
                recursion(nodeArray[i],i);
                //Output the reversed list names
                u = nodeArray[i];
                while(u != NULL)
                {
                        printf ("The name is: %s ", u->nameArray);
                        printf("and the phone number is: %s \n ", u->phoneNumArray);
                        u = u->next;
                }
                recursion(nodeArray[i],i);
        }
}
void recursion(NODE *inputNode, int index)
{
        NODE *temp;
        if(inputNode == NULL)
        {
                return;
        }
        temp = inputNode->previous;
        inputNode->previous = inputNode->next;
        inputNode->next = temp;
        recursion(inputNode->previous,index);
        if(inputNode->previous == NULL){
                nodeArray[index] = inputNode;
        }
}
