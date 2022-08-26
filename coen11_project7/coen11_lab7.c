/*
*********************************************
* Lab 7 - Curtis Robinson - October 28,2021 *
* 	COEN 11 - Array of Linked Lists         *		  
* 					                        *
* 					                        *
* This program adds people to a phone book  *
* as well as allowing the user to delete,   *
* show, and add people to the phonebook.    *
*********************************************
*/

// These are the package import statements necessary to making the code work
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// This is the node structure which hold the variables nameArray and phoneNumArray which are for each person added to the phonebook.
typedef struct node{
	char nameArray[20];
	char phoneNumArray[20];
    struct node *p;
}NODE;

// This is the initialization of the functions and variables necessary to running the program.
NODE *nodeArray[26];
NODE *search (int);
void showAll();
void insert(char inputName[20], char phoneNumArray[20], char inputLetter);
int duplicate(char inputName[20]);
void delete(char entryName[20], char currentLetter);
void showCorresponding(char inputLetter);
void realFile();
void saveFile();



// Main method which hold the switch statement which allows the user to choose the 1-4 options.
int main(void){
        
        readFile();
        writeFile();
        if(argc == 1)
        {   
            printf("Error the name of the file is missing / he name of the file is missing!\n");
            exit(1);
        }
        else{
            
            readFile(argv[1]);
            int selectorValue = 0;
            char currentLetter;
            char letterFinder;
            while(1) {
                    printf(" -------- Options 1. Insert    2. Delete         3. Show All Entries        4. Show Corresponding Entry        5. Quit -------- ");
                    scanf("%d",&selectorValue);
                    char inputName[20];
                    char inputNum[20];
                    switch(selectorValue){
                            case 1:
                                            printf("Enter the Name: \n");
                                            scanf("%s",inputName);
                                            currentLetter = inputName[0];
                                            printf("\nPlease enter phone number: \n");
                                            scanf("%s",inputNum);
                                            printf("\n");
                                            insert(inputName, inputNum, currentLetter);
                                    break;				
                            case 2:
                                            
                                            printf("Enter the Name of the person that you want to delete: ");
                                            scanf("%s",inputName);
                                            letterFinder = inputName[0];
                                            printf("\n");
                                            delete(inputName,letterFinder);
                                    break;
                            case 3:
                                            showAll();
                                    break;
                            case 4:
                                            printf("Enter the Letter of the phonebook you want to show: ");
                                            scanf("%s",inputName);
                                            letterFinder = inputName[0];
                                            printf("\n");
                                            showCorresponding(letterFinder);
                                    break;
                            case 7: // This case quits the file and saves the current information into the phonebook file
                                            prinft("Phonebook saved!");
                                            void saveFile();
                                            printf("Quit successfully!");
                                            return 0;
                            default:
                                            printf("Please re-enter a number between 1-4.");
                                            printf("\n");
                                            }
                    }
                    free(nodeArray[1]);
        }
}

// this function inserts an entry in to the phonebook
void insert(char inputName[20], char phoneNumArray[20], char inputLetter)
{
    //this finds the index needed to input the node into nodeArray
    int value  = inputLetter - 'a';
    NODE *temp, *prev, *new_node;

    if (((temp = (NODE *)malloc(sizeof(NODE))) == NULL) || ((prev = (NODE *)malloc(sizeof(NODE))) == NULL) || ((new_node = (NODE *)malloc(sizeof(NODE))) == NULL))
    {
        printf("Malloc error \n"); 
        exit(1);
    }
        strcpy(new_node->nameArray, inputName);
        strcpy(new_node->phoneNumArray, phoneNumArray);
        temp = nodeArray[value];

    if (nodeArray[value] == NULL || strcmp(temp->nameArray, new_node->nameArray) > 0) 
    {
        new_node->p = nodeArray[value]; 
        nodeArray[value] = new_node; 
        return;
    }
    while (temp->p != NULL && (strcmp(temp->nameArray, new_node->nameArray) < 0)) 
    {
            prev = temp;
            temp = temp->p;
    } 
    if (temp->p == NULL && (strcmp(temp->nameArray, new_node->nameArray) < 0)) {
        new_node->p = temp->p; 
        temp->p = new_node;
        return;
    }
    
    if (strcmp(temp->nameArray, new_node->nameArray) > 0){
        prev->p = new_node; 
        new_node->p = temp;
        return;
    } 
    
    if (temp != NULL && (strcmp(inputName, temp->nameArray) == 0)){
        printf("Sorry, this name and/or number is already in the phonebook.\n");
        return;
    }

}

// This function deletes entries from the phone book 
void delete(char entryName[20], char inputLetter) {
    //this finds the index needed to input the node into nodeArray
    int value  = inputLetter - 'a';

	NODE *temp;
        // prev is the previous node and t is the current one 
        NODE *t, *prev;
        temp = nodeArray[value];
        if(nodeArray[value] == NULL){
                printf("The list is empty");
                return;
        }
        if(strcmp(temp->nameArray, entryName)==0){
                nodeArray[value] = temp->p;
                return;
        }
        while(temp->p != NULL && strcmp(temp->nameArray, entryName) < 0){
                prev = temp;
                temp = temp->p;    
        }
        if(temp == NULL){
                printf("Error...No name was found.");
                return;
        }
        prev->p = temp->p;
        free(temp);
}

// The show() function displays the information of each entry that is in the phone book in total.
void showAll(){  
        NODE *u;    
        int i;
        for(i = 0; i<26;i++){
            u = nodeArray[i];
                while(u != NULL){
                    printf ("The name is: %s ", u->nameArray);
                    printf("and the phone number is: %s \n ", u->phoneNumArray);
                    u = u->p;
                }
        }
}

// This function shows the array with the letter that the user inupts. All of the phonebook inputs.
void showCorresponding(char inputLetter){  
        //this finds the index needed to input the node into nodeArray
        int value  = inputLetter - 'a';
        NODE *u;
        if (nodeArray[value] == NULL){   
                printf ("The list is empty.\n ");
        }
        u = nodeArray[value];
        while(u != NULL){
                printf ("The name is: %s ", u->nameArray);
                printf("and the phone number is: %s \n ", u->phoneNumArray);
                u = u->p;
        }
    
}
void realFile(){
        FILE *readingFile = fopen("storedInformation", "r");
}
void saveFile(){
        FILE *writingFile = fopen("storedInformation", "w");
    
}