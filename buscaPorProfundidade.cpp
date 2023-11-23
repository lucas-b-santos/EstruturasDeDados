#include "SLList.h"

int buscaProfundidade(SLList *grafo, int *ini, int visitados, int cont) {
    visita[ini] = cont;
    for (int i=0; i<grafo[i].length; i++) {
        if(!visitado[grafo])
    }
}

void buscaProfundidade_Grafo(SSList *grafo, int ini, int *visitados) {
    int cont = 1;

}

int main()
{
    SLList grafo[5];
    int visitados[5] = {0, 0, 0, 0, 0};
    grafo[0].addToResto(1);
    grafo[1].addToResto(2);
    grafo[1].addToResto(3);
    grafo[1].addToResto(4);
    grafo[2].addToResto(4);
    grafo[2].addToResto(5);
    grafo[3].addToResto(0);
    grafo[3].addToResto(4);
    cout << buscaProfundidade(grafo, visitados);

}
