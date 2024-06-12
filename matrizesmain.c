#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void lerMatriz(const char *nomeArquivo, float *matriz, int indice) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        exit(1);
    }
    for (int i = 0; i < indice; i++) {
        for (int j = 0; j < indice; j++) {
            fscanf(arquivo, "%f", &matriz[i * indice + j]);
        }
    }
    fclose(arquivo);
}

void escreverMatriz(const char *nomeArquivo, float *matriz, int indice) {
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        exit(1);
    }
    for (int i = 0; i < indice; i++) {
        for (int j = 0; j < indice; j++) {
            fprintf(arquivo, "%.2f ", matriz[i * indice + j]);
        }
        fprintf(arquivo, "\n");
    }
    fclose(arquivo);
}


void somaMatrizes(float *matriz1, float *matriz2, float *matrizResultado, int indice) {
    for(int i = 0; i < indice; i++) {
        for(int j = 0; j < indice; j++) {
        matrizResultado[i * indice + j] = matriz1[i * indice + j] + matriz2[i * indice + j];
        }
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

int main(int argc, char *argv[]) {
    if (argc < 8) {
    printf("Erro: Número incorreto de argumentos.\n");
    return 1;
  }

    int t = atoi(argv[1]);
    int n = atoi(argv[2]);
    int tamanho = n * n;

    srand(time(NULL));

    float *matrizA;
    float *matrizB;
    matrizA = (float *) malloc(n * n * sizeof(float));
    matrizB = (float *) malloc(n * n * sizeof(float));
    lerMatriz(argv[3], matrizA, n);
    lerMatriz(argv[4], matrizB, n);

    float *matrizD;
    matrizD = (float *) malloc(n * n * sizeof(float));
    somaMatrizes(matrizA, matrizB, matrizD, n);
    escreverMatriz(argv[6], matrizD, n);

    float *matrizC;
    matrizC = (float *) malloc(n * n * sizeof(float));
    lerMatriz(argv[5], matrizC, n);

    float *matrizE;
    matrizE = (float *) malloc(n * n * sizeof(float));
    multiplicaMatrizes(matrizC, matrizD, matrizE, n);
    escreverMatriz(argv[7], matrizE, n);

    reducao(matrizE, tamanho);

    free(matrizA);
    free(matrizB);
    free(matrizC);
    free(matrizD);
    free(matrizE);
    
    return 0;
}