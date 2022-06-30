
#include "pch.h"
#define MAXBUFF 99
#include <stdio.h>
#include <stdio.h>
#include <winsock2.h>

typedef struct SockAndVal
{
	char val[MAXBUFF];
	SOCKET* socket;
}; 
