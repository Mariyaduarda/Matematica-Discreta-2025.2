/*
    Trabalho Matemática Discreta, Integrantes:

    - Sofia Arantes da Mata 0117039
    - Dalila Silva de Almeida 0117250
    - Yris Beatriz Silva 0117244
    - Maria Eduarda Siqueira Silva 0076969

    Para rodar, primeiramente entramos na pasta do projeto e rodamos 
    gcc -lm check-closure.c -o check-closure.bin
    ./check-closure.bin entrada.txt saida

    Funções auxiliares:

    // Mensagem de erro quando o arquivo não é compatível
    void erro_arquivo();
    
    // Aloca memória suficiente para uma matriz de tamanho nxn
    int **alocar_matriz(int n);

    // Libera a memória que foi alocada
    void liberar_matriz(int **matriz);
    
    // Lemos o arquivo enviado via linha de comando, aqui que mostramos os erros de formato
    void ler_arquivo(FILE* fp, TipoRelacao* relacao);
    
    // Imprime a relação da matriz original normalmente no arquivo de saída, e imprime os 
    // arcos a serem adicionados para o fecho em vermelho
    void gerar_dot(TipoRelacao *relacao, char *arquivo_saida, int **matriz_fecho);

    // Confere se a relação lida é reflexiva, logo, se xRx
    int reflexiva(TipoRelacao *relacao);

    // Recebe a struct para rodar a matriz original e procurar em quais pontos não 
    // há xRx, nesse ponto ele preenche essa posição na matriz de fecho(que foi instanciada 
    // como zero), e ao final retorna essa matriz com os arcos necessários para a relação
    // ser reflexiva
    int** fecho_reflexivo(TipoRelacao *relacao);

    // Confere se a relação lida é simetrica, logo, se xRy então yRx
    int simetrica(TipoRelacao *relacao);

    // Recebe a struct para rodar a matriz original e procurar em quais pontos xRy mas não 
    // há yRx, nesse ponto ele preenche essa posição na matriz de fecho(que foi instanciada 
    // como zero), e ao final retorna essa matriz com os arcos necessários para a relação
    // ser simetrica
    int** fecho_simetrico(TipoRelacao *relacao);

    // Confere se a relação lida é transitiva, logo, se xRy e yRz então xRz
    int transitiva(TipoRelacao *relacao);

    // Recebe a struct para rodar a matriz original e procurar em quais pontos xRy, yRz mas
    // não há xRz, nesse ponto ele preenche essa posição na matriz de fecho(que foi instanciada 
    // como zero), e ao final retorna essa matriz com os arcos necessários para a relação
    // ser transitiva
    int** fecho_transitivo(TipoRelacao *relacao);
*/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

typedef struct
{
    int tam_conjunto;
    int** matriz;
    int reflexiva;
    int simetrica;
    int transitiva;
}TipoRelacao;

// Mensagem de Erro
void erro_arquivo() {
    printf("[ERRO] Arquivo incompatível.\n");
    exit(0);
}

// Alocar matriz
int **alocar_matriz(int n) {
    // Ponteiros das linhas
    int **matriz = malloc(n * sizeof(int*));     
    int *dados = malloc(n * n * sizeof(int));    

    for (int i = 0; i < n; i++) {
        // Aponta para cada linha
        matriz[i] = dados + (i * n); 
    }

    return matriz;
}

// Liberar a memória da matriz 
void liberar_matriz(int **matriz) {
    free(matriz[0]); 
    free(matriz);
} 

void ler_arquivo(FILE* fp, TipoRelacao* relacao){
    // (x, y) das relações
    int x, y;

    // Lê o tamanho do conjunto(n), se não houver "n numero", retorna erro
    if (fscanf(fp, "n %d", &relacao->tam_conjunto) != 1){
        erro_arquivo();
    }
    // Detecta se o resto da linha está vazia
    int c;
    while ((c = fgetc(fp)) != '\n' && c != EOF) {
        // Se não for espaço, retorna erro
        if (!isspace(c)) {
            erro_arquivo();
        }
    }
    
    printf("[SUCESSO] Tamanho do conjunto obtido: %d\n", relacao->tam_conjunto);

    // Aloca a matriz n x n
    relacao->matriz = alocar_matriz(relacao->tam_conjunto);

    // Inicializa como 0
    for (int i = 0; i < relacao->tam_conjunto; i++)
        for (int j = 0; j < relacao->tam_conjunto; j++)
            relacao->matriz[i][j] = 0;

    char letra;
    // Lê as relações(r) e confere se tem dois números depois do r
    while ((letra = fgetc(fp)) != EOF) {
        if (letra == 'r') {
            // Se a relação começa com r mas tem mais que x e y, retorna erro
            if (fscanf(fp, " %d %d", &x, &y) != 2) {
                erro_arquivo();
            }
            // Se x ou y for maior que o tamanho do conjunto, retorna erro
            // Em um conjunto com 5 nós não tem como se relacionar com um nó 6, afinal ele não existe
            if (x < 1 || x > relacao->tam_conjunto || y < 1 || y > relacao->tam_conjunto) {
                erro_arquivo();
            }
            relacao->matriz[x-1][y-1] = 1;
        }
    }
    
    printf("Matriz binária: \n");
    for (int i = 0; i < relacao->tam_conjunto; i++){
        for (int j = 0; j < relacao->tam_conjunto; j++){
            printf(" %d ", relacao->matriz[i][j]);
        }
        printf("\n");
    }
}

