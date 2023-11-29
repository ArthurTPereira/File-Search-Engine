#include "pagina.h"


// Estrutura de uma pagina
struct pagina{
    char* nome;
    double* pageRank;
    ListaP* listaPaginas;
    int nLinks;
};

// Estrutura de uma lista de paginas
struct listap{
    Pagina* pagina;
    ListaP* prox;
};

// Funcao que cria uma pagina
// Entrada: char* nome - Nome da pagina
// Saida: Pagina* - Ponteiro para a pagina criada
Pagina* CriaPagina(char* nome){
    Pagina* p = (Pagina*) malloc(sizeof(Pagina));
    p->nome = strdup(nome);
    p->listaPaginas = NULL;
    p->pageRank = (double*)malloc(2 * sizeof(double));
    return p;
}

// Funcao que retorna o nome de uma pagina
// Entrada: Pagina* p - Ponteiro para a pagina
// Saida: char* - Nome da pagina
char* GetNomePagina(Pagina* p){
    return p->nome;
}

// Funcao que retorna o page rank de uma pagina
// Entrada: Pagina* p - Ponteiro para a pagina
// Saida: double - Page rank da pagina
double GetPRPagina(Pagina* p, int i){
    return p->pageRank[i];
}

// Funcao que retorna a lista de paginas de uma pagina
// Entrada: Pagina* p - Ponteiro para a pagina
// Saida: ListaP* - Ponteiro para a lista de paginas
ListaP* GetListaPagina(Pagina* p){
    return p->listaPaginas;
}

// Funcao que retorna o numero de links de uma pagina
// Entrada: Pagina* p - Ponteiro para a pagina
// Saida: int - Numero de links da pagina
int GetNLinksPagina(Pagina* p){
    return p->nLinks;
}

// Funcao que seta o page rank de uma pagina
// Entrada: Pagina* p - Ponteiro para a pagina
//          double pr - Page rank
void SetPRPagina(Pagina* p, double pr, int i){
    p->pageRank[i] = pr;
}

// Funcao que seta a lista de paginas de uma pagina
// Entrada: Pagina* o - Ponteiro para a pagina origem
//          Pagina* d - Ponteiro para a pagina destino
// Saida: void
void SetListaPagina(Pagina* o, Pagina* d){
    ListaP* aux = (ListaP*) malloc(sizeof(ListaP));
    aux->pagina = o;
    aux->prox = d->listaPaginas;
    d->listaPaginas = aux;
}

// Funcao que seta o numero de links de uma pagina
// Entrada: Pagina* p - Ponteiro para a pagina
//          int nLinks - Numero de links
// Saida: void
void SetNLinksPagina(Pagina* p, int nLinks){
    p->nLinks = nLinks;
}

// Funcao que libera a memoria alocada para uma pagina
// Entrada: void* p - Ponteiro para a pagina
// Saida: void
void LiberaPagina(void* pagina){
    Pagina* p = (Pagina*)pagina;
    // Libera a memoria alocada para a lista de paginas
    LiberaLista(p->listaPaginas);

    // Libera o nome da pagina e a pagina
    free(p->pageRank);
    free(p->nome);
    free(p);
}

// Funcao que libera a memoria alocada para uma lista de paginas
// Entrada: void* l - Ponteiro para a lista de paginas
// Saida: void
void LiberaLista(void* l){
    ListaP* lista = (ListaP*)l;
    while(lista != NULL){
        ListaP* aux = lista;
        lista = lista->prox;
        free(aux);
    }
}

// Funcao que cria uma lista de paginas
// Entrada: Pagina* pagina - Ponteiro para a pagina
// Saida: ListaP* - Ponteiro para a lista de paginas
ListaP* CriaLista(Pagina* pagina){
    ListaP* l = (ListaP*)malloc(sizeof(ListaP));
    l->pagina = pagina;
    l->prox = NULL;
    return l;
}

// Funcao que insere uma pagina em uma lista de paginas
// Entrada: ListaP* lista - Ponteiro para a lista de paginas
//          Pagina* pagina - Ponteiro para a pagina
// Saida: ListaP* - Ponteiro para a lista de paginas
ListaP* InserePaginaLista(ListaP* lista, Pagina* pagina){
    ListaP* l;
    l = CriaLista(pagina);
    if(lista != NULL){
        l->prox = lista;
    }
    return l;
}

// Funcao que retorna o inicio de uma lista de paginas
// Entrada: ListaP* l - Ponteiro para a lista de paginas
// Saida: Pagina* - Ponteiro para a pagina
Pagina* GetPaginaLista(ListaP* l){
    return l->pagina;
}

// Funcao que retorna o proximo elemento de uma lista de paginas
// Entrada: ListaP* l - Ponteiro para a lista de paginas
// Saida: ListaP* - Ponteiro para o proximo elemento da lista
ListaP* GetProxLista(ListaP* l){
    return l->prox;
}
