#pragma once

#include "pch.h"
#include "PortIndeksList.h"
#define MAXBUFF 99
#include <stdio.h>
#include <stdio.h>
#include <winsock2.h>

typedef struct ClientSend
{
	int brKl;
	int ports[MAXBUFF];
	int prviIndeksi[MAXBUFF];
	int zadnjIndeksi[MAXBUFF];
	char value[MAXBUFF];
};



void popuniClientSend(ClientSend* c, PortIndeksList* head);