#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "../include/comthread.h"
#include "../include/execcomthread.h"

void executarComThreads(const char *arquivoA, const char *arquivoB, const char *arquivoC, const char *arquivoD, const char *arquivoE, int indice, int nthreads) {
    int *matrizA = (int *) malloc(indice * indice * sizeof(int));
    int *matrizB = (int *) malloc(indice * indice * sizeof(int));
    int *matrizC = (int *) malloc(indice * indice * sizeof(int));
    int *matrizD = (int *) malloc(indice * indice * sizeof(int));
    int *matrizE = (int *) malloc(indice * indice * sizeof(int));
    
    int divisao = indice / nthreads;
    pthread_t threads[5 + 3 * nthreads];
    
    //Leitura das Matrizes A e B
    ArqArgs readA = {arquivoA, matrizA, indice};
    ArqArgs readB = {arquivoB, matrizB, indice};
    pthread_create(&threads[0], NULL, lerMatrizThread, &readA);
    pthread_create(&threads[1], NULL, lerMatrizThread, &readB);
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    //Soma das matrizes A e B resultando em D
    SomaMultArgs somaArgs[nthreads];
    clock_t inicioSoma = clock();
    double totalSoma;
    for (int i = 0; i < nthreads; i++) {
        somaArgs[i] = (SomaMultArgs){matrizA, matrizB, matrizD, i * divisao, (i + 1) * divisao, indice};
        pthread_create(&threads[2 + i], NULL, somaMatrizesThread, &somaArgs[i]);
    }
    for (int i = 0; i < nthreads; i++) {
        pthread_join(threads[2 + i], NULL);
    }
    clock_t fimSoma = clock() - inicioSoma;
    totalSoma = ((double) (fimSoma)/CLOCKS_PER_SEC);
    
    //Escritura da matriz D
    ArqArgs writeD = {arquivoD, matrizD, indice};
    pthread_create(&threads[2 + nthreads], NULL, escreverMatrizThread, &writeD);
    pthread_join(threads[2 + nthreads], NULL);

    //Leitura da matriz C
    ArqArgs readC = {arquivoC, matrizC, indice};
    pthread_create(&threads[3 + nthreads], NULL, lerMatrizThread, &readC);
    pthread_join(threads[3 + nthreads], NULL);

    //Multiplicação das matrizes C e D resultando em E
    SomaMultArgs multArgs[nthreads];
    clock_t inicioMult = clock();
    double totalMult;
    for (int i = 0; i < nthreads; i++) {
        multArgs[i] = (SomaMultArgs){matrizC, matrizD, matrizE, i * divisao, (i + 1) * divisao, indice};
        pthread_create(&threads[4 + nthreads + i], NULL, multiplicaMatrizesThread, &multArgs[i]);
    }
    for (int i = 0; i < nthreads; i++) {
        pthread_join(threads[4 + nthreads + i], NULL);
    }
    clock_t fimMult = clock() - inicioMult;
    totalMult = ((double) (fimMult)/CLOCKS_PER_SEC);
    
    //Escritura da matriz E
    ArqArgs writeE = {arquivoE, matrizE, indice};
    pthread_create(&threads[4 + nthreads + nthreads], NULL, escreverMatrizThread, &writeE);
    pthread_join(threads[4 + nthreads + nthreads], NULL);

    //Redução da matriz E
    long int resultados[nthreads];
    ReducaoArgs redArgs[nthreads];
    clock_t inicioReducao = clock();
    double totalReducao;
    for (int i = 0; i < nthreads; i++) {
        redArgs[i] = (ReducaoArgs){matrizE, i * divisao * indice, (i + 1) * divisao * indice, &resultados[i]};
        pthread_create(&threads[5 + nthreads + nthreads + i], NULL, reducaoThread, &redArgs[i]);
    }
    long int resultadoTotalReducao = 0;
    for (int i = 0; i < nthreads; i++) {
        pthread_join(threads[5 + nthreads + nthreads + i], NULL);
        resultadoTotalReducao += resultados[i];
    }
    clock_t fimReducao = clock() - inicioReducao;
    totalReducao = ((double) (fimReducao)/CLOCKS_PER_SEC);

    printf("Redução: %ld\n", resultadoTotalReducao);
    printf("Tempo soma: %f segundos\n", totalSoma);
    printf("Tempo multiplicação: %f segundos\n", totalMult);
    printf("Tempo redução: %f segundos\n", totalReducao);

    free(matrizA);
    free(matrizB);
    free(matrizC);
    free(matrizD);
    free(matrizE);
}