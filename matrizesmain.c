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
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        exit(1);
    }
    for (int i = 0; i < indice; i++) {
        for (int j = 0; j < indice; j++) {
            fscanf(arquivo, "%f", &matriz[i]);
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
            fprintf(arquivo, "%.2f ", matriz[i]);
        }
        fprintf(arquivo, "\n");
    }
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

    preencherMatriz(matrizA, n);
    preencherMatriz(matrizB, n);

    escreverMatriz(argv[3], matrizA, tamanho);
    escreverMatriz(argv[4], matrizB, tamanho);

    lerMatriz(argv[3], matrizA, tamanho);
    lerMatriz(argv[4], matrizB, tamanho);

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