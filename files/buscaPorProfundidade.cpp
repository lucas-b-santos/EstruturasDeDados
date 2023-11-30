#include "SLList.h"

#define num_vertices 5

void buscaProfundidade(SLList *gr, int ini, int *visitado, int cont = 1)
{
    visitado[ini] = cont;

    for (int i = 0; i < gr[ini].length; i++)
    {
        if (!visitado[gr[ini].getElemento(i)]) {
            cout << "VERTICE "<< gr[ini].getElemento(i) <<"; CONT = " << cont << endl;
            buscaProfundidade(gr, gr[ini].getElemento(i), visitado, cont + 1);
        }
    }
}

int main()
{
    SLList grafo[num_vertices];
    int visitados[num_vertices];

    for (int i = 0; i < num_vertices; i++)
        visitados[i] = 0;

    grafo[0].addToResto(1);

    grafo[1].addToResto(2);
    grafo[1].addToResto(3);

    grafo[2].addToResto(4);

    grafo[3].addToResto(0);
    grafo[3].addToResto(4);

    grafo[4].addToResto(1);

    for (int i = 0; i < num_vertices; i++)
    {
        cout << "| " << i << " | -> ";
        grafo[i].printALL();
    }

    buscaProfundidade(grafo, 0, visitados);


    cout << "ARRAY VISITADOS = | ";


    for (int i = 0; i < num_vertices; i++)
    {
        cout << visitados[i] << " | ";
    }

    cout << endl;
}
