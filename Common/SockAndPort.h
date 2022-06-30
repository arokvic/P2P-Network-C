#pragma once
#define MAXBUFF 99
#include "pch.h"
#include <stdio.h>
#include <winsock2.h>

#include <stdio.h>

typedef struct SockAndPort
{
	SOCKET* socket;
	int port;
};