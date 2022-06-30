#include "../Common/ThreadStruct.h"
#include "../Common/UsersList.h"
#include "../Common/ListaFile.h"
#include "../Common/SockAndPort.h"
#include "../Common/PortIndeksList.h"
#include "../Common/SockAndVal.h"
#include "../Common/ClientSend.h"
#include <time.h>
#include "Threads.h"
#include <stdlib.h>
#define WIN32_LEAN_AND_MEAN
#define BUFFER_SIZE 256
#define MAXBUFF 99
#include <stdio.h>
#include "conio.h"
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment (lib, "AdvApi32.lib")
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
//CRITICAL_SECTION css;


DWORD WINAPI ClientServerThread(LPVOID lpParam) {
	char ClientStr[MAXBUFF];
	
	Sleep(1000);
//	SOCKET listenSocket = INVALID_SOCKET;
	int m = 0;
	HANDLE handle[100];
	DWORD dword[100];
	struct SockAndVal *sv = (struct SockAndVal *)lpParam;
	strcpy(ClientStr, sv->val);
	printf("Deo fajla koji ostaje kod mene: %s\n", ClientStr);
	printf("\n\n*************************************************************************\n\n");
	//printf("%s", ClientStr);

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




	// neblokirajuci listen socket


	SOCKET *listenSocket = sv->socket;

	unsigned long mode = 1;
	if (ioctlsocket(*listenSocket, FIONBIO, &mode) != 0)
	{
		printf("ioctlsocket failed with error.");
		return 1;
	}


	// Set listenSocket in listening mode
	iResult = listen(*listenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
	{
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(*listenSocket);
		WSACleanup();
		return 1;
	}




	printf("Klijent ceka druge klijente da se povezu.\n");



	fd_set readfds;
	FD_ZERO(&readfds);

	timeval timeVal;
	timeVal.tv_sec = 1;
	timeVal.tv_usec = 0;

	FD_SET(*listenSocket, &readfds);

	do
	{

		// Struct for information about connected client
		sockaddr_in clientAddr;

		int clientAddrSize = sizeof(struct sockaddr_in);

	


		iResult = select(0, &readfds, NULL, NULL, NULL);

			//printf("\nNew client request accepted.");


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



			
				// izvrsenje operacije



				acceptedSocket[m] = accept(*listenSocket, (struct sockaddr *)&clientAddr, &clientAddrSize);

				// Check if accepted socket is valid 
				if (acceptedSocket[m] == INVALID_SOCKET)
				{
					printf("accept failed with error: %d\n", WSAGetLastError());
					closesocket(*listenSocket);
					WSACleanup();
					return 1;
				}
				printf("Klijent se povezao.\n");
				SelectWrite(acceptedSocket + m);
				//printf("%d", strlen(ClientStr));
				iResult = send(*(acceptedSocket+m), ClientStr, strlen(ClientStr), 0);

				// Check result of send function
				if (iResult == SOCKET_ERROR)
				{
					printf("send failed with error: %d\n", WSAGetLastError());
					closesocket(*(acceptedSocket + m));
					WSACleanup();
					return 1;
				}




				//struct ThreadStruct1 ts;
				//ts.head = fl;
			//	ts.socket = &acceptedSocket[m];
			//	handle[m] = CreateThread(NULL, 0, &AcceptedSocketThreadClient, &acceptedSocket[m], 0, &dword[m]);

				m++;
			

		}

	} while (true);

	// Shutdown the connection since we're done
/*	iResult = shutdown(acceptedSocket, SD_BOTH);

	// Check if connection is succesfully shut down.
	if (iResult == SOCKET_ERROR)
	{
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(acceptedSocket[m]);
		WSACleanup();
		return 1;
	}*/

	//Close listen and accepted sockets
	closesocket(*listenSocket);
	//closesocket(acceptedSocket);

	// Deinitialize WSA library
	WSACleanup();

	return 0;




}


