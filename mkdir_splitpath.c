#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

struct NODE* helper(struct NODE* parent, char* dirName) {
	struct NODE* currentNode = parent->childPtr;
	while (currentNode != NULL) {
		if (strcmp(currentNode->name, dirName) == 0) {
			return currentNode;
		}
		currentNode = currentNode->siblingPtr;
	}	
	return NULL;
}


// make directory
void mkdir(char pathName[]) {
	if (pathName == NULL || strlen(pathName) == 0) {
		printf("MKDIR ERROR: no path provided\n");
		return;
	}
	char baseName[64];
	char dirName[64];

	struct NODE* parentNode = splitPath(pathName, baseName, dirName);
	if (parentNode == NULL) {
		return;
	}
	
	if (strlen(baseName) == 0) {
		printf("MKDIR ERROR: no path provided\n");
		return;
	}
	
	struct NODE* existingDir = helper(parentNode, baseName);
	if (existingDir != NULL) {
		printf("MKDIR ERROR: directory %s already exists\n", baseName);
		return;
	}

	struct NODE* newDir = (struct NODE*)malloc(sizeof(struct NODE));
	if (newDir == NULL) {
		// perror("MKDIR: memory allocation failed\n");
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
		struct NODE* child = parentNode->childPtr;
		while (child->siblingPtr != NULL) {
			child = child->siblingPtr;
		}
		child->siblingPtr = newDir;
	}

	printf("MKDIR SUCCESS: node %s successfully created\n", pathName);

	return;
}

struct NODE* splitPath(char* pathName, char* baseName, char* dirName) {
	if (pathName == NULL || strlen(pathName) == 0) {
		dirName[0] = '\0';
		baseName[0] = '\0';
		return NULL;
	}

	char* dirEnd = strrchr(pathName, '/');

	if (dirEnd == NULL) {
		strcpy(baseName, pathName);
		dirName[0] = '\0';
		return cwd;
	}

	if (dirEnd == pathName) {
		strcpy(baseName, dirEnd + 1);
		dirName[0] = '/';
	} else {
		int offset = dirEnd - pathName;
		strncpy(dirName, pathName, offset);
		dirName[offset] = '\0';
		strcpy(baseName, dirEnd + 1);
	}

	struct NODE* currentNode = (pathName[0] == '/') ? root: cwd;
    	
	char* token = strtok(dirName, "/");
	while (token != NULL) {
		currentNode = helper(currentNode, token);
		if (currentNode == NULL) {
			printf("ERROR: directory %s does not exist\n", token);
			return NULL;
		}
		token = strtok(NULL, "/");
	}

	return currentNode;
}
