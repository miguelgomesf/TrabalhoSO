#include <stdio.h>
#include <stdlib.h>
#include "semthread.h"

void lerMatriz(const char *nomeArquivo, int *matriz, int indice) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        exit(1);
    }
    for (int i = 0; i < indice; i++) {
        for (int j = 0; j < indice; j++) {
            fscanf(arquivo, "%d", &matriz[i * indice + j]);
        }
    }
    fclose(arquivo);
}

void escreverMatriz(const char *nomeArquivo, int *matriz, int indice) {
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        exit(1);
    }
    for (int i = 0; i < indice; i++) {
        for (int j = 0; j < indice; j++) {
            fprintf(arquivo, "%d", matriz[i * indice + j]);
            if (j < indice - 1) {
                fprintf(arquivo, " ");
            }
        }
        if (i < indice - 1) {
            fprintf(arquivo, "\n");
        }
    }
    fclose(arquivo);
}

void somaMatrizes(int *matriz1, int *matriz2, int *matrizResultado, int indice) {
    for(int i = 0; i < indice; i++) {
        for(int j = 0; j < indice; j++) {
        matrizResultado[i * indice + j] = matriz1[i * indice + j] + matriz2[i * indice + j];
        }
    }
}

void multiplicaMatrizes(int *matriz1, int *matriz2, int *matrizResultado, int indice) {
    for (int i = 0; i < indice; i++) {
        for (int j = 0; j < indice; j++)
        {
            int calculo = 0;
            for (int k = 0; k < indice; k++) {
                calculo += matriz1[i * indice + k] * matriz2[k * indice + j];
            }
            matrizResultado[i * indice + j] = calculo;
        } 
    }
}

void reducao(int *matriz, int indice) {
    long int resultadoReducao = 0;
    for (int i = 0; i < indice; i++) {
            resultadoReducao += matriz[i]; 
    }
    printf("Reducao: %ld\n", resultadoReducao);
}