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
    float *resultado;
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
            fprintf(arquivo, "%.2f", matriz[i * indice + j]);
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
            fprintf(arquivo, "%.2f", args->matriz[i * args->indice + j]);
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
    *args->resultado = resultadoReducao;
    return NULL;
}

void executarSemThreads(const char *arquivoA, const char *arquivoB, const char *arquivoC, const char *arquivoD, const char *arquivoE, int indice) {
    float *matrizA = (float *) malloc(indice * indice * sizeof(float));
    float *matrizB = (float *) malloc(indice * indice * sizeof(float));
    lerMatriz(arquivoA, matrizA, indice);
    lerMatriz(arquivoB, matrizB, indice);

    float *matrizD = (float *) malloc(indice * indice * sizeof(float));
    somaMatrizes(matrizA, matrizB, matrizD, indice);
    escreverMatriz(arquivoD, matrizD, indice);

    float *matrizC = (float *) malloc(indice * indice * sizeof(float));
    lerMatriz(arquivoC, matrizC, indice);

    float *matrizE = (float *) malloc(indice * indice * sizeof(float));
    multiplicaMatrizes(matrizC, matrizD, matrizE, indice);
    escreverMatriz(arquivoE, matrizE, indice);

    reducao(matrizE, indice * indice);

    free(matrizA);
    free(matrizB);
    free(matrizC);
    free(matrizD);
    free(matrizE);

}

void executarComThreads(const char *arquivoA, const char *arquivoB, const char *arquivoC, const char *arquivoD, const char *arquivoE, int indice, int nthreads) {
    int divisao = indice / nthreads;
    pthread_t threads[5 + nthreads];

    float *matrizA = (float *) malloc(indice * indice * sizeof(float));
    float *matrizB = (float *) malloc(indice * indice * sizeof(float));

    ArqArgs readA = {arquivoA, matrizA, indice};
    ArqArgs readB = {arquivoB, matrizB, indice};
    pthread_create(&threads[0], NULL, lerMatrizThread, &readA);
    pthread_create(&threads[1], NULL, lerMatrizThread, &readB);
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    float *matrizD = (float *) malloc(indice * indice * sizeof(float));

    SomaMultArgs somaArgs[nthreads];
    for (int i = 0; i < nthreads; i++) {
        somaArgs[i] = (SomaMultArgs){matrizA, matrizB, matrizD, i * divisao, (i + 1) * divisao, indice};
        pthread_create(&threads[2 + i], NULL, somaMatrizesThread, &somaArgs[i]);
    }
    for (int i = 0; i < nthreads; i++) {
        pthread_join(threads[2 + i], NULL);
    }
    ArqArgs writeD = {arquivoD, matrizD, indice};
    pthread_create(&threads[2 + nthreads], NULL, escreverMatrizThread, &writeD);
    pthread_join(threads[2 + nthreads], NULL);

    float *matrizC = (float *) malloc(indice * indice * sizeof(float));

    ArqArgs readC = {arquivoC, matrizC, nthreads};
    pthread_create(&threads[3 + nthreads], NULL, lerMatrizThread, &readC);
    pthread_join(threads[3 + nthreads], NULL);

    float *matrizE = (float *) malloc(indice * indice * sizeof(float));

    SomaMultArgs multArgs[nthreads];
    for (int i = 0; i < nthreads; i++) {
        multArgs[i] = (SomaMultArgs){matrizC, matrizD, matrizE, i * divisao, (i + 1) * divisao, indice};
        pthread_create(&threads[4 + nthreads + i], NULL, multiplicaMatrizesThread, &multArgs[i]);
    }
    for (int i = 0; i < nthreads; i++) {
        pthread_join(threads[4 + nthreads + i], NULL);
    }
    ArqArgs writeE = {arquivoE, matrizE, indice};
    pthread_create(&threads[4 + nthreads + nthreads], NULL, escreverMatrizThread, &writeE);
    pthread_join(threads[4 + nthreads + nthreads], NULL);

    float resultados[nthreads];
    ReducaoArgs redArgs[nthreads];
    for (int i = 0; i < nthreads; i++) {
        redArgs[i] = (ReducaoArgs){matrizE, i * divisao * indice, (i + 1) * divisao * indice, &resultados[i]};
        pthread_create(&threads[4 + nthreads + nthreads + i], NULL, reducaoThread, &redArgs[i]);
    }
    
    float resultadoTotalReducao = 0;
    for (int i = 0; i < nthreads; i++) {
        pthread_join(threads[4 + nthreads + nthreads + i], NULL);
        resultadoTotalReducao += resultados[i];
    }

    printf("Reducao: %.2f\n", resultadoTotalReducao);

    free(matrizA);
    free(matrizB);
    free(matrizC);
    free(matrizD);
    free(matrizE);
}

int main(int argc, char *argv[]) {
    if (argc < 8) {
    printf("Erro: Número incorreto de argumentos.\n");
    return 1;
  }

    int t = atoi(argv[1]);
    int n = atoi(argv[2]);
    
    if(t == 1)
        executarSemThreads(argv[3], argv[4], argv[5], argv[6], argv[7], n);
    else
        executarComThreads(argv[3], argv[4], argv[5], argv[6], argv[7], n, t);
    
    return 0;
}