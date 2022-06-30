#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "conio.h"

#include <iostream>
#include <conio.h> 

#include "../Common/ThreadStruct.h"
#include "../Common/Threads.h"
#include "../Common/UsersList.h"
#include "../Common/ListaFile.h"
#include "../Common/SockAndPort.h"
#include "../Common/PortIndeksList.h"
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define SERVER_PORT 27016
#define BUFFER_SIZE 256



// TCP server that use blocking sockets
int main()
{
	ThreadStruct1 *ts = (ThreadStruct1*)malloc(sizeof(ThreadStruct1*));
	//struct UserList* ul = NULL;
	struct FileList fll = {};
	struct UserList ull = {};
	//struct FileList* fllptr = NULL;
	UserList *ul = (UserList*)malloc(sizeof(UserList*));
	FileList *fl = (FileList*)malloc(sizeof(FileList*));
	fl = &fll;
	ul = &ull;

	//struct FileList* fl = NULL;
	//struct ThreadStruct1 ts;
	

	ts->head = fl;
	ts->head2 = ul;
	ReadFromFile(&fl);
	//print(ts->head);
	//print(&fll);
	
	//printf("%s\n", fl->name);
	//printf("%s", fl->value);
	//struct List l1;
	int m = 0;
	HANDLE handle[100];
	DWORD dword[100];

	// Socket used for listening for new clients 
	SOCKET listenSocket = INVALID_SOCKET;
	
	// Socket used for communication with client
	SOCKET acceptedSocket[100];
	for (int i = 0; i < 100; i++) {
		acceptedSocket[i] = INVALID_SOCKET;
	}

	// Variable used to store function return value
	int iResult;

	// Buffer used for storing incoming data
	char dataBuffer[BUFFER_SIZE];

	// WSADATA data structure that is to receive details of the Windows Sockets implementation
	WSADATA wsaData;

	// Initialize windows sockets library for this process
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup failed with error: %d\n", WSAGetLastError());
		return 1;
	}

	
	// Initialize serverAddress structure used by bind
	sockaddr_in serverAddress;
	memset((char*)&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;				// IPv4 address family
	serverAddress.sin_addr.s_addr = INADDR_ANY;		// Use all available addresses
	serverAddress.sin_port = htons(SERVER_PORT);	// Use specific port


	// Create a SOCKET for connecting to server
	listenSocket = socket(AF_INET,      // IPv4 address family
		SOCK_STREAM,  // Stream socket
		IPPROTO_TCP); // TCP protocol

// Check if socket is successfully created
	if (listenSocket == INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket - bind port number and local address to socket
	iResult = bind(listenSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress));

	// Check if socket is successfully binded to address and port from sockaddr_in structure
	if (iResult == SOCKET_ERROR)
	{
		printf("bind failed with error: %d\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}


	// neblokirajuci listen socket

	unsigned long mode = 1;
	if (ioctlsocket(listenSocket, FIONBIO, &mode) != 0)
	{
		printf("ioctlsocket failed with error.");
		return 1;
	}
	

	// Set listenSocket in listening mode
	iResult = listen(listenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
	{
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}


	

	printf("Server ceka klijente.\n");



	fd_set readfds;
	FD_ZERO(&readfds);

	timeval timeVal;
	timeVal.tv_sec = 1;
	timeVal.tv_usec = 0;

	FD_SET(listenSocket, &readfds);
	
	do
	{
		
		// Struct for information about connected client
		sockaddr_in clientAddr;

		int clientAddrSize = sizeof(struct sockaddr_in);

		 /*Accept new connections from clients */
		//acceptedSocket = accept(listenSocket, (struct sockaddr *)&clientAddr, &clientAddrSize);

		// Check if accepted socket is valid 


	/*	if (acceptedSocket[m] != INVALID_SOCKET)
		{
			FD_SET(acceptedSocket[m], &readfds);
			
		}*/

		

		iResult = select(0, &readfds, NULL, NULL, NULL);
		
	//	printf("\nNew client request accepted. Client address: %s : %d\n",  inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));


		if (iResult == 0) {
			// vreme za cekanje je isteklo
			Sleep(1000);
		}
		else if (iResult == SOCKET_ERROR) {
			//desila se greska prilikom poziva funkcije
			printf("select failed with error: %d\n", WSAGetLastError());
		
		}
		else {
			// rezultat je jednak broju soketa koji su zadovoljili uslov 


			if (FD_ISSET(listenSocket, &readfds)) {
				// izvrsenje operacije



				acceptedSocket[m] = accept(listenSocket, (struct sockaddr *)&clientAddr, &clientAddrSize);

				// Check if accepted socket is valid 
				if (acceptedSocket[m] == INVALID_SOCKET)
				{
					printf("accept failed with error: %d\n", WSAGetLastError());
					
					closesocket(listenSocket);
					WSACleanup();
					return 1;
				}

				
				ts->socket = &acceptedSocket[m];
				handle[m] = CreateThread(NULL, 0, &AcceptedSocketThread, ts, 0, &dword[m]);

				m++;
			}

		}

	} while (true);
	for (int i = 0; i < 100; i++) {

		CloseHandle(handle[i]);
	}

	//Close listen and accepted sockets
	closesocket(listenSocket);
	//closesocket(acceptedSocket);

	// Deinitialize WSA library
	WSACleanup();

	return 0;
}