void AcceptedSocketThreadClient(struct SockAndPort sp,SOCKET* listenSocket) {
	printf("Povezan na server\n");
	struct SockAndVal sv;
	HANDLE handle[2];
	DWORD dword[2];
	struct PortIndeksList ps;
	struct PortIndeksList* psp;
	struct ClientSend cs ;
	
	psp = &ps;
	//struct SockAndPort sp = *(SockAndPort *)lpParam;
	char dataBuffer[BUFFER_SIZE];
	SOCKET *connectSocket = sp.socket;
	int iResult;
	char ClientServerString[256];
	
	SelectWrite(connectSocket);
	int clSpace;
	clSpace = SendPortAndSpace(sp.port, connectSocket);
	printf("Imam %d bajtova memorije\n", clSpace);
	while (true) {

		



		printf("Unesite naziv fajla koji zelite da skinete:\n");
		gets_s(dataBuffer, BUFFER_SIZE);

		if (strcmp(dataBuffer, "close server") == 0)
		{
			return ;
		}

		SelectWrite(connectSocket);
		// Send message to server using connected socket
		iResult = send(*connectSocket, dataBuffer, (int)strlen(dataBuffer), 0);

		// Check result of send function
		if (iResult == SOCKET_ERROR)
		{
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(*connectSocket);
			WSACleanup();
			return ;
		}


		Sleep(200);/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		SelectRead(connectSocket);
		iResult = recv(*connectSocket, dataBuffer, BUFFER_SIZE, 0);

		if (iResult > 0)	{// Check if message is successfully received
		


			dataBuffer[iResult] = '\0';

			if (!strcmp(dataBuffer, "Trazeni fajl ne postoji")) {
				printf("Trazeni fajl ne postoji\n");
				
			}
			else {


				if (!strcmp("1", dataBuffer)) {	//PRVI KOJI SKIDA FAJL

					SelectRead(connectSocket);
					iResult = recv(*connectSocket, dataBuffer, BUFFER_SIZE, 0);



					if (iResult > 0) {

						dataBuffer[iResult] = '\0';
						printf("fajl postoji, njegova vrednost je : %s\n", dataBuffer);

						for (int i = 0; i < clSpace; ++i)
						{
							ClientServerString[i] = dataBuffer[i];
						}
						ClientServerString[clSpace] = '\0';
						//	printf("\n%s", ClientServerString);

						strcpy(sv.val, ClientServerString);
						sv.socket = listenSocket;
						handle[0] = CreateThread(NULL, 0, &ClientServerThread, &sv, 0, &dword[0]);
						CloseHandle(handle[0]);
						while (true) {}


						break;
					}
					else if (iResult == 0)	// Check if shutdown command is received
					{
						// Connection was closed successfully
						printf("Connection with client closed.\n");
						closesocket(*connectSocket);
					}
					else	// There was an error during recv
					{

						printf("recv failed with error: %d\n", WSAGetLastError());
						closesocket(*connectSocket);
						//SelectSend(&acceptedSocket);


						iResult = shutdown(*connectSocket, SD_BOTH);

						// Check if connection is succesfully shut down.
						if (iResult == SOCKET_ERROR)
						{
							printf("shutdown failed with error: %d\n", WSAGetLastError());
							closesocket(*connectSocket);
							WSACleanup();
							return;
						}

						//Close listen and accepted sockets

						closesocket(*connectSocket);

						// Deinitialize WSA library
						WSACleanup();

					}



				}
				else if (!strcmp("2", dataBuffer)) {	//IMA KLIJENATA KOJI SU SKIDALI FAJL 
						
					
						SelectRead(connectSocket);
						iResult = recv(*connectSocket, dataBuffer, BUFFER_SIZE, 0);

						if (iResult > 0) {



							//printf("fajl postoji, njegova vrednost je : %d", dataBuffer);


							int brKl = *(int*)dataBuffer;
							//	printf("Server sent: %d.\n", brKl);
							int sizeOfMess = (sizeof(int) + sizeof(int) + sizeof(int)) * brKl;

							SelectRead(connectSocket);
							iResult = recv(*connectSocket, dataBuffer, sizeOfMess, 0);
							if (iResult > 0) {
								int i = 1;
								int j = 0;
								while (i <= brKl) {
									//						int x = *((int*)dataBuffer + j);
														//printf("----%d\n", x);
												//		int y = *((int*)dataBuffer + j + 1);
													//	printf("----%d\n", y);
													//	int z = *((int*)dataBuffer + j + 2);
														//printf("----%d\n", z);

									addPortIndeks(&psp, *((int*)dataBuffer + j + 2), *((int*)dataBuffer + j + 1), *((int*)dataBuffer + j));

									j = j + 3;
									i++;

								}
								//printPortIndeksList(psp);
								Sleep(400);

								SelectRead(connectSocket);
								iResult = recv(*connectSocket, dataBuffer, BUFFER_SIZE, 0);
								if (iResult > 0) {
									//	printf("%s", dataBuffer);
									char str[BUFFER_SIZE];
									strcpy(str, dataBuffer);
									for (int i = 0; i < clSpace; ++i)
									{
										ClientServerString[i] = dataBuffer[i];
									}
									ClientServerString[clSpace] = '\0';
									//		printf("\n%s", ClientServerString);
									strcpy(sv.val, ClientServerString);
									sv.socket = listenSocket;
									handle[0] = CreateThread(NULL, 0, &ClientServerThread, &sv, 0, &dword[0]);
									strcpy(cs.value, str);
									cs.brKl = brKl;

									popuniClientSend(&cs, &ps);

									handle[1] = CreateThread(NULL, 0, &Client2Clients, &cs, 0, &dword[1]);
									CloseHandle(handle[1]);


									while (true) {}


								}
								else if (iResult == 0)	// Check if shutdown command is received
								{
									// Connection was closed successfully
									printf("Connection with client closed.\n");
									closesocket(*connectSocket);
								}
								else	// There was an error during recv
								{

									printf("recv failed with error: %d\n", WSAGetLastError());
									closesocket(*connectSocket);
									//SelectSend(&acceptedSocket);





									iResult = shutdown(*connectSocket, SD_BOTH);

									// Check if connection is succesfully shut down.
									if (iResult == SOCKET_ERROR)
									{
										printf("shutdown failed with error: %d\n", WSAGetLastError());
										closesocket(*connectSocket);
										WSACleanup();
										return;
									}

									//Close listen and accepted sockets

									closesocket(*connectSocket);

									// Deinitialize WSA library
									WSACleanup();





								}



								break;
							}
							else if (iResult == 0)	// Check if shutdown command is received
							{
								// Connection was closed successfully
								printf("Connection with client closed.\n");
								closesocket(*connectSocket);
							}
							else	// There was an error during recv
							{

								printf("recv failed with error: %d\n", WSAGetLastError());
								closesocket(*connectSocket);
								//SelectSend(&acceptedSocket);


								iResult = shutdown(*connectSocket, SD_BOTH);

								// Check if connection is succesfully shut down.
								if (iResult == SOCKET_ERROR)
								{
									printf("shutdown failed with error: %d\n", WSAGetLastError());
									closesocket(*connectSocket);
									WSACleanup();
									return;
								}

								//Close listen and accepted sockets

								closesocket(*connectSocket);

								// Deinitialize WSA library
								WSACleanup();

							}

						}
						else if (iResult == 0)	// Check if shutdown command is received
						{
					// Connection was closed successfully
						printf("Connection with client closed.\n");
						closesocket(*connectSocket);
						}
						else	// There was an error during recv
						{

						printf("recv failed with error: %d\n", WSAGetLastError());
						closesocket(*connectSocket);
						//SelectSend(&acceptedSocket);


						iResult = shutdown(*connectSocket, SD_BOTH);

						// Check if connection is succesfully shut down.
						if (iResult == SOCKET_ERROR)
						{
							printf("shutdown failed with error: %d\n", WSAGetLastError());
							closesocket(*connectSocket);
							WSACleanup();
							return;
						}

						//Close listen and accepted sockets

						closesocket(*connectSocket);

						// Deinitialize WSA library
						WSACleanup();

						}
			}
		else {	//FAJL JE CEO KOD KLIJENATA
						SelectRead(connectSocket);
						iResult = recv(*connectSocket, dataBuffer, BUFFER_SIZE, 0);
						if (iResult > 0) {




							//printf("fajl postoji, njegova vrednost je : %d", dataBuffer);


							int brKl = *(int*)dataBuffer;
						//	printf("Server sent: %d.\n", brKl);
							int sizeOfMess = (sizeof(int) + sizeof(int) + sizeof(int)) * brKl;

							SelectRead(connectSocket);
							iResult = recv(*connectSocket, dataBuffer, sizeOfMess, 0);
							if (iResult > 0) {
								int i = 1;
								int j = 0;
								while (i <= brKl) {
									//						int x = *((int*)dataBuffer + j);
														//printf("----%d\n", x);
												//		int y = *((int*)dataBuffer + j + 1);
													//	printf("----%d\n", y);
													//	int z = *((int*)dataBuffer + j + 2);
														//printf("----%d\n", z);
									
									addPortIndeks(&psp, *((int*)dataBuffer + j + 2), *((int*)dataBuffer + j + 1), *((int*)dataBuffer + j));
									
									j = j + 3;
									i++;

								}
								//printPortIndeksList(psp);
								Sleep(400);
								char str[BUFFER_SIZE];
								strcpy(str, "\0");
								strcpy(cs.value, str);
								cs.brKl = brKl;
								
								popuniClientSend(&cs, &ps);
								
								handle[1] = CreateThread(NULL, 0, &Client2Clients, &cs, 0, &dword[1]);
								CloseHandle(handle[1]);


							}
							else if (iResult == 0)	// Check if shutdown command is received
							{
								// Connection was closed successfully
								printf("Connection with client closed.\n");
								closesocket(*connectSocket);
							}
							else	// There was an error during recv
							{

								printf("recv failed with error: %d\n", WSAGetLastError());
								closesocket(*connectSocket);
								//SelectSend(&acceptedSocket);





								iResult = shutdown(*connectSocket, SD_BOTH);

								// Check if connection is succesfully shut down.
								if (iResult == SOCKET_ERROR)
								{
									printf("shutdown failed with error: %d\n", WSAGetLastError());
									closesocket(*connectSocket);
									WSACleanup();
									return;
								}

								//Close listen and accepted sockets

								closesocket(*connectSocket);

								// Deinitialize WSA library
								WSACleanup();

							}
						}

						else if (iResult == 0)	// Check if shutdown command is received
						{
							// Connection was closed successfully
							printf("Connection with client closed.\n");
							closesocket(*connectSocket);
						}
						else	// There was an error during recv
						{

							printf("recv failed with error: %d\n", WSAGetLastError());
							closesocket(*connectSocket);
							//SelectSend(&acceptedSocket);





							iResult = shutdown(*connectSocket, SD_BOTH);

							// Check if connection is succesfully shut down.
							if (iResult == SOCKET_ERROR)
							{
								printf("shutdown failed with error: %d\n", WSAGetLastError());
								closesocket(*connectSocket);
								WSACleanup();
								return;
							}

							//Close listen and accepted sockets

							closesocket(*connectSocket);

							// Deinitialize WSA library
							WSACleanup();

						}
			}





			// Log message text
			






			}
			

			//strcpy(ts.name, dataBuffer);
		//	handle = CreateThread(NULL, 0, &FindThread, &ts, 0, &dword);
			


		}
		else if (iResult == 0)	// Check if shutdown command is received
		{
			// Connection was closed successfully
			printf("Connection with client closed.\n");
			closesocket(*connectSocket);
		}
		else	// There was an error during recv
		{

			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(*connectSocket);
			//SelectSend(&acceptedSocket);





			iResult = shutdown(*connectSocket, SD_BOTH);

			// Check if connection is succesfully shut down.
			if (iResult == SOCKET_ERROR)
			{
				printf("shutdown failed with error: %d\n", WSAGetLastError());
				closesocket(*connectSocket);
				WSACleanup();
				return ;
			}

			//Close listen and accepted sockets

			closesocket(*connectSocket);

			// Deinitialize WSA library
			WSACleanup();





		}	}}



