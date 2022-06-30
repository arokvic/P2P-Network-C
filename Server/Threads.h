#pragma once
#include <stdlib.h>
#define WIN32_LEAN_AND_MEAN
#define MAXBUFF 99
#include <stdio.h>
#include "conio.h"
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>


DWORD WINAPI AcceptedSocketThread(LPVOID lpParam);
//DWORD WINAPI FindThread(LPVOID lpParam);
void SelectRead(SOCKET* acceptedSocket);
void SelectWrite(SOCKET* acceptedSocket);
void getPortAndSpace(int* p, int*s, SOCKET* acceptedSocket);