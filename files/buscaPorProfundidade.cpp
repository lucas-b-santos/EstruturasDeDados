#include "SLList.h"

#define num_vertices 6

int buscaProfundidade(int elemento, SLList *gr, int ini, int *visitado, int cont = 1) {
    visitado[ini] = cont;
    cout << cont << endl;
    for (int i = 0; i < gr[ini].length; i++) {
        if (gr[ini].getElemento(i) == elemento) return true;
        if (!visitado[gr[ini].getElemento(i)]) buscaProfundidade(gr, gr[ini].getElemento(i), visitado, cont+1);
    }
    return false;
}

int main()
{
    SLList grafo[num_vertices];
    int visitados[num_vertices];

    for (int i = 0; i < num_vertices; i++) visitados[i] = 0;

    int cont;

    grafo[0].addToResto(1);

    grafo[1].addToResto(2);
    grafo[1].addToResto(3);
    grafo[1].addToResto(4);

    grafo[2].addToResto(4);
    grafo[2].addToResto(5);

    grafo[3].addToResto(0);
    grafo[3].addToResto(4);

    for (int i = 0; i < num_vertices; i++)
    {
        cout << "| " << i << " | -> ";
        grafo[i].printALL();
    }

    cont = buscaProfundidade(grafo, 0, visitados);

}
