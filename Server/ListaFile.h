
#define MAXBUFF 99

#include <stdio.h>

typedef struct FileList
{
	char value[MAXBUFF];
	char name[15];
	struct FileList* next = NULL;
}fileLista;



void addToTheBegining(FileList** head, char* value, char* name);
char* find(FileList** head, char* name);
void deleteNode(char* name, FileList** head);
void ReadFromFile(FileList** head);
void print(FileList *head);
bool fileExists(FileList** head, char* name);