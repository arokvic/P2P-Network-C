#include "pch.h"
#include "PortIndeksList.h"
#include "UsersList.h";
#include "ClientSend.h";

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
CRITICAL_SECTION csssss;

void popuniClientSend(ClientSend* c, PortIndeksList* head) {
	InitializeCriticalSection(&csssss);
	EnterCriticalSection(&csssss);
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

	LeaveCriticalSection(&csssss);
	DeleteCriticalSection(&csssss);

}