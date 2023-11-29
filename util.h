#ifndef UTIL_H
#define UTIL_H

#include "pagina.h"
#include "TST.h"

void ProcessadorDeConsultas(TST* termos, TST* stopWords);

TST* LePaginas(char* arquivo, int* nPaginas);

TST* LeStopWords(char* arquivo, int* nStopWords);

void LeGraphs(TST* paginas, char* arquivo, int nPaginas);

TST* Indexador(TST* termos, TST* stopWords, Pagina* p, char* diretorio);

TST* RecIndexador(TST* termos, TST* paginas, TST* stopWords, char* diretorio);

void LiberaTermos(void* termos);

void LiberaWord(void* word);

int PowerMethod(TST* paginas, int nPaginas);

int ComparePaginas(const void *p1,const void *p2);

Pagina** PreencheVetor(TST* intercessao, Pagina** vetor, int* posicao);

#endif