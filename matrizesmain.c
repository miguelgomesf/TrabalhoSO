#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    const char *nomeArquivo;
    float *matriz;
    int indice;
} ArqArgs;

void *lerMatriz(void *arg) {
    ArqArgs *args = (ArqArgs *)arg;
    FILE *arquivo = fopen(args->nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", args->nomeArquivo);
        exit(1);
    }
    for (int i = 0; i < args->indice; i++) {
        for (int j = 0; j < args->indice; j++) {
            fscanf(arquivo, "%f", &args->matriz[i * args->indice + j]);
        }
    }
    fclose(arquivo);
}

void *escreverMatriz(void *arg) {
    ArqArgs *args = (ArqArgs *)arg;
    FILE *arquivo = fopen(args->nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", args->nomeArquivo);
        exit(1);
    }
    for (int i = 0; i < args->indice; i++) {
        for (int j = 0; j < args->indice; j++) {
            fprintf(arquivo, "%.2f ", args->matriz[i * args->indice + j]);
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

    pthread_t threads[5 + t];

    float *matrizA;
    float *matrizB;
    matrizA = (float *) malloc(n * n * sizeof(float));
    matrizB = (float *) malloc(n * n * sizeof(float));

    ArqArgs readA = {argv[3], matrizA, n};
    ArqArgs readB = {argv[4], matrizB, n};
    pthread_create(&threads[0], NULL, lerMatriz, &readA);
    pthread_create(&threads[1], NULL, lerMatriz, &readB);
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    float *matrizD;
    matrizD = (float *) malloc(n * n * sizeof(float));
    somaMatrizes(matrizA, matrizB, matrizD, n);
    ArqArgs writeD = {argv[6], matrizD, n};
    pthread_create(&threads[2], NULL, escreverMatriz, &writeD);
    pthread_join(threads[2], NULL);


    float *matrizC;
    matrizC = (float *) malloc(n * n * sizeof(float));
    ArqArgs readC = {argv[5], matrizC, n};
    pthread_create(&threads[3], NULL, lerMatriz, &readC);
    pthread_join(threads[3], NULL);


    float *matrizE;
    matrizE = (float *) malloc(n * n * sizeof(float));
    multiplicaMatrizes(matrizC, matrizD, matrizE, n);
    ArqArgs writeE = {argv[7], matrizE, n};
    pthread_create(&threads[4], NULL, escreverMatriz, &writeE);
    pthread_join(threads[4], NULL);

    reducao(matrizE, tamanho);

    free(matrizA);
    free(matrizB);
    free(matrizC);
    free(matrizD);
    free(matrizE);
    
    return 0;
}