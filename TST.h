#ifndef TST_H
#define TST_H
#include <stdbool.h>
typedef struct node TST;

TST* create_node();

TST* TST_insert(TST* t, char* key, void* val, bool isStopWord);

void* TST_search(TST* t, char* key);

TST* rec_insert(TST* t, char* key, void* val, int d, bool isStopWord);

TST* rec_search(TST* t, char* key, int d);

void TST_destroy(TST* n, void (*Libera) (void*));

TST* GetLeft(TST* n);

TST* GetRight(TST* n);

TST* GetMid(TST* n);

void* GetValue(TST* n);

void TST_print(TST* n);

#endif