
#include <stdio.h>

typedef struct UserList
{
	int prviIndeks;
	int poslednjiIndeks;
	struct UserList* next;
	char fileName[15];
	int port;
}userLista;



void addUser(UserList** head, int prviInd, int poslInd , char* name , int port);
//char* find(UserList** head, char* fileName);
void deleteUser(int port , UserList** head);