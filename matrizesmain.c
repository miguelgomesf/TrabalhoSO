#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    const char *nomeArquivo;
    float *matriz;
    int indice;
} ArqArgs;

typedef struct {
    float *matriz1; 
    float *matriz2;
    float *matrizResultado;
    int inicio;
    int fim;
    int indice;
} SomaMultArgs;

typedef struct {
    float *matriz;
    int inicio;
    int fim;
    float resultado;
} ReducaoArgs;

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

void *lerMatrizThread(void *arg) {
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

void *escreverMatrizThread(void *arg) {
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

void *somaMatrizesThread(void *arg) {
    SomaMultArgs *args = (SomaMultArgs *)arg;
    for (int i = args->inicio; i < args->fim; i++) {
        for (int j = 0; j < args->indice; j++) {
            args->matrizResultado[i * args->indice + j] = args->matriz1[i * args->indice + j] + args->matriz2[i * args->indice + j];
        }
    }
    return NULL;
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

void *multiplicaMatrizesThread(void *arg) {
    SomaMultArgs *args = (SomaMultArgs *)arg;
    for (int i = args->inicio; i < args->fim; i++) {
        for (int j = 0; j < args->indice; j++) {
            float calculo = 0;
            for (int k = 0; k < args->indice; k++) {
                calculo += args->matriz1[i * args->indice + k] * args->matriz2[k * args->indice + j];
            }
            args->matrizResultado[i * args->indice + j] = calculo;
        }
    }
    return NULL;
}

void reducao(float *matriz, int indice) {
    float resultadoReducao = 0;
    for (int i = 0; i < indice; i++) {
            resultadoReducao += matriz[i]; 
    }
    printf("Reducao: %.2f\n", resultadoReducao);
}

void *reducaoThread(void *arg) {
    ReducaoArgs *args = (ReducaoArgs *)arg;
    float resultadoReducao = 0;
    for (int i = args->inicio; i < args->fim; i++) {
            resultadoReducao += args->matriz[i]; 
    }
    args->resultado = resultadoReducao;
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 8) {
    printf("Erro: Número incorreto de argumentos.\n");
    return 1;
  }

    int t = atoi(argv[1]);
    int n = atoi(argv[2]);
    int tamanho = n * n;
    int divisao = n/t;

    pthread_t threads[5 + t];

    float *matrizA;
    float *matrizB;
    matrizA = (float *) malloc(n * n * sizeof(float));
    matrizB = (float *) malloc(n * n * sizeof(float));

    ArqArgs readA = {argv[3], matrizA, n};
    ArqArgs readB = {argv[4], matrizB, n};
    pthread_create(&threads[0], NULL, lerMatrizThread, &readA);
    pthread_create(&threads[1], NULL, lerMatrizThread, &readB);
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    float *matrizD;
    matrizD = (float *) malloc(n * n * sizeof(float));
    
    SomaMultArgs somaArgs[t];
    for (int i = 0; i < t; i++) {
        somaArgs[i] = (SomaMultArgs){matrizA, matrizB, matrizD, i * divisao, (i + 1) * divisao, n};
        pthread_create(&threads[2 + i], NULL, somaMatrizesThread, &somaArgs[i]);
    }
    for (int i = 0; i < t; i++) {
        pthread_join(threads[2 + i], NULL);
    }

    ArqArgs writeD = {argv[6], matrizD, n};
    pthread_create(&threads[2 + t], NULL, escreverMatrizThread, &writeD);
    pthread_join(threads[2 + t], NULL);

    float *matrizC;
    matrizC = (float *) malloc(n * n * sizeof(float));
    ArqArgs readC = {argv[5], matrizC, n};
    pthread_create(&threads[3 + t], NULL, lerMatrizThread, &readC);
    pthread_join(threads[3 + t], NULL);

    float *matrizE;
    matrizE = (float *) malloc(n * n * sizeof(float));
    
    SomaMultArgs multArgs[t];
    for (int i = 0; i < t; i++) {
        multArgs[i] = (SomaMultArgs){matrizC, matrizD, matrizE, i * divisao, (i + 1) * divisao, n};
        pthread_create(&threads[4 + t + i], NULL, multiplicaMatrizesThread, &multArgs[i]);
    }
    for (int i = 0; i < t; i++) {
        pthread_join(threads[4 + t + i], NULL);
    }

    ArqArgs writeE = {argv[7], matrizE, n};
    pthread_create(&threads[4 + t + t], NULL, escreverMatrizThread, &writeE);
    pthread_join(threads[4 + t + t], NULL);

    ReducaoArgs redArgs[t];
    for (int i = 0; i < t; i++) {
        redArgs[i] = (ReducaoArgs){matrizE, i * divisao, (i + 1) * divisao, 0};
        pthread_create(&threads[4 + t + t + i], NULL, reducaoThread, &redArgs[i]);
    }
    
    float resultadoTotalReducao = 0;
    for (int i = 0; i < t; i++) {
        pthread_join(threads[4 + t + t + i], NULL);
        resultadoTotalReducao += redArgs[i].resultado;
    }

    printf("Reducao: %.2f\n", resultadoTotalReducao);

    free(matrizA);
    free(matrizB);
    free(matrizC);
    free(matrizD);
    free(matrizE);
    
    return 0;
}