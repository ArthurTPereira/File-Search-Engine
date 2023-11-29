#include "util.h"
#include <stdbool.h>
#include <math.h>
#include <ctype.h>

#define ERROR -1
#define SUCCESS 0

#define EPSILON 0.000000001 // 10^-9

// Funcao auxiliar para converter uma string para minusculo
// Entrada: str - string a ser convertida
// Saida: void
void STR_toLower(char* str){
    int i = 0;
    while(str[i] != '\0'){
        str[i] = tolower(str[i]);
        i++;
    }
}

// Funcao que le o arquivo index e retorna a arvore
// Entrada: arquivo - caminho do arquivo index.txt
//          nPaginas - ponteiro para o numero de paginas
// Saida: TST com as paginas
TST* LePaginas(char* arquivo, int* nPaginas){
    
    // Cria o caminho para o arquivo index.txt
    char* indexPath = malloc((strlen(arquivo) + strlen("index.txt") + 1) * sizeof(char));
    strcpy(indexPath, arquivo);
    strcat(indexPath, "index.txt");

    // Abre o arquivo index.txt
    FILE* fpIndex = fopen(indexPath,"r");

    // Verifica se o arquivo index.txt foi aberto corretamente
    if (fpIndex == NULL) {
        printf("Erro: index.txt nao encontrado.\n");
        exit(ERROR);
    }

    // Buffer para leitura
    size_t bufferSize = 64;
    char * buffer = (char*) malloc(bufferSize * sizeof(char));

    TST* paginas = NULL;
    int i = 0;

    // Le o arquivo index.txt
    while (!feof(fpIndex)) {
        int qtdLida = getline(&buffer, &bufferSize, fpIndex);
        if(qtdLida == -1) break;
        
        // Obtem o nome da pagina atual
        char* token = strtok(buffer,"\n");
        buffer = token;

        // Cria a pagina
        Pagina* p = CriaPagina(buffer);

        // Incrementa o numero de paginas
        paginas = TST_insert(paginas, GetNomePagina(p), (void*)p, false);
        i++;
    }

    // Atualiza o numero de paginas
    *nPaginas = i;

    // Fecha o arquivo index.txt
    fclose(fpIndex);
    
    // Libera a memoria alocada
    free(buffer);
    free(indexPath);

    return paginas;
}

// Funcao que le o arquivo stopwords e retorna a arvore
// Entrada: arquivo - caminho do arquivo stopwords.txt
//          nStopWords - ponteiro para o numero de stopwords
// Saida: TST com as stopwords
TST* LeStopWords(char* arquivo, int* nStopWords){

    // Cria o caminho para o arquivo stopwords.txt
    char* stopwordsPath = malloc((strlen(arquivo) + strlen("stopwords.txt") + 1) * sizeof(char));
    strcpy(stopwordsPath, arquivo);
    strcat(stopwordsPath, "stopwords.txt");

    // Abre o arquivo stopwords.txt
    FILE* fpstopwords = fopen(stopwordsPath,"r");

    // Verifica se o arquivo stopwords.txt foi aberto corretamente
    if (fpstopwords == NULL) {
        printf("Erro: stopwords.txt nao encontrado.\n");
        exit(ERROR);
    }

    // Buffer para leitura
    size_t bufferSize = 64;
    char * buffer = (char*) malloc(bufferSize * sizeof(char));

    TST* stopwords = NULL;
    int i = 0;
    
    // Le o arquivo stopwords.txt
    while (!feof(fpstopwords)) {
        int qtdLida = getline(&buffer, &bufferSize, fpstopwords);
        if(qtdLida == -1) break;
        
        // Obtem a stop word atual
        char* pt = strtok(buffer,"\n");
        buffer = pt;

        // Converte a stop word para minusculo
        STR_toLower(buffer);

        char* valor = strdup(buffer);

        // Insere a stop word na arvore
        stopwords = TST_insert(stopwords, buffer, (void*)valor, true);

        // Incrementa o numero de stopwords
        i++;
    }

    // Atualiza o numero de stopwords
    *nStopWords = i;


    // Fecha o arquivo stopwords.txt
    fclose(fpstopwords);

    // Libera a memoria alocada
    free(buffer);
    free(stopwordsPath);

    return stopwords;
}

