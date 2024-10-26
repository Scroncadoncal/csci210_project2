

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

//make directory
void mkdir(char pathName[]){

    // TO BE IMPLEMENTED
    //
    // YOUR CODE TO REPLACE THE PRINTF FUNCTION BELOW
    if (pathName == NULL || strlen(pathName) == 0) {
	    printf("MKDIR ERROR: no path provided\n");
	    return;
    }

    char baseName[64];
    char dirName[64];
    struct NODE* parentNode;

    parentNode = splitPath(pathName, baseName, dirName);

    if (parentNode == NULL) {
	    return;
    }

    struct NODE* child = parentNode->childPtr;
    while (child != NULL) {
	    if (strcmp(child->name, baseName) == 0) {
		    printf("MKDIR ERROR: directory %s already exists\n", baseName);
		    return;
	    }
    	    child = child->siblingPtr;
    }

    struct NODE* newDir = (struct NODE*)malloc(sizeof(struct NODE));
    if (newDir == NULL) {
	    perror("MKDIR ERROR: memory allocation failed");
	    return;
    }
    
    strncpy(newDir->name, baseName, 63);
    newDir->name[63] = '\0';
    newDir->fileType = 'D';
    newDir->childPtr = NULL;
    newDir->siblingPtr = NULL;
    newDir->parentPtr = parentNode;

    if (parentNode->childPtr == NULL) {
	    parentNode->childPtr = newDir;
    } else {
	    child = parentNode->childPtr;
	    while (child->siblingPtr != NULL) {
		    child = child->sibingPtr;
            }
	    child->siblingPtr = newDir;
    }

    printf("MKDIR SUCCESS: node %s successfully created\n", pathName);

    return;
}

//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName){

    // TO BE IMPLEMENTED
    // NOTE THAT WITHOUT COMPLETING THIS FUNCTION CORRECTLY
    // rm, rmdir, ls, cd, touch COMMANDS WILL NOT EXECUTE CORRECTLY
    // SEE THE PROVIDED EXECUTABLE TO SEE THEIR EXPECTED BEHAVIOR

    // YOUR CODE HERE
    if (pathName == NULL || strlen(pathName) == 0) {
	    dirName[0] = '\0';
	    strcpy(baseName, "");
	    return NULL;
    }

    struct NODE* currentNode = NULL;
    char* token;
    char* saveptr;
    char tempDir[64];
    char* pathCopy = strdup(pathName);
    dirName[0] = '\0';
    
    token = strtok_r(pathCopy, "/", &saveptr);
    while (token != NULL) {
	    struct NODE* child = currentNode->childPtr;
	    int found = 0;
	    while (child != NULL) {
		    if (strcmp(child->name, token) == 0) {
			    currentNode = child;
			    found = 1;
			    
			    if (strlen(dirName) > 0) {
				    strcat(dirName, "/");
			    }
			    strcat(dirName, token);
			    break;
		    }
		    child = child->siblingPtr;
	    }

	    if (!found) {
		    printf("ERROR: directory %s does not exist\n", token);
		    free(pathCopy);
		    return NULL;
	    }

	    token = strtok_r(NULL, "/", &saveptr);
    }

    strcpy(baseName, token);

    if (strlen(dirName) == 0) {
	    strcpy(dirName, "/");
    }

    // Printf for debugging
    printf("pathName = %s\n", pathName);
    printf("baseName = %s\n", baseName);
    printf("dirName = %s\n", dirName);
    return currentNode;
}
