#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "execsemthread.c"
#include "execcomthread.c"

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