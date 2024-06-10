#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int t, n;
int i, j, k = 0;

void lerMatriz(const char *nomeArquivo, float *matriz, int tamanho) {
    FILE *arquivo = fopen(*nomeArquivo, "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        exit(1);
    }
    fread(matriz, sizeof(float), tamanho, arquivo);
    fclose(arquivo);
}

void escreverMatriz(const char *nomeArquivo, float *matriz, int tamanho) {
    FILE *arquivo = fopen(*nomeArquivo, "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        exit(1);
    }
    fwrite(matriz, sizeof(float), tamanho, arquivo);
    fclose(arquivo);
}

void somaMatrizes(float *matriz1, float *matriz2, float *matrizResultado, int indice) {
    for(i = 0; i < tamanho; i++) {
        matrizResultado = matriz1 + matriz2;
    }
}

void multiplicaMatrizes(float *matriz1, float *matriz2, float *matrizResultado, int indice) {
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++)
        {
            float calculo = 0;
            for (int k = 0; k < n; k++) {
                calculo += matriz1[i * n + k] * matriz2[k * n + j];
            }
            matrizResultado[i * n + j] = calculo;
        } 
    }
}

void reducao(float *matriz, int indice) {
    int resultadoReducao = 0;
    for (i = 0; i < tamanho; i++) {
            int resultadoReducao += matriz*; 
    }
}

int main(argc, *argv[]) {
    float *matrizA, *matrizB;
    matrizA = (float *) malloc(n * n * sizeof(float));
    matrizB = (float *) malloc(n * n * sizeof(float));

    int tamanho = n * n;

    lerMatriz(arqA, matrizA, tamanho);
    lerMatriz(arqB, matrizB, tamanho);

    float *matrizD;
    matrizD = (float *) malloc(tamanho * sizeof(float));
    somaMatrizes(matrizA, matrizB, matrizD, n);
    escreverMatriz(arqD, matrizD, tamanho);

    float *matrizC;
    matrizC = (float *) malloc(tamanho * sizeof(float));
    lerMatriz(arqC, matrizC, tamanho);

    float *matrizE;
    matrizE = (float *) malloc(tamanho * sizeof(float));
    multiplicaMatrizes(matrizC, matrizD, matrizE, n);
    escreverMatriz(arqE, matrizE, tamanho);

    return 0;
}