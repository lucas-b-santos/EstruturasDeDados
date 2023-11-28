#include "SLList.h"

#define num_vertices 5

void buscaLargura(SLList *gr, int ini, int *visitado, int cont = 1)
{
    SLList fila;
    int vert;
    visitado[ini] = cont;
    fila.addToResto(ini);
    while (!fila.isEmpty())
    {
        vert = fila.getElemento(0);
        fila.deleteFromTopo();
        cont++;
        for (int i = 0; i < gr[vert].length; i++)
        {
            if (!visitado[gr[vert].getElemento(i)])
            {
                fila.addToResto(gr[vert].getElemento(i));
                visitado[gr[vert].getElemento(i)] = cont;
            }
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

    cout << "ARRAY VISITADOS = | ";

    buscaLargura(grafo, 0, visitados);

    for (int i = 0; i < num_vertices; i++)
    {
        cout << visitados[i] << " | ";
    }

    cout << endl;
}
