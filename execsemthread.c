#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "semthread.c"
#include "execsemthread.h"

void executarSemThreads(const char *arquivoA, const char *arquivoB, const char *arquivoC, const char *arquivoD, const char *arquivoE, int indice) {
    int *matrizA = (int *) malloc(indice * indice * sizeof(int));
    int *matrizB = (int *) malloc(indice * indice * sizeof(int));
    lerMatriz(arquivoA, matrizA, indice);
    lerMatriz(arquivoB, matrizB, indice);

    int *matrizD = (int *) malloc(indice * indice * sizeof(int));
    clock_t inicioSoma = clock();
    double totalSoma;
    somaMatrizes(matrizA, matrizB, matrizD, indice);
    clock_t fimSoma = clock();
    totalSoma = ((double) (fimSoma - inicioSoma))/CLOCKS_PER_SEC;
    escreverMatriz(arquivoD, matrizD, indice);

    int *matrizC = (int *) malloc(indice * indice * sizeof(int));
    lerMatriz(arquivoC, matrizC, indice);

    int *matrizE = (int *) malloc(indice * indice * sizeof(int));
    clock_t inicioMult = clock();
    double totalMult;
    multiplicaMatrizes(matrizC, matrizD, matrizE, indice);
    clock_t fimMult = clock();
    totalMult = ((double) (fimMult - inicioMult)/CLOCKS_PER_SEC);
    escreverMatriz(arquivoE, matrizE, indice);

    clock_t inicioReducao = clock();
    double totalReducao;
    reducao(matrizE, indice * indice);
    clock_t fimReducao = clock();
    totalReducao = ((double) (fimReducao - inicioReducao)/CLOCKS_PER_SEC);
    
    printf("Tempo soma: %f segundos\n", totalSoma);
    printf("Tempo multiplicacao: %f segundos\n", totalMult);
    printf("Tempo reducao: %f segundos\n", totalReducao);

    free(matrizA);
    free(matrizB);
    free(matrizC);
    free(matrizD);
    free(matrizE);

}