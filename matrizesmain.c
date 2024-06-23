#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct {
    const char *nomeArquivo;
    int *matriz;
    int indice;
} ArqArgs;

typedef struct {
    int *matriz1; 
    int *matriz2;
    int *matrizResultado;
    int inicio;
    int fim;
    int indice;
} SomaMultArgs;

typedef struct {
    int *matriz;
    int inicio;
    int fim;
    long int *resultado;
} ReducaoArgs;

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


void somaMatrizes(int *matriz1, int *matriz2, int *matrizResultado, int indice) {
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

void reducao(int *matriz, int indice) {
    long int resultadoReducao = 0;
    for (int i = 0; i < indice; i++) {
            resultadoReducao += matriz[i]; 
    }
    printf("Reducao: %ld\n", resultadoReducao);
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

void executarSemThreads(const char *arquivoA, const char *arquivoB, const char *arquivoC, const char *arquivoD, const char *arquivoE, int indice) {
    int *matrizA = (int *) malloc(indice * indice * sizeof(int));
    int *matrizB = (int *) malloc(indice * indice * sizeof(int));
    lerMatriz(arquivoA, matrizA, indice);
    lerMatriz(arquivoB, matrizB, indice);

    int *matrizD = (int *) malloc(indice * indice * sizeof(int));
    clock_t inicioSoma = clock();
    double totalSoma;
    somaMatrizes(matrizA, matrizB, matrizD, indice);
    clock_t fimSoma = clock();
    totalSoma = ((double) (fimSoma - inicioSoma))/CLOCKS_PER_SEC;
    escreverMatriz(arquivoD, matrizD, indice);

    int *matrizC = (int *) malloc(indice * indice * sizeof(int));
    lerMatriz(arquivoC, matrizC, indice);

    int *matrizE = (int *) malloc(indice * indice * sizeof(int));
    clock_t inicioMult = clock();
    double totalMult;
    multiplicaMatrizes(matrizC, matrizD, matrizE, indice);
    clock_t fimMult = clock();
    totalMult = ((double) (fimMult - inicioMult)/CLOCKS_PER_SEC);
    escreverMatriz(arquivoE, matrizE, indice);

    clock_t inicioReducao = clock();
    double totalReducao;
    reducao(matrizE, indice * indice);
    clock_t fimReducao = clock();
    totalReducao = ((double) (fimReducao - inicioReducao)/CLOCKS_PER_SEC);
    
    printf("Tempo soma: %f segundos\n", totalSoma);
    printf("Tempo multiplicacao: %f segundos\n", totalMult);
    printf("Tempo reducao: %f segundos\n", totalReducao);

    free(matrizA);
    free(matrizB);
    free(matrizC);
    free(matrizD);
    free(matrizE);

}

void executarComThreads(const char *arquivoA, const char *arquivoB, const char *arquivoC, const char *arquivoD, const char *arquivoE, int indice, int nthreads) {
    int divisao = indice / nthreads;
    pthread_t threads[5 + nthreads];

    int *matrizA = (int *) malloc(indice * indice * sizeof(int));
    int *matrizB = (int *) malloc(indice * indice * sizeof(int));

    ArqArgs readA = {arquivoA, matrizA, indice};
    ArqArgs readB = {arquivoB, matrizB, indice};
    pthread_create(&threads[0], NULL, lerMatrizThread, &readA);
    pthread_create(&threads[1], NULL, lerMatrizThread, &readB);
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    int *matrizD = (int *) malloc(indice * indice * sizeof(int));

    SomaMultArgs somaArgs[nthreads];
    clock_t inicioSoma = clock();
    double totalSoma;
    for (int i = 0; i < nthreads; i++) {
        somaArgs[i] = (SomaMultArgs){matrizA, matrizB, matrizD, i * divisao, (i + 1) * divisao, indice};
        pthread_create(&threads[2 + i], NULL, somaMatrizesThread, &somaArgs[i]);
    }
    for (int i = 0; i < nthreads; i++) {
        pthread_join(threads[2 + i], NULL);
    }
    clock_t fimSoma = clock();
    totalSoma = ((double) (fimSoma - inicioSoma)/CLOCKS_PER_SEC);
    ArqArgs writeD = {arquivoD, matrizD, indice};
    pthread_create(&threads[2 + nthreads], NULL, escreverMatrizThread, &writeD);
    pthread_join(threads[2 + nthreads], NULL);

    int *matrizC = (int *) malloc(indice * indice * sizeof(int));

    ArqArgs readC = {arquivoC, matrizC, indice};
    pthread_create(&threads[3 + nthreads], NULL, lerMatrizThread, &readC);
    pthread_join(threads[3 + nthreads], NULL);

    int *matrizE = (int *) malloc(indice * indice * sizeof(int));

    SomaMultArgs multArgs[nthreads];
    clock_t inicioMult = clock();
    double totalMult;
    for (int i = 0; i < nthreads; i++) {
        multArgs[i] = (SomaMultArgs){matrizC, matrizD, matrizE, i * divisao, (i + 1) * divisao, indice};
        pthread_create(&threads[4 + nthreads + i], NULL, multiplicaMatrizesThread, &multArgs[i]);
    }
    for (int i = 0; i < nthreads; i++) {
        pthread_join(threads[4 + nthreads + i], NULL);
    }
    clock_t fimMult = clock();
    totalMult = ((double) (fimMult - inicioMult)/CLOCKS_PER_SEC);
    ArqArgs writeE = {arquivoE, matrizE, indice};
    pthread_create(&threads[4 + nthreads + nthreads], NULL, escreverMatrizThread, &writeE);
    pthread_join(threads[4 + nthreads + nthreads], NULL);

    long int resultados[nthreads];
    ReducaoArgs redArgs[nthreads];
    clock_t inicioReducao = clock();
    double totalReducao;
    for (int i = 0; i < nthreads; i++) {
        redArgs[i] = (ReducaoArgs){matrizE, i * divisao * indice, (i + 1) * divisao * indice, &resultados[i]};
        pthread_create(&threads[5 + nthreads + nthreads + i], NULL, reducaoThread, &redArgs[i]);
    }
    long int resultadoTotalReducao = 0;
    for (int i = 0; i < nthreads; i++) {
        pthread_join(threads[5 + nthreads + nthreads + i], NULL);
        resultadoTotalReducao += resultados[i];
    }
    clock_t fimReducao = clock();
    totalReducao = ((double) (fimReducao - inicioReducao)/CLOCKS_PER_SEC);

    printf("Reducao: %ld\n", resultadoTotalReducao);
    printf("Tempo soma: %f segundos\n", totalSoma);
    printf("Tempo multiplicacao: %f segundos\n", totalMult);
    printf("Tempo reducao: %f segundos\n", totalReducao);

    free(matrizA);
    free(matrizB);
    free(matrizC);
    free(matrizD);
    free(matrizE);
}

int main(int argc, char *argv[]) {
    clock_t inicioProg = clock();
    double totalProg;
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

    clock_t fimProg = clock();
    totalProg = ((double) (fimProg - inicioProg))/CLOCKS_PER_SEC;
    printf("Tempo total: %f segundos\n", totalProg);
    return 0;
}