// Funcao que le o arquivo graph.txt e atualiza os links da lista de paginas
// Entrada: paginas - TST com as paginas
//          arquivo - caminho do arquivo graph.txt
//          nPaginas - numero de paginas
void LeGraphs(TST* paginas, char* arquivo, int nPaginas){
    // Cria o caminho para o arquivo graphs.txt
    char* graphsPath = malloc((strlen(arquivo) + strlen("graph.txt") + 1) * sizeof(char));
    strcpy(graphsPath, arquivo);
    strcat(graphsPath, "graph.txt");

    // Abre o arquivo graphs.txt
    FILE* fpGraphs = fopen(graphsPath,"r");

    // Verifica se o arquivo graphs.txt foi aberto corretamente
    if (fpGraphs == NULL) {
        printf("Erro: graph.txt nao encontrado.\n");
        exit(ERROR);
    }

    // Buffer para leitura
    size_t bufferSize = 64;
    char * buffer = (char*) malloc(bufferSize * sizeof(char));

    // Le o arquivo graphs.txt
    while (!feof(fpGraphs)) {
        int qtdLida = getline(&buffer, &bufferSize, fpGraphs);
        if (qtdLida == -1) break;
        // Obtem o nome da pagina atual
        char* token = strtok(buffer, "\n");

        // Obtem a pagina atual
        token = strtok(token, " ");

        // Obtem a pagina atual
        Pagina* origem = (Pagina*)TST_search(paginas, token);

        // Obtem o numero de links da pagina atual
        token = strtok(NULL," ");
        if (origem == NULL) {
            printf("Erro: pagina %s nao encontrada.\n", token);
            printf("%s\n",buffer);
            exit(ERROR);
        }
        // Atualiza o numero de links da pagina atual
        SetNLinksPagina(origem, atoi(token));

        //Inicializa o PR com 1/nPaginas
        SetPRPagina(origem, ((double)1.0/nPaginas), 0);

        // Obtem as paginas que a pagina atual aponta
        while((token = strtok(NULL, " ")) != NULL){
            // Obtem a pagina atual
            Pagina* destino = (Pagina*)TST_search(paginas, token);
            
            // Insere a pagina atual na lista de paginas que a pagina incial aponta
            if (destino != NULL) {
                SetListaPagina(origem, destino);
            }
        }
        
    }

    // Fecha o arquivo graphs.txt
    fclose(fpGraphs);
    
    // Libera a memoria alocada
    free(buffer);
    free(graphsPath);
}

// Funcao que le os termos de uma pagina e preenche os termos
// Entrada: termos - TST com os termos
//          stopWords - TST com as stop words
//          p - pagina atual
//          diretorio - caminho do diretorio das paginas
// Saida: TST com os termos
TST* Indexador(TST* termos, TST* stopWords, Pagina* p, char* diretorio){
    // Cria o caminho para a pagina atual
    char* paginaPath = malloc((strlen(diretorio) + strlen(GetNomePagina(p)) + 1) * sizeof(char));
    strcpy(paginaPath, diretorio);
    strcat(paginaPath, GetNomePagina(p));

    // Abre o arquivo da pagina
    FILE* fpPagina = fopen(paginaPath, "r");

    // Verifica se o arquivo da pagina foi aberto corretamente
    if (fpPagina == NULL) {
        printf("Erro: %s nao encontrado.\n",paginaPath);
        exit(ERROR);
    }

    // Buffer para leitura
    size_t bufferSize = 64;
    char * buffer = (char*) malloc(bufferSize * sizeof(char));

    int i = 0;

    // Le o arquivo da pagina
    while (!feof(fpPagina)) {
        int qtdLida = getline(&buffer, &bufferSize, fpPagina);
        if(qtdLida == -1) break;
        
        char* token = strtok(buffer,"\n");
        buffer = token;

        token = strtok(buffer, " ");
        while(token != NULL){
            // Converte o token para minusculo
            STR_toLower(token);

            // Se o termo atual nao for uma stop word
            if(TST_search(stopWords, token) == NULL){

                TST* lista;
                lista = (TST*)TST_search(termos, token);

                // Se a lista estiver vazia, insere a pagina
                if(lista == NULL){
                    lista = TST_insert(lista, GetNomePagina(p), (void*)p, false);
                    termos = TST_insert(termos, token, (void*)lista, false);
                    i++;
                }
                // Se a pagina nao estiver na lista, insere
                else if((Pagina*)TST_search(lista, GetNomePagina(p)) != p){
                    lista = TST_insert(lista, GetNomePagina(p), (void*)p, false);
                    termos = TST_insert(termos, token, (void*)lista, false);
                    i++;
                }
            }

            // Avanca para o proximo termo
            token = strtok(NULL, " ");
        }
    }

    // Fecha o arquivo da pagina
    fclose(fpPagina);
    
    // Libera a memoria alocada
    free(buffer);
    free(paginaPath);

    return termos;
    
}

