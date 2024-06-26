#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/semthread.h"
#include "../include/execsemthread.h"

void executarSemThreads(const char *arquivoA, const char *arquivoB, const char *arquivoC, const char *arquivoD, const char *arquivoE, int indice) {
    int *matrizA = (int *) malloc(indice * indice * sizeof(int));
    int *matrizB = (int *) malloc(indice * indice * sizeof(int));
    int *matrizC = (int *) malloc(indice * indice * sizeof(int));
    int *matrizD = (int *) malloc(indice * indice * sizeof(int));
    int *matrizE = (int *) malloc(indice * indice * sizeof(int));
    
    //Leitura das matrizes A e B
    lerMatriz(arquivoA, matrizA, indice);
    lerMatriz(arquivoB, matrizB, indice);

    //Soma das matrizes A e B resultando em D
    clock_t inicioSoma = clock();
    double totalSoma;
    somaMatrizes(matrizA, matrizB, matrizD, indice);
    clock_t fimSoma = clock() - inicioSoma;
    totalSoma = ((double) (fimSoma))/CLOCKS_PER_SEC;
    
    //Escritura da matriz D
    escreverMatriz(arquivoD, matrizD, indice);

    //Leitura da matriz C
    lerMatriz(arquivoC, matrizC, indice);
    
    //Multiplicação das matrizes C e D resultando em E
    clock_t inicioMult = clock();
    double totalMult;
    multiplicaMatrizes(matrizC, matrizD, matrizE, indice);
    clock_t fimMult = clock() - inicioMult;
    totalMult = ((double) (fimMult)/CLOCKS_PER_SEC);
    
    //Escritura da matriz E
    escreverMatriz(arquivoE, matrizE, indice);

    //Redução da matriz E
    clock_t inicioReducao = clock();
    double totalReducao;
    reducao(matrizE, indice * indice);
    clock_t fimReducao = clock() - inicioReducao;
    totalReducao = ((double) (fimReducao)/CLOCKS_PER_SEC);
    
    printf("Tempo soma: %f segundos\n", totalSoma);
    printf("Tempo multiplicação: %f segundos\n", totalMult);
    printf("Tempo redução: %f segundos\n", totalReducao);

    free(matrizA);
    free(matrizB);
    free(matrizC);
    free(matrizD);
    free(matrizE);

}