#include "pch.h"
#include "ListaFile.h"
#include <stdlib.h>
#define WIN32_LEAN_AND_MEAN
#define MAXBUFF 99
#include <stdio.h>
#include "conio.h"
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment (lib, "AdvApi32.lib")
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")

//struct FileList *head;
CRITICAL_SECTION cssss;
void addToTheBegining(FileList** head, char* value, char* name)
{
	InitializeCriticalSection(&cssss);
	EnterCriticalSection(&cssss);

	FileList* newItem = (FileList*)malloc(sizeof(FileList));
	strcpy_s(newItem->value, MAXBUFF, value);
	strcpy_s(newItem->name, 15, name);
	newItem->next = NULL;
	if (*head == NULL)
	{
		*head = newItem;
	}
	else
	{
		FileList* first = *head;
		while (first->next != NULL)
		{
			first = first->next;
		}
		first->next = newItem;
		newItem->next = NULL;
	}

	//printf("%s %s", newItem->name, newItem->value);
	LeaveCriticalSection(&cssss);
	DeleteCriticalSection(&cssss);
}

void print(FileList *head) {
	FileList *current_node = head;
	while (current_node != NULL) {
		printf("%s %s", current_node->name, current_node->value);

		current_node = current_node->next;
	}
}

bool fileExists(FileList* head, char* name) {
	


	FileList* current = head; // Initialize current  
	while (current != NULL)
	{
		if (!strcmp(current->name, name))
			return true;
		current = current->next;
	}
	return false;



}


char* findFileVal(FileList** head, char* name)
{
	char success[MAXBUFF];
	FileList* first = *head;
	while (first)
	{
		if (!strcmp(first->name, name))
		{
			strcpy_s(success, MAXBUFF, first->value);
			break;
		}
		first = first->next;
	}
	return success;
}
/*
void deleteNode(char* name, FileList** head)
{

	FileList* temp = *head;
	FileList* prev = NULL;

	// If head node itself holds 
	// the key to be deleted 
	if (temp != NULL && !strcmp(temp->name, name))
	{
		*head = temp->next; // Changed head 
		delete temp;            // free old head 
		return;
	}

	// Else Search for the key to be deleted,  
	// keep track of the previous node as we 
	// need to change 'prev->next' 
	while (temp != NULL && strcmp(temp->name, name) != 0)
	{
		prev = temp;
		temp = temp->next;
	}

	// If key was not present in linked list 
	if (temp == NULL)
		return;

	// Unlink the node from linked list 
	prev->next = temp->next;

	// Free memory 
	delete temp;


	/*
	struct FileList *iter;

	iter = head;


   if (iter == NULL) {
	   printf("not found\n");
	   return;
   }

   if (iter->value == value) {
	   printf("found in first element: %i\n", value);
	   head = head->next;
	   return;
   }

   while (iter->next != NULL) {
	   if (iter->next->value == value) {
		   printf("deleting element: %i\n", value);
		   iter->next = iter->next->next;
		   return;
	   }

	   iter = iter->next;
   }

   printf("not found\n");
}*/


void ReadFromFile(FileList** head) {
	int x = 0;
	char name[50];
	char val[50];
	char const* const fileName = "fajlovi.txt"; /* should check that argc > 1 */
	FILE* file;
	fopen_s(&file, fileName, "r"); /* should check the result */
	char line[256];

	while (fgets(line, sizeof(line), file)) {
		char *token = strtok(line, " ");
		while (token != NULL) {
			if (x == 0) {
				strcpy(name, token);
				x++;
				token = strtok(NULL, " ");
			}
			else if (x == 1) {
				strcpy(val, token);
				x++;
				token = strtok(NULL, " ");
			}


		}

		x = 0;
		addToTheBegining(head, val, name);


	}
	/* may check feof here to make a difference between eof and io failure -- network
	   timeout for instance */

	   //	printf("%s\n", name);
	   //	printf("%s\n", val);
	fclose(file);



}