// Funcao que le os arquivos das paginas e preenche a arvore de termos
// Entrada: termos - TST com os termos
//          paginas - TST com as paginas
//          stopWords - TST com as stop words
//          diretorio - caminho do diretorio das paginas
// Saida: TST com os termos
TST* RecIndexador(TST* termos, TST* paginas, TST* stopWords, char* diretorio) {
    // Se a pagina atual for NULL, retorna
    if (paginas == NULL) return termos;

    // Itera recursivamente sobre a arvore
    termos = RecIndexador(termos, GetLeft(paginas), stopWords, diretorio);
    termos = RecIndexador(termos, GetMid(paginas), stopWords, diretorio);
    
    // Se nao for null, chama o indexador
    if((Pagina*)GetValue(paginas) != NULL){
        termos = Indexador(termos, stopWords, (Pagina*)GetValue(paginas), diretorio);
    }

    // Itera recursivamente sobre a arvore
    termos = RecIndexador(termos, GetRight(paginas), stopWords, diretorio);

    // Retorna a arvore de termos
    return termos;
}

// Funcao que calcula o PageRank de uma pagina
// Entrada: p - pagina atual
//         nPaginas - numero de paginas
//         impar - indica se eh impar
// Saida: PageRank atual
double CalculaPageRank(Pagina* p, int nPaginas, unsigned int impar){
    double pr = 0;

    // Se a iteracao for impar
    if(impar & 1){

        // Se o numero de links for 0, calcula o page rank de uma forma
        if(GetNLinksPagina(p) == 0){
            pr = (0.15 / (double)nPaginas) + (0.85 * GetPRPagina(p, 0));
            ListaP* l = GetListaPagina(p);
            double aux = 0;

            // Enquanto a lista nao for nula
            while(l != NULL){

                // Realiza o somatorio
                aux += (GetPRPagina(GetPaginaLista(l), 0) / (double)GetNLinksPagina(GetPaginaLista(l)));
                
                // Avanca para o proximo elemento da lista
                l = GetProxLista(l);
            }

            // Atualiza o page rank
            pr += (aux * 0.85);
        }
        else{ // Se o numero de links for diferente de 0, calcula o page rank de outra forma
            pr = (0.15 / (double)nPaginas);
            ListaP* l = GetListaPagina(p);
            double aux = 0;

            // Enquanto a lista nao for nula
            while(l != NULL){

                // Realiza o somatorio
                aux += (GetPRPagina(GetPaginaLista(l), 0) / (double)GetNLinksPagina(GetPaginaLista(l)));
                
                // Avanca para o proximo elemento da lista
                l = GetProxLista(l);
            }

            // Atualiza o page rank
            pr += (aux * 0.85);
        }

        // Seta o page rank da pagina atual na posicao 1
        SetPRPagina(p, pr, 1);
    }
    else{ // Se a iteracao for par

        // Se o numero de links for 0, calcula o page rank de uma forma
        if(GetNLinksPagina(p) == 0){
            pr = (0.15 / (double)nPaginas) + (0.85 * GetPRPagina(p, 1));
            ListaP* l = GetListaPagina(p);
            double aux = 0;

            // Enquanto a lista nao for nula
            while(l != NULL){

                // Realiza o somatorio
                aux += (GetPRPagina(GetPaginaLista(l), 1) / (double)GetNLinksPagina(GetPaginaLista(l)));
                
                // Avanca para o proximo elemento da lista
                l = GetProxLista(l);
            }

            // Atualiza o page rank
            pr += (aux * 0.85);
        }
        else{ // Se o numero de links for diferente de 0, calcula o page rank de outra forma
            pr = (0.15 / (double)nPaginas);
            ListaP* l = GetListaPagina(p);
            double aux = 0;

            // Enquanto a lista nao for nula
            while(l != NULL){

                // Realiza o somatorio
                aux += (GetPRPagina(GetPaginaLista(l), 1) / (double)GetNLinksPagina(GetPaginaLista(l)));
                
                // Avanca para o proximo elemento da lista
                l = GetProxLista(l);
            }

            // Atualiza o page rank
            pr += (aux * 0.85);
        }

        // Seta o page rank da pagina atual na posicao 0
        SetPRPagina(p, pr, 0);
    }

    // Retorna a diferenca entre o page rank atual e o anterior
    return fabs(GetPRPagina(p, 0) - GetPRPagina(p, 1));
}