// Imprime a relação da matriz original normalmente no arquivo de saída, e imprime os 
// arcos a serem adicionados para o fecho em vermelho
void gerar_dot(TipoRelacao *relacao, char *arquivo_saida, int **matriz_fecho) {
    FILE *fp = fopen(arquivo_saida, "w");
    if (!fp) {
        printf("Erro ao criar arquivo DOT.\n");
        return;
    }

    fprintf(fp, "digraph Relacao {\n");

    int n = relacao->tam_conjunto;

    // Nós
    for (int i = 1; i <= n; i++) {
        fprintf(fp, "    %d;\n", i);
    }

    fprintf(fp, "\n");

    // Arestas (relações)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (relacao->matriz[i][j] == 1) {
                fprintf(fp, "    %d -> %d;\n", i + 1, j + 1);
            }
        }
    }

    // Arestas (relações) do fecho
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matriz_fecho[i][j] == 1) {
                fprintf(fp, "    %d -> %d [color=red];\n", i + 1, j + 1);
            }
        }
    }

    fprintf(fp, "}\n");
    fclose(fp);

    printf("[SUCESSO] Arquivo DOT gerado: %s\n", arquivo_saida);
}

// ================= Reflexiva ===================

// A relação R em um conjunto A é chamada de reflexiva se (x, x) ∈ R para
// todo elemento x ∈ A.
int reflexiva(TipoRelacao *relacao){
    for (int i = 0; i < relacao->tam_conjunto; i++) {
        if ((relacao->matriz[i][i]) == 0){
            return 0;
        }
    }
    return 1;
}

// Recebe uma struct de TipoRelacao e retorna uma matriz com apenas os arcos necessários para a 
// matriz original ser uma relação reflexiva
int** fecho_reflexivo(TipoRelacao *relacao) {
    // Aloca matriz do fecho
    int **matriz_fecho = alocar_matriz(relacao->tam_conjunto);

    // Inicializa tudo com 0
    for (int i = 0; i < relacao->tam_conjunto; i++)
        for (int j = 0; j < relacao->tam_conjunto; j++)
            matriz_fecho[i][j] = 0;

    // Preenche os arcos necessários para que ela se torne uma relação reflexiva
    for (int i = 0; i < relacao->tam_conjunto; i++) {
        if (relacao->matriz[i][i] == 0) {
            matriz_fecho[i][i] = 1;
        }
    }

    return matriz_fecho;
}

// ================= Simétrica ===================

// Uma relação R em um conjunto A é chamada de simétrica se (y, x) ∈ R
// sempre que (x, y) ∈ R.
int simetrica(TipoRelacao *relacao){
    for (int i = 0; i < relacao->tam_conjunto; i++) {
        for (int j = 0; j < relacao->tam_conjunto; j++) {
            // Percorre todos, se tem ida, entra no if
            if ((relacao->matriz[i][j]) == 1){
                // Se não tem volta, retorna zero
                if ((relacao->matriz[j][i]) == 0){
                    return 0;
                }   
            }
        }
    }
    // Se todas as idas voltaram, retorna sucesso
    return 1;
}

// Recebe uma struct de TipoRelacao e retorna uma matriz com apenas os arcos necessários para a 
// matriz original ser uma relação simetrica
int** fecho_simetrico(TipoRelacao *relacao) {
    // Aloca matriz do fecho
    int **matriz_fecho = alocar_matriz(relacao->tam_conjunto);

    // Inicializa tudo com 0
    for (int i = 0; i < relacao->tam_conjunto; i++)
        for (int j = 0; j < relacao->tam_conjunto; j++)
            matriz_fecho[i][j] = 0;

    // Preenche os arcos necessários para que ela se torne uma relação simétrica
    for (int i = 0; i < relacao->tam_conjunto; i++) {
        for (int j = 0; j < relacao->tam_conjunto; j++) {
            // Percorre todos, se tem ida, entra no if
            if ((relacao->matriz[i][j]) == 1){
                // Se não tem volta, retorna zero
                if ((relacao->matriz[j][i]) == 0){
                    matriz_fecho[j][i] = 1;
                }   
            }
        }
    }
    
    return matriz_fecho;
}

// ================= Transitiva ===================

// Uma relação R em um conjunto A é chamada transitiva se, sempre que
// (x, y) ∈ R e (y, z) ∈ R, então (x, z) ∈ R, ∀x, y, z ∈ A.
int transitiva(TipoRelacao *relacao) {
    for (int i = 0; i < relacao->tam_conjunto; i++) {
        for (int j = 0; j < relacao->tam_conjunto; j++) {
            // Percorre todos, se xRy, entra no if
            if ((relacao->matriz[i][j]) == 1){
                for (int k = 0; k < relacao->tam_conjunto; k++) {
                    // Se yRz, entra no if
                    if ((relacao->matriz[j][k]) == 1){
                        // Se x não se relaciona com z, retorna 0
                        if ((relacao->matriz[i][k]) == 0){
                            return 0;
                        }
                    }
                }
            }
        }
    }
    // Se for transitiva, retorna sucesso
    return 1;
}

