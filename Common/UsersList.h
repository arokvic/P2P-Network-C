#pragma once
#include "pch.h"
#include "PortIndeksList.h"
#include <stdio.h>


typedef struct UserList
{
	int prviIndeks = NULL;
	int poslednjiIndeks = NULL;
	struct UserList* next = NULL;
	char fileName[15];
	int port = NULL;
};



void addUser(UserList** head, int prviInd, int poslInd, char* name, int port);
//char* find(UserList** head, char* fileName);
void deleteUser(int port, UserList** head);
void addUserPort(UserList ** head, int port);
void printPorts(UserList* head);
void addUserEnd(int port, UserList **head);
bool find(UserList** head, char* name, PortIndeksList** head2);
void addIndeks(UserList ** head, int port, int prvi, int poslednji, char * name);