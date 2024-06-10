#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void preencherMatriz(float *matriz, int indice) {
  for (int i = 0; i < indice; i++) {
      matriz[i] = (float) rand() / RAND_MAX;
  }
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
    for(int i = 0; i < indice; i++) {
        matrizResultado[i] = matriz1[i] + matriz2[i];
    }
}

void multiplicaMatrizes(float *matriz1, float *matriz2, float *matrizResultado, int indice) {
    for (int i = 0; i < indice; i++) {
        for (int j = 0; j < indice; j++)
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
    float resultadoReducao = 0;
    for (int i = 0; i < indice; i++) {
            resultadoReducao += matriz[i]; 
    }
    printf("Reducao: %.2f\n", resultadoReducao);
}

void imprimirMatriz(float *matriz, int indice, const char *nome) {
    printf("Matriz %s:\n", nome);
    for (int i = 0; i < indice; i++) {
        printf("%.2f ", matriz[i]);
        if ((i + 1) % (int)sqrt(indice) == 0) {
            printf("\n");
        }
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc < 8) {
    printf("Erro: Número incorreto de argumentos.\n");
    return 1;
  }

    int t = atoi(argv[1]);
    int n = atoi(argv[2]);
    int tamanho = n * n;

    srand(time(NULL));

    float *matrizA, *matrizB;
    matrizA = (float *) malloc(tamanho * sizeof(float));
    matrizB = (float *) malloc(tamanho * sizeof(float));

    preencherMatriz(matrizA, tamanho);
    preencherMatriz(matrizB, tamanho);

    escreverMatriz(argv[3], matrizA, tamanho);
    escreverMatriz(argv[4], matrizB, tamanho);

    lerMatriz(argv[3], matrizA, tamanho);
    lerMatriz(argv[4], matrizB, tamanho);

    imprimirMatriz(matrizA, tamanho, "A");
    imprimirMatriz(matrizB, tamanho, "B");


    float *matrizD;
    matrizD = (float *) malloc(tamanho * sizeof(float));
    somaMatrizes(matrizA, matrizB, matrizD, tamanho);
    escreverMatriz(argv[6], matrizD, tamanho);

    float *matrizC;
    matrizC = (float *) malloc(tamanho * sizeof(float));
    preencherMatriz(matrizC, tamanho);
    escreverMatriz(argv[5], matrizC, tamanho);
    lerMatriz(argv[5], matrizC, tamanho);

    float *matrizE;
    matrizE = (float *) malloc(tamanho * sizeof(float));
    multiplicaMatrizes(matrizC, matrizD, matrizE, n);
    escreverMatriz(argv[7], matrizE, tamanho);

    reducao(matrizE, tamanho);

    free(matrizA);
    free(matrizB);
    free(matrizC);
    free(matrizD);
    free(matrizE);

    return 0;
}