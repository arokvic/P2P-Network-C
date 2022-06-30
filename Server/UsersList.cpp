#include "UsersList.h"
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

struct UserList *head;

void addUser(UserList** head, int prviInd, int poslInd, char* name, int port)
{
	UserList* newItem = (UserList*)malloc(sizeof(UserList));
	strcpy_s(newItem->fileName, MAXBUFF, name);
	newItem->poslednjiIndeks = poslInd;
	newItem->prviIndeks = prviInd;
	newItem->port = port;
	newItem->next = NULL;
	if (*head == NULL)
	{
		*head = newItem;
	}
	else
	{
		UserList* first = *head;
		while (first->next != NULL)
		{
			first = first->next;
		}
		first->next = newItem;
		newItem->next = NULL;
	}
}

/*char* find(FileList** head, char* name)
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
}*/

void deleteUser(int port, UserList** head)
{

	UserList* temp = *head;
	UserList* prev = NULL;

	// If head node itself holds 
	// the key to be deleted 
	if (temp != NULL && temp->port == port)
	{
		*head = temp->next; // Changed head 
		delete temp;            // free old head 
		return;
	}

	// Else Search for the key to be deleted,  
	// keep track of the previous node as we 
	// need to change 'prev->next' */ 
	while (temp != NULL && temp->port != port)
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
}






