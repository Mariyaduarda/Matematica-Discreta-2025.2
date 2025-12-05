Trabalho Matemática Discreta desenvolvido por:

    - Sofia Arantes da Mata        | 0117039
    - Dalila Silva de Almeida      | 0117250
    - Yris Beatriz Silva           | 0117244
    - Maria Eduarda Siqueira Silva | 0076969

    Descrição: 
    Este programa analisa relações binárias em conjuntos finitos, 
    verificando se possuem as propriedades de reflexividade, simetria e transitividade. 
    Para cada propriedade não satisfeita, o programa calcula o fecho correspondente, sendo o número mínino
    de pares que devem ser adicionados para a relação ter aquela propriedade.

    Propriedades Verificadas:

    Reflexiva: Uma relação R é reflexiva se (x, x) ∈ R para todo elemento x do conjunto
    Simétrica: Uma relação R é simétrica se (y, x) ∈ R sempre que (x, y) ∈ R
    Transitiva: Uma relação R é transitiva se (x, z) ∈ R sempre que (x, y) ∈ R e (y, z) ∈ R

    Como usar:

    Para compilação:
    gcc -lm check-closure.c -o check-closure.bin

    Execução:
    ./check-closure.bin entrada.txt saida
