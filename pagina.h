#ifndef PAGINA_H
#define PAGINA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct pagina Pagina;

typedef struct listap ListaP;

Pagina* CriaPagina(char* nome);

char* GetNomePagina(Pagina* p);

double GetPRPagina(Pagina* p, int i); 

ListaP* GetListaPagina(Pagina* p);

int GetNLinksPagina(Pagina* p);

void SetPRPagina(Pagina* p, double pr, int i);

void SetListaPagina(Pagina* o, Pagina* d);

void SetNLinksPagina(Pagina* p, int nLinks);

void LiberaPagina(void* p);

ListaP* CriaLista(Pagina* pagina);

ListaP* InserePaginaLista(ListaP* lista, Pagina* pagina);

Pagina* GetPaginaLista(ListaP* l);

ListaP* GetProxLista(ListaP* l);

void LiberaLista(void* l);

#endif