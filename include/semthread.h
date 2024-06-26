#ifndef SEMTHREAD_H
#define SEMTHREAD_H

void lerMatriz(const char *nomeArquivo, int *matriz, int indice);
void escreverMatriz(const char *nomeArquivo, int *matriz, int indice);
void somaMatrizes(int *matriz1, int *matriz2, int *matrizResultado, int indice);
void multiplicaMatrizes(int *matriz1, int *matriz2, int *matrizResultado, int indice);
void reducao(int *matriz, int indice);

#endif 