void SelectRead(SOCKET* acceptedSocket) {

	unsigned long mode = 1;
	if (ioctlsocket(*acceptedSocket, FIONBIO, &mode) != 0)
	{
		printf("ioctlsocket failed with error.");
		return;
	}

	fd_set readfds;
	FD_ZERO(&readfds);
	timeval timeVal;
	timeVal.tv_sec = 0;
	timeVal.tv_usec = 0;
	int iResult;

	FD_SET(*acceptedSocket, &readfds);



	char dataBuffer[BUFFER_SIZE];

	while (true) {
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
			
			return;
		}
	}



}void SelectWrite(SOCKET* acceptedSocket) {

	unsigned long mode = 1;
	if (ioctlsocket(*acceptedSocket, FIONBIO, &mode) != 0)
	{
		printf("ioctlsocket failed with error.");
		return;
	}

	fd_set readfds;
	FD_ZERO(&readfds);
	timeval timeVal;
	timeVal.tv_sec = 0;
	timeVal.tv_usec = 0;
	int iResult;

	FD_SET(*acceptedSocket, &readfds);



	char dataBuffer[BUFFER_SIZE];

	while (true) {
		iResult = select(0, NULL, &readfds, NULL, NULL);

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
			return;

		}
	}

}


int SendPortAndSpace(int p, SOCKET* connectSocket) {
	
	srand(time(NULL));
	int x = rand() % 10;

	char str1[10]; 
	_itoa(p,str1,10);
	char str2[10];
	_itoa(x, str2, 10);
	strcat(str1, " ");
	strcat(str1, str2);
	//printf("%s", str1);
	SelectWrite(connectSocket);



	
	// Send message to server using connected socket
	int iResult = send(*connectSocket, str1, (int)strlen(str1), 0);

	// Check result of send function
	if (iResult == SOCKET_ERROR)
	{
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(*connectSocket);
		WSACleanup();
		return 1;
	}


	return x;


}

