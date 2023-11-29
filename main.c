#include <string.h>
#include <stdlib.h>

#include "util.h"

#define ERROR -1
#define SUCCESS 0

int main(int argc, char* argv[]) {

    // Verifica os argumentos
    if (argc < 2) {
        printf("Erro: Nenhum arquivo de entrada foi passado.\n");
        return ERROR;
    }

    // Numero de paginas total
    int nPaginas = 0;

    // Le as paginas
    TST* paginas = LePaginas(argv[1], &nPaginas);

    // Numero de stop words
    int nStopWords = 0;

    // Le as stop words
    TST* stopWords = LeStopWords(argv[1], &nStopWords);

    // Le o arquivo graph.txt
    LeGraphs(paginas, argv[1], nPaginas);

    // Monta o caminho para a pasta pages
    char* pages = malloc((strlen(argv[1]) + strlen("pages/") + 1)* sizeof(char));
    strcpy(pages, argv[1]);
    strcat(pages, "pages/");

    // Inicia o indexador
    TST* termos = NULL;
    termos = RecIndexador(termos, paginas, stopWords, pages);

    // Executa o power method
    int posicao = PowerMethod(paginas, nPaginas);

    // Executa o processador de consultas
    ProcessadorDeConsultas(termos, stopWords);
    
    // Libera a memoria
    TST_destroy(termos, LiberaTermos);
    TST_destroy(paginas, LiberaPagina);
    TST_destroy(stopWords, LiberaWord);
    free(pages);

    return SUCCESS;
}
