#include <stdio.h>
#include <stdlib.h>
#include "comthread.h"

void *lerMatrizThread(void *arg) {
    ArqArgs *args = (ArqArgs *)arg;
    FILE *arquivo = fopen(args->nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", args->nomeArquivo);
        exit(1);
    }
    for (int i = 0; i < args->indice; i++) {
        for (int j = 0; j < args->indice; j++) {
            fscanf(arquivo, "%d", &args->matriz[i * args->indice + j]);
        }
    }
    fclose(arquivo);
}

void *escreverMatrizThread(void *arg) {
    ArqArgs *args = (ArqArgs *)arg;
    FILE *arquivo = fopen(args->nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", args->nomeArquivo);
        exit(1);
    }
    for (int i = 0; i < args->indice; i++) {
        for (int j = 0; j < args->indice; j++) {
            fprintf(arquivo, "%d", args->matriz[i * args->indice + j]);
            if (j < args->indice - 1) {
                fprintf(arquivo, " ");
            }
        }
        if (i < args->indice - 1) {
            fprintf(arquivo, "\n");
        }
    }
    fclose(arquivo);
}

void *somaMatrizesThread(void *arg) {
    SomaMultArgs *args = (SomaMultArgs *)arg;
    for (int i = args->inicio; i < args->fim; i++) {
        for (int j = 0; j < args->indice; j++) {
            args->matrizResultado[i * args->indice + j] = args->matriz1[i * args->indice + j] + args->matriz2[i * args->indice + j];
        }
    }
    return NULL;
}

void *multiplicaMatrizesThread(void *arg) {
    SomaMultArgs *args = (SomaMultArgs *)arg;
    for (int i = args->inicio; i < args->fim; i++) {
        for (int j = 0; j < args->indice; j++) {
            int calculo = 0;
            for (int k = 0; k < args->indice; k++) {
                calculo += args->matriz1[i * args->indice + k] * args->matriz2[k * args->indice + j];
            }
            args->matrizResultado[i * args->indice + j] = calculo;
        }
    }
    return NULL;
}

void *reducaoThread(void *arg) {
    ReducaoArgs *args = (ReducaoArgs *)arg;
    long int resultadoReducao = 0;
    for (int i = args->inicio; i < args->fim; i++) {
            resultadoReducao += args->matriz[i]; 
    }
    *args->resultado = resultadoReducao;
    return NULL;
}