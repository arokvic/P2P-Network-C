#pragma once
#pragma once
#include <stdlib.h>
#define WIN32_LEAN_AND_MEAN
#define MAXBUFF 99
#include <stdio.h>
#include "conio.h"
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>


DWORD WINAPI ClientServerThread(LPVOID lpParam);
//DWORD WINAPI AcceptedSocketThreadClient(LPVOID lpParam);
DWORD WINAPI Client2Clients(LPVOID lpParam);
void AcceptedSocketThreadClient(struct SockAndPort sp,SOCKET* listenSocket);
void SelectWrite(SOCKET* acceptedSocket);
void SelectRead(SOCKET* acceptedSocket);
int SendPortAndSpace(int p, SOCKET* connectSocket);
int getRandInterval(int begin, int end);