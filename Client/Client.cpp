#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "conio.h"
#include "../Common/ListaFIle.h"
#include "../Common/UsersList.h"
#include "../Common/ThreadStruct.h"
#include "../Common/SockAndPort.h"
#include "../Common/PortIndeksList.h"
#include "Threads.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define SERVER_IP_ADDRESS "127.0.0.1"
#define SERVER_PORT 27016

#define BUFFER_SIZE 256


// TCP client that use blocking sockets
int main()
{	
	


	SOCKET listenSocket = INVALID_SOCKET;

	// Socket used for communication with client
	

	// Variable used to store function return value
	int iResult;

	// Buffer used for storing incoming data
	

	// WSADATA data structure that is to receive details of the Windows Sockets implementation
	WSADATA wsaData;

	// Initialize windows sockets library for this process
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup failed with error: %d\n", WSAGetLastError());
		return 1;
	}


	// Initialize serverAddress structure used by bind
	sockaddr_in serverAddress2;
	memset((char*)&serverAddress2, 0, sizeof(serverAddress2));
	serverAddress2.sin_family = AF_INET;				// IPv4 address family
	serverAddress2.sin_addr.s_addr = INADDR_ANY;		// Use all available addresses
	serverAddress2.sin_port = htons(0);	// Use specific port


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
	iResult = bind(listenSocket, (struct sockaddr*) &serverAddress2, sizeof(serverAddress2));

	// Check if socket is successfully binded to address and port from sockaddr_in structure
	if (iResult == SOCKET_ERROR)
	{
		printf("bind failed with error: %d\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}
	int addrlen = sizeof(serverAddress2);
	if (getsockname(listenSocket, (struct sockaddr*)&serverAddress2, &addrlen) != SOCKET_ERROR) {
		//printf("port = %u\n", ntohs(serverAddress2.sin_port));
		
	}
	else {
		printf("Error");
	}
	int port = ntohs(serverAddress2.sin_port);

	




















	struct FileList* fl = NULL;
	
	// Socket used to communicate with server
	SOCKET connectSocket = INVALID_SOCKET;
	SOCKET acceptedSocket = INVALID_SOCKET;

	// Variable used to store function return value
	//int iResult;

	// Buffer we will use to store message
	char dataBuffer[BUFFER_SIZE];

	// WSADATA data structure that is to receive details of the Windows Sockets implementation
//	WSADATA wsaData;

	// Initialize windows sockets library for this process
//	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//	{
//		printf("WSAStartup failed with error: %d\n", WSAGetLastError());
	//	return 1;
//	}

	// create a socket
	connectSocket = socket(AF_INET,
		SOCK_STREAM,
		IPPROTO_TCP);

	if (connectSocket == INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	// Create and initialize address structure
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;								// IPv4 protocol	// ip address of server
	InetPton(AF_INET, "127.0.0.1", &serverAddress.sin_addr.s_addr);
	serverAddress.sin_port = htons(SERVER_PORT);					// server port

	// Connect to server specified in serverAddress and socket connectSocket
	if (connect(connectSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
	{
		printf("Unable to connect to server.\n");
		closesocket(connectSocket);
		WSACleanup();
		return 1;
	}

	
	struct SockAndPort sp;
	sp.port = port;
	sp.socket = &connectSocket;

	
	
	AcceptedSocketThreadClient(sp,&listenSocket);

	// Shutdown the connection since we're done

	iResult = shutdown(connectSocket, SD_BOTH);

	// Check if connection is succesfully shut down.
	if (iResult == SOCKET_ERROR)
	{
		printf("Shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(connectSocket);
		WSACleanup();
		return 1;
	}

	// For demonstration purpose
	printf("\nPress any key to exit: ");
	_getch();


	// Close connected socket
	closesocket(connectSocket);

	// Deinitialize WSA library
	WSACleanup();


	return 0;
		
			
}



bool InitializeWindowsSockets()
{
	WSADATA wsaData;
	// Initialize windows sockets library for this process
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup failed with error: %d\n", WSAGetLastError());
		return false;
	}
	return true;
}