// Funcao que calcula o PageRank de todas as paginas
// Entrada: paginas - TST com as paginas
//         nPaginas - numero de paginas
//        impar - indica se eh impar
// Saida: double - erro
double RecPR(TST* paginas, int nPaginas, double erro, int impar) {
    if (paginas == NULL) return erro;

    // Itera recursivamente sobre a arvore
    erro = RecPR(GetLeft(paginas), nPaginas, erro, impar);
    erro = RecPR(GetMid(paginas), nPaginas, erro, impar);

    // Se nao for null, calcula o page rank
    if((Pagina*)GetValue(paginas) != NULL){
        erro += CalculaPageRank((Pagina*)GetValue(paginas), nPaginas, impar);
    }

    // Itera recursivamente sobre a arvore
    erro = RecPR(GetRight(paginas), nPaginas, erro, impar);

    // Retorna a diferenca entre o page rank atual e o anterior
    return erro;
}

// Funcao que ajusta o page rank de todas as paginas
// Entrada: paginas - TST com as paginas
// Saida: void
void AjustaPR(TST* paginas){
    // Se a pagina atual for NULL, retorna
    if (paginas == NULL) return;

    // Itera recursivamente sobre a arvore
    AjustaPR(GetLeft(paginas));
    AjustaPR(GetMid(paginas));

    // Obtem a pagina atual
    Pagina* p = (Pagina*)GetValue(paginas);

    // Se nao for null, ajusta o page rank
    if(p != NULL){
        SetPRPagina(p, GetPRPagina(p, 1), 0);
    }

    // Itera recursivamente sobre a arvore
    AjustaPR(GetRight(paginas));
    return;
}

// Funcao que executa o power method para calcular o page rank das paginas
// Entrada: paginas - TST com as paginas
//         nPaginas - numero de paginas
// Saida: int - posicao final do page rank no vetor de page rank

int PowerMethod(TST* paginas, int nPaginas){
    unsigned int i = 1;
    double erro = 0;
    
    // Enquanto o erro for maior que o epsilon, calcula o page rank recursivamente
    while((RecPR(paginas, nPaginas, erro, (i & 1)) / nPaginas) >= EPSILON){
        erro = 0;
        i++;
    }
    i++;

    // Se i for impar, ajusta o page rank
    if(i & 1){ 
        AjustaPR(paginas);
    }
    return (i & 1);
}

// Funcao que libera uma palavra
// Entrada: word - ponteiro para a palavra
// Saida: void
void LiberaWord(void* word){
    free(word);
}

// Funcao que libera a memoria alocada de uma arvore
// Entrada: termos - ponteiro para a arvore
// Saida: void
void LiberaTermos(void* termos){
    
    // Se a arvore for nula, retorna
    if (termos == NULL) return;

    // Itera recursivamente sobre a arvore
    LiberaTermos(GetLeft(termos));
    LiberaTermos(GetMid(termos));
    LiberaTermos(GetRight(termos));

    // Libera a memoria alocada
    free(termos);
    return;
}


