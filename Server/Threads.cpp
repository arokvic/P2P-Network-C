#include "../Common/ThreadStruct.h"
#include "../Common/UsersList.h"
#include "../Common/ListaFile.h"
#include "../Common/PortIndeksList.h"
#include <stdlib.h>
#include "Threads.h"
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




DWORD WINAPI AcceptedSocketThread(LPVOID lpParam) {

	printf("Klijent se povezao\n");
	int clPort;
	int clSpace;
	struct ThreadStruct1* ts = (ThreadStruct1 *)lpParam;
	HANDLE handle, handle2;
	DWORD dword;
	char dataBuffer[BUFFER_SIZE];
	//SOCKET acceptedSocket = *(SOCKET *)lpParam;
	SOCKET *acceptedSocket = ts->socket;
	struct FileList * fl = ts->head;
//	print(fl);
	struct UserList * ul = ts->head2;
	struct PortIndeksList pil;
	struct PortIndeksList * pilp;
	pilp = &pil;


	int iResult;
	//while (true) {

//	SelectRead(&acceptedSocket);
	getPortAndSpace(&clPort, &clSpace, acceptedSocket);
	//printf("%d %d", clPort, clSpace);

	addUserPort(  &ul, clPort);

	//printPorts(ul);


	while (true) {

		SelectRead(acceptedSocket);
		iResult = recv(*acceptedSocket, dataBuffer, BUFFER_SIZE, 0);

		if (iResult > 0)	// Check if message is successfully received
		{

			dataBuffer[iResult] = '\0';

			// Log message text
			printf("Klijent poslao: %s.\n", dataBuffer);
			strcpy(ts->name, dataBuffer);



			
			char res[BUFFER_SIZE];
			if (fileExists(fl, ts->name)) {

				//pil = find(&ul, ts->name);
				if (find(&ul,ts->name,&pilp)) { // FAJL NADJEN U USERLIST I IZVUKLI SMO SVE PORTOVE I INDEKSE KOJI SADRZE FAJL SA DATIM IMENOM(PIL)
					if (proveraSviKodKlijenta(&pil, strlen(findFileVal(&fl, ts->name)))) {
					//	printf("SVI SU KOD KLIJENTAAA");
						SelectWrite(acceptedSocket);
						iResult = send(*acceptedSocket, "3\0", 2, 0);
						if (iResult == SOCKET_ERROR)
						{
							printf("send failed with error: %d\n", WSAGetLastError());
							closesocket(*acceptedSocket);
							WSACleanup();
							return 1;
						}

					//	printf("usao");
					//	printPortIndeksList(&pil);
						int poslInd = IzracunajIndekse(&pil, clSpace);
						int prviInd = poslInd - clSpace + 1;
					//	printf("-------%d %d", prviInd, poslInd);
					
						addIndeks(&ul, clPort, prviInd, poslInd, ts->name);
						
						char val[256];
						strcpy(val, findFileVal(&fl, ts->name));
						int valLen = strlen(val);
					//	int lenToSend = valLen - prviInd;
					//	char valToSend[256];
					//	for (int i = 0; i < lenToSend; ++i)
					//	{
					//		valToSend[i] = val[prviInd + i];
					//	}
					//	valToSend[lenToSend] = '\0';
						//printf("\n%s", valToSend);
						int brPovezanihKlijenata = IzracunajbrojElemenata(&pil);
						//send
					/*	int x = 1;
						int* portoviIndeksi;
						while (x <= brPovezanihKlijenata) {
							portoviIndeksi = VratiPortIndkse(&pil, x);
							printf("%d %d %d\n", portoviIndeksi[0], portoviIndeksi[1], portoviIndeksi[2]);
							x++;
						}*/
						int sizeSer = (sizeof(int) + sizeof(int) + sizeof(int)) * brPovezanihKlijenata;
						Sleep(400);

						SelectWrite(acceptedSocket);
						iResult = send(*acceptedSocket, (char*)&brPovezanihKlijenata, 4, 0);	// SALJE BROJ KLIJENATA SA FAJLOM
						if (iResult == SOCKET_ERROR)
						{
							printf("send failed with error: %d\n", WSAGetLastError());
							closesocket(*acceptedSocket);
							WSACleanup();
							return 1;
						}
						Sleep(400);
						char* mess = (char*)malloc(sizeSer * brPovezanihKlijenata);
						mess = serijalizacijaListe(&pil, brPovezanihKlijenata);

						SelectWrite(acceptedSocket);
						iResult = send(*acceptedSocket, mess, sizeSer, 0);	//SALJE STRUKTURU
						if (iResult == SOCKET_ERROR)
						{
							printf("send failed with error: %d\n", WSAGetLastError());
							closesocket(*acceptedSocket);
							WSACleanup();
							return 1;
						}



					}
					else {



						SelectWrite(acceptedSocket);
						iResult = send(*acceptedSocket, "2\0", 2, 0);
						if (iResult == SOCKET_ERROR)
						{
							printf("send failed with error: %d\n", WSAGetLastError());
							closesocket(*acceptedSocket);
							WSACleanup();
							return 1;
						}


					//	printf("usao");
					//	printPortIndeksList(&pil);
						int poslInd = IzracunajIndekse(&pil, clSpace);
						int prviInd = poslInd - clSpace + 1;
					//	printf("-------%d %d", prviInd, poslInd);
						
						addIndeks(&ul, clPort, prviInd, poslInd, ts->name);
						
						char val[256];
						strcpy(val, findFileVal(&fl, ts->name));
						int valLen = strlen(val);
						int lenToSend = valLen - prviInd;
						char valToSend[256];
						for (int i = 0; i < lenToSend; ++i)
						{
							valToSend[i] = val[prviInd + i];
						}
						valToSend[lenToSend] = '\0';
						//printf("\n%s", valToSend);
						int brPovezanihKlijenata = IzracunajbrojElemenata(&pil);
						//send
					/*	int x = 1;
						int* portoviIndeksi;
						while (x <= brPovezanihKlijenata) {
							portoviIndeksi = VratiPortIndkse(&pil, x);
							printf("%d %d %d\n", portoviIndeksi[0], portoviIndeksi[1], portoviIndeksi[2]);
							x++;
						}*/
						int sizeSer = (sizeof(int) + sizeof(int) + sizeof(int)) * brPovezanihKlijenata;
						Sleep(400);

						SelectWrite(acceptedSocket);
						iResult = send(*acceptedSocket, (char*)&brPovezanihKlijenata, 4, 0);	// SALJE BROJ KLIJENATA SA FAJLOM
						if (iResult == SOCKET_ERROR)
						{
							printf("send failed with error: %d\n", WSAGetLastError());
							closesocket(*acceptedSocket);
							WSACleanup();
							return 1;
						}
						Sleep(400);
						char* mess = (char*)malloc(sizeSer * brPovezanihKlijenata);
						mess = serijalizacijaListe(&pil, brPovezanihKlijenata);

						SelectWrite(acceptedSocket);
						iResult = send(*acceptedSocket, mess, sizeSer, 0);	//SALJE STRUKTURU
						if (iResult == SOCKET_ERROR)
						{
							printf("send failed with error: %d\n", WSAGetLastError());
							closesocket(*acceptedSocket);
							WSACleanup();
							return 1;
						}

						Sleep(400);
						SelectWrite(acceptedSocket);
						iResult = send(*acceptedSocket, valToSend, valLen, 0);	//SALJE SADRZAJ
						if (iResult == SOCKET_ERROR)
						{
							printf("send failed with error: %d\n", WSAGetLastError());
							closesocket(*acceptedSocket);
							WSACleanup();
							return 1;
						}
					}

				}
				else {	//FAJL NEMA NIKO
					void SelectWrite(SOCKET* acceptedSocket);
					iResult = send(*acceptedSocket, "1\0", 2, 0);
					if (iResult == SOCKET_ERROR)
					{
						printf("send failed with error: %d\n", WSAGetLastError());
						closesocket(*acceptedSocket);
						WSACleanup();
						return 1;
					}



				//	printf("nisam");
					char val[256];
					strcpy(val,findFileVal(&fl, ts->name));
					//printf("%s", val);
					int fajlLen = strlen(val);
					int prviInd = 0;
					int poslInd = clSpace - 1;
					
					addIndeks(&ul, clPort, prviInd, poslInd, ts->name);
					
					//printPorts(ul);



					strcpy(res, findFileVal(&fl, ts->name));
					Sleep(400);
					SelectWrite(acceptedSocket);
					iResult = send(*acceptedSocket, (char*)res, strlen(res), 0);

					
				   // Check result of send function
					if (iResult == SOCKET_ERROR)
					{
						printf("send failed with error: %d\n", WSAGetLastError());
						closesocket(*acceptedSocket);
						WSACleanup();
						return 1;
					}

					void SelectWrite(SOCKET* acceptedSocket);
					iResult = send(*acceptedSocket, (char*)res, (int)strlen(res), 0);


					// Check result of send function
					if (iResult == SOCKET_ERROR)
					{
						printf("send failed with error: %d\n", WSAGetLastError());
						closesocket(*acceptedSocket);
						WSACleanup();
						return 1;
					}



				}
				
				//while (true) {}

				
				
			}
			else {
			
				strcpy(res, "Trazeni fajl ne postoji");
				void SelectWrite(SOCKET* acceptedSocket);
				iResult = send(*acceptedSocket, (char*)res, (int)strlen(res), 0);

				
				if (iResult == SOCKET_ERROR)
				{
					printf("send failed with error: %d\n", WSAGetLastError());
					closesocket(*acceptedSocket);
					WSACleanup();
					return 1;
				}

				

			}
		}
		else if (iResult == 0)	// Check if shutdown command is received
		{
			// Connection was closed successfully
			printf("Connection with client closed.\n");
			closesocket(*acceptedSocket);
		}
		else	// There was an error during recv
		{

			//printf("recv failed with error: %d\n", WSAGetLastError());
			printf("Klijent se diskonektovao.\n");
		
			deleteUser(clPort, &ul);
			int poslInd = IzracunajIndekse(&pil, clSpace);
			
			return 0;
			closesocket(*acceptedSocket);
			//SelectSend(&acceptedSocket);





			iResult = shutdown(*acceptedSocket, SD_BOTH);

			// Check if connection is succesfully shut down.
			if (iResult == SOCKET_ERROR)
			{
				printf("shutdown failed with error: %d\n", WSAGetLastError());
				closesocket(*acceptedSocket);
				WSACleanup();
				return 1;
			}

			//Close listen and accepted sockets

			closesocket(*acceptedSocket);

			// Deinitialize WSA library
			WSACleanup();





		}		SelectWrite(acceptedSocket);	}}/*DWORD WINAPI FindThread(LPVOID lpParam) {	char val[50];	char name[50];	struct ThreadStruct1* ts = (struct ThreadStruct1 *)lpParam;	struct FileList * fl = ts->head;	struct UserList * ul = ts->head2;	strcpy(name, ts->name);//	if (fileExists(&fl, name))//	{	//	printf("fajl postoji:");	//	strcpy(val, find(&fl, name));	//	printf("%s", val);//	}//	else {//		printf("fajl ne postoji");//	}	return 0;}*/void SelectRead(SOCKET* acceptedSocket) {

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
void getPortAndSpace(int* p, int*s, SOCKET* acceptedSocket) {	char dataBuffer[256];	SelectRead(acceptedSocket);
	int iResult = recv(*acceptedSocket, dataBuffer, BUFFER_SIZE, 0);

	if (iResult > 0)	// Check if message is successfully received
	{

		dataBuffer[iResult] = '\0';
		char port[10];
		char space[10];

		int x = 0;
		char *token = strtok(dataBuffer, " ");
		while (token != NULL) {
			if (x == 0) {
				strcpy(port, token);
				x++;
				token = strtok(NULL, " ");
			}
			else if (x == 1) {
				strcpy(space, token);
				x++;
				token = strtok(NULL, " ");
			}


		}

		*p = atoi(port);
		*s = atoi(space);

	}
	else if (iResult == 0)	// Check if shutdown command is received
	{
		// Connection was closed successfully
		printf("Connection with client closed.\n");
		closesocket(*acceptedSocket);
	}
	else	// There was an error during recv
	{

		printf("recv failed with error: %d\n", WSAGetLastError());
		closesocket(*acceptedSocket);
		//SelectSend(&acceptedSocket);

		iResult = shutdown(*acceptedSocket, SD_BOTH);

		// Check if connection is succesfully shut down.
		if (iResult == SOCKET_ERROR)
		{
			printf("shutdown failed with error: %d\n", WSAGetLastError());
			closesocket(*acceptedSocket);
			WSACleanup();
			return ;
		}

		//Close listen and accepted sockets

		closesocket(*acceptedSocket);

		// Deinitialize WSA library
		WSACleanup();





	}}