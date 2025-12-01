/*
    Trabalho Matemática Discreta, Integrantes:

    - Sofia Arantes da Mata 0117039
    - Dalila 
    - Yris
    - Maria Eduarda
*/

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    char* StrEntrada;
    char* StrSaida;
    /* Testa se a quantidade de parâmetros informada esta correta */
    if(argc != 3)
    {
        print("\nErro de Sintaxe\n");
        print("Usar: ./check-closure.bin <entrada> <saida>\n\n");
        exit(1);
    }
    /* Obtem os parametros informados */
    StrEntrada = argv[1];
    StrSaida = argv[2];
    /* Restante do código */
    
    /* Encerra a aplicacao */
    exit(0);
}