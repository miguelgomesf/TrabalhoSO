#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int i, j, k = 0;

void preencherMatriz(const char *nomeArquivo, float *matriz, int indice) {
  FILE *arquivo = fopen(nomeArquivo, "wb");
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo %s para escrita.\n", nomeArquivo);
    exit(1);
  }

  for (int i = 0; i < indice; i++) {
    for (int j = 0; j < indice; j++) {
      matriz[i * indice + j] = rand() / (float) RAND_MAX;
    }
  }

  fwrite(matriz, sizeof(float), indice * indice, arquivo);
  fclose(arquivo);
}


void lerMatriz(const char *nomeArquivo, float *matriz, int indice) {
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        exit(1);
    }
    fread(matriz, sizeof(float), indice, arquivo);
    fclose(arquivo);
}

void escreverMatriz(const char *nomeArquivo, float *matriz, int indice) {
    FILE *arquivo = fopen(nomeArquivo, "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        exit(1);
    }
    fwrite(matriz, sizeof(float), indice, arquivo);
    fclose(arquivo);
}

void somaMatrizes(float *matriz1, float *matriz2, float *matrizResultado, int indice) {
    for(i = 0; i < indice; i++) {
        matrizResultado[i] = matriz1[i] + matriz2[i];
    }
}

void multiplicaMatrizes(float *matriz1, float *matriz2, float *matrizResultado, int indice) {
    for (i = 0; i < indice; i++) {
        for (j = 0; j < indice; j++)
        {
            float calculo = 0;
            for (int k = 0; k < indice; k++) {
                calculo += matriz1[i * indice + k] * matriz2[k * indice + j];
            }
            matrizResultado[i * indice + j] = calculo;
        } 
    }
}

void reducao(float *matriz, int indice) {
    int resultadoReducao = 0;
    for (i = 0; i < indice; i++) {
            resultadoReducao += matriz[i]; 
    }
}

int main(int argc, char *argv[]) {

    int t = atoi(argv[1]);
    int n = atoi(argv[2]);

    int tamanho = n * n;

    float *matrizA, *matrizB;
    matrizA = (float *) malloc(tamanho * sizeof(float));
    matrizB = (float *) malloc(tamanho * sizeof(float));

    preencherMatriz(argv[3], matrizA, n);
    preencherMatriz(argv[3], matrizB, n);

    lerMatriz(argv[3], matrizA, tamanho);
    lerMatriz(argv[4], matrizB, tamanho);

    float *matrizD;
    matrizD = (float *) malloc(tamanho * sizeof(float));
    somaMatrizes(matrizA, matrizB, matrizD, n);
    escreverMatriz(argv[6], matrizD, tamanho);

    float *matrizC;
    matrizC = (float *) malloc(tamanho * sizeof(float));
    preencherMatriz(argv[3], matrizC, n);
    lerMatriz(argv[5], matrizC, tamanho);

    float *matrizE;
    matrizE = (float *) malloc(tamanho * sizeof(float));
    multiplicaMatrizes(matrizC, matrizD, matrizE, n);
    escreverMatriz(argv[7], matrizE, tamanho);

    free(matrizA);
    free(matrizB);
    free(matrizC);
    free(matrizD);
    free(matrizE);

    return 0;
}