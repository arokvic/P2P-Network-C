#pragma once

#include <stdio.h>
#include <winsock2.h>
#include "pch.h"

typedef struct ThreadStruct1
{
	SOCKET* socket;
	struct FileList* head;
	struct UserList* head2;
	char name[50];
};