// Recebe uma struct de TipoRelacao e retorna uma matriz com apenas os arcos necessários para a 
// matriz original ser uma relação transitiva
int** fecho_transitivo(TipoRelacao *relacao) {
    // Aloca matriz do fecho
    int **matriz_fecho = alocar_matriz(relacao->tam_conjunto);

    // Inicializa tudo com 0
    for (int i = 0; i < relacao->tam_conjunto; i++)
        for (int j = 0; j < relacao->tam_conjunto; j++)
            matriz_fecho[i][j] = 0;

    // Preenche os arcos necessários para que ela se torne uma relação transitiva
    for (int i = 0; i < relacao->tam_conjunto; i++) {
        for (int j = 0; j < relacao->tam_conjunto; j++) {
            // Percorre todos, se xRy, entra no if
            if ((relacao->matriz[i][j]) == 1){
                for (int k = 0; k < relacao->tam_conjunto; k++) {
                    // Se yRz, entra no if
                    if ((relacao->matriz[j][k]) == 1){
                        // Se x não se relaciona com z, retorna 0
                        if ((relacao->matriz[i][k]) == 0){
                            matriz_fecho[i][k] = 1;
                        }
                    }
                }
            }
        }
    }
    
    return matriz_fecho;
}

int main(int argc, char **argv)
{
    char* StrEntrada;
    char* StrSaida;
    /* Testa se a quantidade de parâmetros informada esta correta */
    if(argc != 3)
    {
        printf("\nErro de Sintaxe\n");
        printf("Usar: ./check-closure.bin <entrada> <saida>\n\n");
        exit(1);
    }
    /* Obtem os parametros informados */
    StrEntrada = argv[1];
    StrSaida = argv[2];

    FILE* fp = fopen(StrEntrada, "r");
    if (!fp) return 0;

    TipoRelacao relacao;

    // Lê o arquivo e preenche a struct(tam_conjunto e matriz binária)
    ler_arquivo(fp, &relacao);

    // Preenche os dados das propriedades da relação recebida
    relacao.reflexiva = reflexiva(&relacao);
    relacao.simetrica = simetrica(&relacao);
    relacao.transitiva = transitiva(&relacao);

    // Fecho Reflexivo

    // matriz_fecho recebe a matriz preenchida na sub-rotina com os arcos necessários para a relação 
    // ter a propriedade reflexiva
    int** matriz_fecho_reflexivo = fecho_reflexivo(&relacao);
    if (relacao.simetrica == 0) {
        printf("\nMatriz Fecho Reflexivo: \n");
        for (int i = 0; i < relacao.tam_conjunto; i++){
            for (int j = 0; j < relacao.tam_conjunto; j++){
                printf(" %d ", matriz_fecho_reflexivo[i][j]);
            }
            printf("\n");
        }
    }

    // Concatena nome recebido por comando para o nome do arquivo no fecho
    char nome_saida[200];
    snprintf(nome_saida, sizeof(nome_saida), "%s-ref.dot", StrSaida);
    gerar_dot(&relacao, nome_saida, matriz_fecho_reflexivo);

    // Fecho Simétrico

    // matriz_fecho recebe a matriz preenchida na sub-rotina com os arcos necessários para a relação 
    // ter a propriedade simétrica
    int** matriz_fecho_simetrico = fecho_simetrico(&relacao);

    if (relacao.simetrica == 0) {
        printf("\nMatriz Fecho Simetrico: \n");
        for (int i = 0; i < relacao.tam_conjunto; i++){
            for (int j = 0; j < relacao.tam_conjunto; j++){
                printf(" %d ", matriz_fecho_simetrico[i][j]);
            }
            printf("\n");
        }
    }
    // Concatena nome recebido por comando para o nome do arquivo no fecho
    snprintf(nome_saida, sizeof(nome_saida), "%s-sim.dot", StrSaida);
    gerar_dot(&relacao, nome_saida, matriz_fecho_simetrico);

    // Fecho Transitivo

    // matriz_fecho recebe a matriz preenchida na sub-rotina com os arcos necessários para a relação 
    // ter a propriedade transitiva
    int** matriz_fecho_transitivo = fecho_transitivo(&relacao);

    if (relacao.transitiva == 0) {
        printf("\nMatriz Fecho Transitivo: \n");
        for (int i = 0; i < relacao.tam_conjunto; i++){
            for (int j = 0; j < relacao.tam_conjunto; j++){
                printf(" %d ", matriz_fecho_transitivo[i][j]);
            }
            printf("\n");
        }
    }
    // Concatena nome recebido por comando para o nome do arquivo no fecho
    snprintf(nome_saida, sizeof(nome_saida), "%s-tra.dot", StrSaida);
    gerar_dot(&relacao, nome_saida, matriz_fecho_transitivo);


    /* Encerra a aplicacao */
    exit(0);
}