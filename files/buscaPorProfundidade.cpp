#include "SLList.h"

int buscaProfundidade(SLList *grafo, int ini, int *visitados, int numVertices, int cont = 0)
{
    visitados[ini] = 1;
    for (int i = 0; i < numVertices; i++)
    {
        if (grafo[ini].isIn(i) && !visitados[i])
        {
            return cont + buscaProfundidade(grafo, i, visitados, numVertices, cont + 1);
        }
    }
}

int main()
{
    SLList grafo[4];
    int visitados[4] = {0, 0, 0, 0};
    int cont;
    grafo[0].addToResto(1);
    grafo[1].addToResto(2);
    grafo[1].addToResto(3);
    grafo[1].addToResto(4);
    grafo[2].addToResto(4);
    grafo[2].addToResto(5);
    grafo[3].addToResto(0);
    grafo[3].addToResto(4);
    for (int i = 0; i < 4; i++)
    {
        cout << "| " << i << " | -> ";
        grafo[i].printALL();
    }

}