// Funcao que executa a intercessao entre duas arvores
// Entrada: termo1 - TST com o primeiro termo
//          termo2 - TST com o segundo termo
//          intercessao - TST com a intercessao
//          nPaginas - ponteiro para o numero de paginas
// Saida: TST com a intercessao
TST* RecIntercessao(TST* termo1, TST* termo2, TST* intercessao, int* nPaginas){
    // Se a primeira arvore for nula, retorna
    if (termo1 == NULL) return intercessao;
    
    // Itera recursivamente sobre a arvore
    intercessao = RecIntercessao(GetLeft(termo1), termo2, intercessao, nPaginas);
    intercessao = RecIntercessao(GetMid(termo1), termo2, intercessao, nPaginas);
    
    // Obtem a pagina atual
    Pagina* p = (Pagina*)GetValue(termo1);
    
    // Se a pagina nao for nula e se ela existir na segunda arvore
    if(p != NULL && TST_search(termo2, GetNomePagina(p)) != NULL){

        // Insere na intercessao
        intercessao = TST_insert(intercessao, GetNomePagina(p), (void*)p, false);
        
        // Incrementa o numero de paginas
        *nPaginas = *nPaginas + 1;
    }

    // Itera recursivamente sobre a arvore
    intercessao = RecIntercessao(GetRight(termo1), termo2, intercessao,nPaginas);
    
    // Retorna a intercessao
    return intercessao;
}

// Funcao que compara duas paginas
// Entrada: p1 - ponteiro para a primeira pagina
//          p2 - ponteiro para a segunda pagina
// Saida: resultado da comparacao
int ComparePaginas(const void *p1, const void *p2){
    Pagina** pag1 = (Pagina**) p1;
    Pagina** pag2 = (Pagina**) p2;
    if (GetPRPagina((*pag1),0) < GetPRPagina((*pag2),0)) {
        return 1;
    } else if (GetPRPagina((*pag1),0) > GetPRPagina((*pag2),0)) {
        return -1;
    }

    // Se os prs forem iguais, ordena por ordem alfabetica
    return strcmp(GetNomePagina((*pag1)), GetNomePagina((*pag2)));
}

// Funcao que preenche o vetor de paginas
Pagina** PreencheVetor(TST* intercessao, Pagina** vetor, int* posicao){
    if (intercessao == NULL) return vetor;

    // Itera recursivamente sobre a arvore
    vetor = PreencheVetor(GetLeft(intercessao), vetor, posicao);
    vetor = PreencheVetor(GetMid(intercessao), vetor, posicao);

    // Se nao for null, insere no vetor e incrementa a posicao
    Pagina* p = (Pagina*)GetValue(intercessao);
    if(p != NULL){
        vetor[*posicao] = p;
        *posicao = *posicao + 1;
    }

    // Itera recursivamente sobre a arvore
    vetor = PreencheVetor(GetRight(intercessao), vetor, posicao);

    // Retorna o vetor
    return vetor;
}

// Funcao que conta o numero de chaves de uma arvore
// Entrada: tst - arvore
//          nChaves - ponteiro para o numero de chaves
// Saida: void
void ContaNChaves(TST* tst, int* nChaves){
    if (tst == NULL) return;

    // Itera recursivamente sobre a arvore
    ContaNChaves(GetLeft(tst), nChaves);
    ContaNChaves(GetMid(tst), nChaves);

    // Se nao for null, incrementa o numero de chaves
    Pagina* p = (Pagina*)GetValue(tst);
    if(p != NULL){
        *nChaves = *nChaves + 1;
    }
    
    // Itera recursivamente sobre a arvore
    ContaNChaves(GetRight(tst), nChaves);
    return;
}