int getRandInterval(int begin, int end) {
	int range = (end - begin) + 1;
	int limit = (10 + 1) - (((int)10 + 1) % range);

	/* Imagine range-sized buckets all in a row, then fire randomly towards
	 * the buckets until you land in one of them. All buckets are equally
	 * likely. If you land off the end of the line of buckets, try again. */
	int randVal = rand();
	while (randVal >= limit) randVal = rand();

	/// Return the position you hit in the bucket + begin as random number
	return (randVal % range) + begin;
}


DWORD WINAPI Client2Clients(LPVOID lpParam) {

	printf("\n\n*************************************************************************\n\n");

	struct ClientSend *cs = (struct ClientSend *)lpParam;
	//printf("!!!%d ", cs->brKl);
	printf("Deo fajla koji je poslao server: %s\n", cs->value);
	/*for (int i = 0; i < cs->brKl; i++) {
		printf("!!!PORT %d ", cs->ports[i]);
		printf("!!!%d ", cs->prviIndeksi[i]);
		printf("!!!%d ", cs->zadnjIndeksi[i]);
	}*/
	char ceoFajl[MAXBUFF];
	strcpy(ceoFajl, "");
	//strcpy(ceoFajl, cs->value);
	
	int i = 0;
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


	SOCKET connectSocket = INVALID_SOCKET;
	SOCKET acceptedSocket = INVALID_SOCKET;



	// Buffer we will use to store message
	char dataBuffer[BUFFER_SIZE];


	


	while (i < cs->brKl) {

		connectSocket = INVALID_SOCKET;
		acceptedSocket = INVALID_SOCKET;
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
		serverAddress.sin_port = htons(cs->ports[i]);					// server port

		// Connect to server specified in serverAddress and socket connectSocket
		if (connect(connectSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		{
			printf("Unable to connect to server.\n");
			closesocket(connectSocket);
			WSACleanup();
			return 1;
		}
		Sleep(400);
		SelectRead(&connectSocket);
		iResult = recv(connectSocket, dataBuffer, BUFFER_SIZE, 0);

		if (iResult > 0)	// Check if message is successfully received
		{
			dataBuffer[iResult] = '\0';
		//	printf("\nDOBIO SAM OD KLIJENTASERVERA : %s\n", dataBuffer);
			
			
		/*	if (i == cs->brKl - 1) {
				for (int j = cs->zadnjIndeksi[i] + 1; j < (cs->zadnjIndeksi[i] + strlen(cs->value) ); i++) {
					ceoFajl[j] = cs->value[i];

				}
				printf("\nCEO FAJL  : &s", ceoFajl);
				i++;
			}
			//else {
				for (int j = cs->prviIndeksi[i]; j < cs->zadnjIndeksi[i]; j++) {
					int x = 0;
					ceoFajl[j] = dataBuffer[x];
					x++;

				}
				ceoFajl[cs->zadnjIndeksi[i] ] = '\0';
				printf("\nCEO FAJL  : %s", ceoFajl);
				i++;
		//	}*/
		//	i++;


			strcat(ceoFajl, dataBuffer);
			printf("\nDeo fajla koji je poslao klijent%d: %s\n\n",i+1 , dataBuffer);
			if (i == cs->brKl - 1) {
				strcat(ceoFajl, cs->value);
				printf("\nCeo fajl: %s\n\n", ceoFajl);
				

				return 0;
			}

			i++;


		}
		else if (iResult == 0)	// Check if shutdown command is received
		{
			// Connection was closed successfully
			printf("Connection with client closed.\n");
			closesocket(connectSocket);
		}
		else	// There was an error during recv
		{

			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(connectSocket);
			//SelectSend(&acceptedSocket);





			iResult = shutdown(connectSocket, SD_BOTH);

			// Check if connection is succesfully shut down.
			if (iResult == SOCKET_ERROR)
			{
				printf("shutdown failed with error: %d\n", WSAGetLastError());
				closesocket(connectSocket);
				WSACleanup();
				return 1;
			}

			//Close listen and accepted sockets

			closesocket(connectSocket);

			// Deinitialize WSA library
			WSACleanup();







			return 0;
		}
	}
	return 0;
}

