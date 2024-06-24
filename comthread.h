#ifndef COMTHREAD_H
#define COMTHREAD_H

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

void *lerMatrizThread(void *arg);
void *escreverMatrizThread(void *arg);
void *somaMatrizesThread(void *arg);
void *multiplicaMatrizesThread(void *arg);
void *reducaoThread(void *arg);

#endif