// Funcao que executa o processador de consultas
// Entrada: termos - TST com os termos
//          stopWords - TST com as stop words
// Saida: void
void ProcessadorDeConsultas(TST* termos, TST* stopWords){
    // Buffer para leitura
    size_t bufferSize = 64;
    char * buffer = (char*) malloc(bufferSize * sizeof(char));

    // Enquanto houver linhas para ler, executa o processador de consultas
    while(getline(&buffer, &bufferSize, stdin) > 0 && !feof(stdin)){
        int checkIntercessao = 0;
        // Obtem a linha atual
        char* token = strtok(buffer, "\n");

        // Obtem a busca
        char* busca = strdup(buffer);

        int nPaginas = 0;
        TST* intercessao = NULL;

        // Obtem o primeiro termo
        token = strtok(buffer, " ");
        char* termo1 = token;

        // Obtem o proximo termo
        token = strtok(NULL, " ");
        
        // Se nao houver proximo termo, so tem um termo
        if(token == NULL){
            // Case insensitive
            STR_toLower(termo1);

            // Obtem a intercessao e conta o numero de chaves
            intercessao = TST_search(termos, termo1);
            ContaNChaves(intercessao, &nPaginas);
        }
        else{
            while(TST_search(stopWords, termo1) != NULL && token != NULL){
                termo1 = token;
                token = strtok(NULL, " ");
            }
            while(token != NULL && TST_search(stopWords, token) != NULL){
                token = strtok(NULL, " ");
            }
            if(token == NULL){
                //so tem um termo valido
                STR_toLower(termo1);
                intercessao = TST_search(termos, termo1);
                ContaNChaves(intercessao, &nPaginas);
            }
            else{
                STR_toLower(termo1);
                STR_toLower(token);
                TST* termo1Pag = TST_search(termos, termo1);
                TST* termo2Pag = TST_search(termos, token);
                if(termo1Pag == NULL || termo2Pag == NULL){
                    //nao tem intercessao
                    intercessao = NULL;
                }
                else{
                    intercessao = RecIntercessao(TST_search(termos, termo1), TST_search(termos, token), NULL, &nPaginas);
                    checkIntercessao = 1; // Muito importante para liberar a memoria posteriormente
                    if(intercessao != NULL){
                        token = strtok(NULL, " ");
                        while(token != NULL){
                            if(TST_search(stopWords, token) != NULL){
                                token = strtok(NULL, " ");
                                continue;
                            }
                            TST* aux = intercessao;
                            TST* termo = TST_search(termos, token);
                            if(termo == NULL){
                                //nao tem intercessao
                                nPaginas == 0;
                                intercessao = NULL;
                                LiberaTermos(aux);
                                break;
                            }
                            nPaginas = 0;
                            intercessao = RecIntercessao(intercessao, termo, NULL, &nPaginas);
                            if(intercessao == NULL){
                                //nao tem intercessao
                                LiberaTermos(aux);
                                break;
                            }
                            LiberaTermos(aux);
                            token = strtok(NULL, " ");
                        }
                    }
                }
            }
        }

        // Imprime o resultado
        printf("search:%s\n", busca);
        printf("pages:");

        // Se nao houver intercessao, imprime o pr vazio
        if(intercessao == NULL){
            printf("\npr:\n");
        }
        else{

            int i = 0;
            // Vetor de paginas auxiliar
            Pagina** intercessaoVet = (Pagina**)malloc(nPaginas * sizeof(Pagina*));
            
            // Preenche o vetor de paginas com a intercessao
            intercessaoVet = PreencheVetor(intercessao, intercessaoVet, &i);

            // Ordena o vetor de paginas por page rank, e por ordem alfabetica em caso de empate
            qsort((void*)intercessaoVet, nPaginas, sizeof(Pagina*), ComparePaginas);

            // Imprime o nome de cada pagina
            for(i = 0;i < nPaginas - 1;i++){
                printf("%s ", GetNomePagina(intercessaoVet[i]));
            }
            // Imprime sem espaco no final
            printf("%s", GetNomePagina(intercessaoVet[i]));
            
            // Imprime o pr
            printf("\npr:");

            // Imprime cada page rank
            for(i = 0;i < nPaginas - 1;i++){
                printf("%lf ", GetPRPagina(intercessaoVet[i], 0));
            }

            // Imprime sem espaco no final e quebra de linha
            printf("%lf", GetPRPagina(intercessaoVet[i], 0));
            printf("\n");

            // Libera o vetor de paginas
            free(intercessaoVet);

            // Libera a intercessao somente quando passar pelo recIntercessao
            if (checkIntercessao == 1) {
                LiberaTermos(intercessao);
            }
        }

        // Libera a string de busca
        free(busca);
        
    }

    // Libera o buffer
    free(buffer);
    return;
}