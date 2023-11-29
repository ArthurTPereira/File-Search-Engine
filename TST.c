#include "TST.h"
#include <stdlib.h>
#include <stdbool.h>
#include "pagina.h"

#define NULL_Value NULL

struct node {
    void* val;
    unsigned char c;
    TST *l, *m, *r;
};

TST* create_node() {
    TST* n = (TST*) malloc(sizeof(TST));
    n->val = NULL_Value;
    n->l = NULL;
    n->m = NULL;
    n->r = NULL;
    return n;
}

TST* rec_insert(TST* t, char* key, void* val, int d, bool isStopWord ) {
    unsigned char c = key[d];
    if (t == NULL) {
        t = create_node();
        t->c = c;
    }
    if (c < t->c) {
        t->l = rec_insert(t->l, key, val, d, isStopWord);
    }
    else if (c > t->c){ 
        t->r = rec_insert(t->r, key, val, d, isStopWord); 
    }
    else if (key[d + 1] != '\0'){
        t->m = rec_insert(t->m, key, val, d+1, isStopWord);
    } else { 
        // Libera o valor antigo antes de substituir (ha uma stopword repetida)
        if (isStopWord && t->val != NULL) free(t->val); 
        t->val = val;
    }
    return t;
}

TST* TST_insert(TST* t, char* key , void* val, bool isStopWord) {
    return rec_insert(t, key, val, 0, isStopWord);
}

TST* rec_search(TST* t, char* key, int d) {
    if (t == NULL) { return NULL; }
    unsigned char c = key[d];
    if (c < t->c) { return rec_search(t->l, key, d); }
    else if (c > t->c) { return rec_search(t->r, key, d); }
    else if (key[d + 1] != '\0') {
    return rec_search(t->m, key, d+1);
    } else { return t; }
}

void* TST_search(TST* t, char* key) {
    t = rec_search(t, key, 0);
    if (t == NULL) { return NULL_Value; }
    else { return t->val; }
}

// Funcao que retorna o filho esquerdo de um no
// Entrada:     TST* n - Ponteiro para o no
// Saida:       TST* - Ponteiro para o filho esquerdo
TST* GetLeft(TST* n){
    return n->l;
}

// Funcao que retorna o filho direto de um no
// Entrada:     TST* n - Ponteiro para o no
// Saida:       TST* - Ponteiro para o filho direto
TST* GetRight(TST* n){
    return n->r;
}

// Funcao que retorna o filho do meio de um no
// Entrada:     TST* n - Ponteiro para o no
// Saida:       TST* - Ponteiro para o filho do meio
TST* GetMid(TST* n){
    return n->m;
}

// Funcao que retorna o valor de um no
// Entrada:     TST* n - Ponteiro para o no
// Saida:       void* - Valor do no
void* GetValue(TST* n){
    return n->val;
}

// Funcao que destroi uma TST
// Entrada:     TST* n - Ponteiro para a TST
//              void (*Libera) (void*) - Ponteiro para a funcao que libera o valor
// Saida:       void
void TST_destroy(TST* n, void (*Libera) (void*)) {
    if (n == NULL) return;
    TST_destroy(n->l, Libera);
    TST_destroy(n->m, Libera);
    TST_destroy(n->r, Libera);
    if(n->val != NULL) Libera(n->val);
    free(n);
    return;
}

// Temporario
void TST_print(TST* n) {
    if (n == NULL) return;
    TST_print(n->l);
    TST_print(n->m);
    if(n->val != NULL){
        printf("%s\n", GetNomePagina((Pagina*)n->val));
    }
    TST_print(n->r);
    return;
}
