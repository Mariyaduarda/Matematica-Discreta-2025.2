/*
    Trabalho Matemática Discreta, Integrantes:

    - Sofia Arantes da Mata 0117039
    - Dalila Silva de Almeida 0117250
    - Yris Beatriz Silva 0117244
    - Maria Eduarda Siqueira Silva 0076969

    Como rodar, primeiramente entramos na pasta do projeto e rodamos 
    gcc -lm check-closure.c -o check-closure.bin

    ./check-closure.bin entrada.txt saida.txt
*/

#include <stdlib.h>
#include <stdio.h>

typedef struct
{
    int tam_conjunto;
    int** matriz;
    int reflexiva;
    int simetrica;
    int antiSimetrica;
    int transitiva;
}TipoRelacao;

// Mensagem de Erro
void erro_arquivo() {
    printf("[ERRO] Arquivo incompatível.");
    exit(0);
}

// A
int **alocar_matriz(int n) {
    int **matriz = malloc(n * sizeof(int*));     // ponteiros das linhas
    int *dados = malloc(n * n * sizeof(int));    // bloco único

    for (int i = 0; i < n; i++) {
        matriz[i] = dados + (i * n); // aponta para cada linha
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

    // Lê o tamanho do conjunto(n)
    if (fscanf(fp, "n %d", &relacao->tam_conjunto) != 1){
        erro_arquivo();
    }
    
    printf("[SUCESSO] Tamanho do conjunto obtido: %d\n", relacao->tam_conjunto);

    // Aloca a matriz n x n
    relacao->matriz = alocar_matriz(relacao->tam_conjunto);

    // Inicializa como 0
    for (int i = 0; i < relacao->tam_conjunto; i++)
        for (int j = 0; j < relacao->tam_conjunto; j++)
            relacao->matriz[i][j] = 0;

    // Lê as relações(r)
    while (fscanf(fp, " r %d %d", &x, &y) == 2)
    {
        relacao->matriz[x - 1][y - 1] = 1;
    }
    
    printf("Matriz binária: \n");
    for (int i = 0; i < relacao->tam_conjunto; i++){
        for (int j = 0; j < relacao->tam_conjunto; j++){
            printf(" %d ", relacao->matriz[i][j]);
        }
        printf("\n");
    }
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

    ler_arquivo(fp, &relacao);

    /* Encerra a aplicacao */
    exit(0);
}