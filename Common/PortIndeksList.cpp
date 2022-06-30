#include "pch.h"
#include "PortIndeksList.h"
#include "UsersList.h";


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

CRITICAL_SECTION csss;

void addPortIndeks(PortIndeksList** head, int prviInd, int poslInd, int port)
{
	InitializeCriticalSection(&csss);
	EnterCriticalSection(&csss);
	PortIndeksList* newItem = (PortIndeksList*)malloc(sizeof(PortIndeksList));
	newItem->zadnjiIndeks = poslInd;
	newItem->prviIndeks = prviInd;
	newItem->port = port;
	newItem->next = NULL;
	if (*head == NULL)
	{
		*head = newItem;
	}
	else
	{
		PortIndeksList* first = *head;
		while (first->next != NULL)
		{
			first = first->next;
		}
		first->next = newItem;
		newItem->next = NULL;
	}
	LeaveCriticalSection(&csss);
	DeleteCriticalSection(&csss);
}

void printPortIndeksList(PortIndeksList* head) {

	PortIndeksList *current_node = head;
	while (current_node != NULL) {
		printf("%d \n", current_node->port);
		printf("%d \n", current_node->prviIndeks);
		printf("%d \n", current_node->zadnjiIndeks);

		current_node = current_node->next;
	}

}

int IzracunajIndekse(PortIndeksList* head, int clSize) {
	int ret = clSize;
	PortIndeksList *current_node = head;
	while (current_node != NULL) {
		if (current_node->next == NULL) {
			ret += current_node->zadnjiIndeks;
			return ret;
		}

		current_node = current_node->next;
	}


}


int IzracunajbrojElemenata(PortIndeksList* head) {
	int ret = 0;
	PortIndeksList *current_node = head;
		while (current_node != NULL) {
			ret++;

			current_node = current_node->next;
		}

		return ret - 1;
	}

int* VratiPortIndkse(PortIndeksList* head,int x) {
	int ret[3];
	int i = 0;
	PortIndeksList *current_node = head;
	while (current_node != NULL && i<x ) {
		i++;

		current_node = current_node->next;
	}
	ret[0] = current_node->port;
	ret[1] = current_node->prviIndeks;
	ret[2] = current_node->zadnjiIndeks;

	return ret;
}


char* serijalizacijaListe(PortIndeksList* head, int brEl) {
	int size = sizeof(int) + sizeof(int) + sizeof(int);
	char* mess = (char*)malloc(size * brEl);
	int i = 0;

	PortIndeksList *current_node = head;
	current_node = current_node->next;
	while (current_node != NULL) {
		*((int*)mess + i) = current_node->port;
		i++;
		*((int*)mess + i) = current_node->prviIndeks;
		i++;
		*((int*)mess + i) = current_node->zadnjiIndeks;
		i++;



		current_node = current_node->next;
	}
	return mess;

}

/*
void popuniC2C(c2c* c, PortIndeksList* head) {

	int i = 0;

	PortIndeksList *current_node = head;
	current_node = current_node->next;
	while (current_node != NULL) {
		c->ports[i] = current_node->port;
		c->prviIndeksi[i] = current_node->prviIndeks;
		c->zadnjIndeksi[i] = current_node->zadnjiIndeks;

		i++;
		current_node = current_node->next;
	}

}*/

bool proveraSviKodKlijenta(PortIndeksList* head, int duzinaFajla) {
	bool ret = false;
	PortIndeksList *current_node = head;
	current_node = current_node->next;
	while (current_node != NULL) {
		int zadnjiIndeks = current_node->zadnjiIndeks;
		if (zadnjiIndeks >= duzinaFajla - 1)
			ret = true;


		current_node = current_node->next;
	}
	return ret;


}


void deleteNode(PortIndeksList** head_ref, int port)
{
	InitializeCriticalSection(&csss);
	EnterCriticalSection(&csss);
	// Store head node 
	PortIndeksList* temp = *head_ref;
	PortIndeksList* prev = NULL;

	// If head node itself holds 
	// the key to be deleted 
	if (temp != NULL && temp->port == port)
	{
		*head_ref = temp->next; // Changed head 
		delete temp;   
		LeaveCriticalSection(&csss);// free old head 
		DeleteCriticalSection(&csss);
		return;
	}


	while (temp != NULL && temp->port != port)
	{
		prev = temp;
		temp = temp->next;
	}

	// If key was not present in linked list 
	if (temp == NULL) {
		LeaveCriticalSection(&csss);
		DeleteCriticalSection(&csss);
		return;
	}

	// Unlink the node from linked list 
	prev->next = temp->next;

	// Free memory 
	delete temp;
	LeaveCriticalSection(&csss);
	DeleteCriticalSection(&csss);
}












