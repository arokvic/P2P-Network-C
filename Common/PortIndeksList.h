#pragma once
#pragma once

#define MAXBUFF 99
#include "pch.h"
#include <stdio.h>


typedef struct PortIndeksList
{
	char clVal[MAXBUFF];
	int port;
	int prviIndeks;
	int zadnjiIndeks;
	struct PortIndeksList* next = NULL;
};

void addPortIndeks(PortIndeksList** head, int prviInd, int poslInd, int port);
void printPortIndeksList(PortIndeksList* head);
int IzracunajIndekse(PortIndeksList* head, int clSize);
int IzracunajbrojElemenata(PortIndeksList* head);
int* VratiPortIndkse(PortIndeksList* head, int x);
char* serijalizacijaListe(PortIndeksList* head, int brEl);
bool proveraSviKodKlijenta(PortIndeksList* head, int duzinaFajla);
void deleteNode(PortIndeksList** head_ref, int port);
//void popuniC2C(c2c* c, PortIndeksList* head);