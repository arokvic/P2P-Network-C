#pragma once
#include "pch.h"

DWORD WINAPI AcceptedSocketThread(LPVOID lpParam);
DWORD WINAPI FindThread(LPVOID lpParam);
DWORD WINAPI ClientServerThread(LPVOID lpParam);
DWORD WINAPI AcceptedSocketThreadClient(LPVOID lpParam);