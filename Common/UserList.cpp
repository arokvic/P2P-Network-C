#include "pch.h"
#include "UsersList.h"
#include "PortIndeksList.h"

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

//struct UserList *head;


void addUserPort(UserList ** head, int port) {
	
	UserList* newItem = (UserList*)malloc(sizeof(UserList));
	newItem->port = port;
	//strcpy(newItem->fileName, "aaaaa");
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

void addIndeks(UserList ** head, int port, int prvi, int poslednji,char * name) {
	
	UserList* first = *head;
	while (first)
	{
		if (first->port == port)
		{
			first->prviIndeks = prvi;
			first->poslednjiIndeks = poslednji;
			strcpy(first->fileName, name);

		}
		first = first->next;
	}
	
	

}



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


void deleteUser(int port, UserList** head)
{
	
	UserList* temp = *head;
	UserList* prev = NULL;

	// If head node itself holds 
	// the key to be deleted 
	if (temp != NULL && temp->port == port)
	{
		*head = temp->next; // Changed head 
		delete temp;    
		
		
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

void printPorts(UserList* head) {

	UserList *current_node = head;
	while (current_node != NULL) {
		printf("%d \n", current_node->port);
		printf("%d \n", current_node->prviIndeks);
		printf("%d \n", current_node->poslednjiIndeks);
		printf("%s \n", current_node->fileName);

		current_node = current_node->next;
	}

}


void addUserEnd(int port, UserList **head)
{
	
	struct UserList *newNode = (UserList*)malloc(sizeof(struct UserList));
	newNode->port = port;
	newNode->next = NULL;

	//if head is NULL, it is an empty list
	if (*head == NULL)
		*head = newNode;
	//Otherwise, find the last node and add the newNode
	else
	{
		struct UserList *lastNode = *head;

		//last node's next address will be NULL.
		while (lastNode->next != NULL)
		{
			lastNode = lastNode->next;
		}

		//add the newNode at the end of the linked list
		lastNode->next = newNode;
	}
	
}

bool find(UserList** head, char* name, PortIndeksList** head2)
{
	bool ret = false;
	//struct PortIndeksList ret;
	//struct PortIndeksList * ptr;
	//ptr = &ret;


	UserList* first = *head;
	while (first)
	{
		if (!strcmp(first->fileName, name))
		{
			addPortIndeks(head2, first->prviIndeks, first->poslednjiIndeks, first->port);
			ret = true;

		}
		first = first->next;
	}
	return ret;
}










