#include "SLList.h"

#define num_vertices 5

int procuraMenorDistancia(float *dist, int *visitado)
{
    int menor = -1, primeiro = 1;
    for (int i = 0; i < num_vertices; i++)
    {
        if (dist[i] >= 0 && visitado[i] == 0)
        {
            if (primeiro)
            {
                menor = i;
                primeiro = 0;
            }
            else
            {
                if (dist[menor] > dist[i])
                    menor = i;
            }
        }
    }
    return menor;
}

SLList menorCaminho(SLList *gr, int ini, int destino, int *visitados, int *ant, float *dist)
{

    dist[ini] = 0;
    int indice, vert;
    SLList rota;

    rota.addToTopo(destino);

    for (int cont = 0; cont < num_vertices; cont++)
    {
        vert = procuraMenorDistancia(dist, visitados);
        if (vert == -1)
            break;

        visitados[vert] = 1;
        for (int i = 0; i < gr[vert].length; i++)
        {
            indice = gr[vert].getElemento(i);
            if (dist[indice] < 0)
            {
                dist[indice] = dist[vert] + gr[vert].getPeso(i);
                ant[indice] = vert;
            }
            else
            {
                if (dist[indice] > dist[vert] + gr[vert].getPeso(i))
                {
                    dist[indice] = dist[vert] + gr[vert].getPeso(i);
                }
            }
        }
    }

    while (rota.getElemento(0) != ini)
    {
        rota.addToTopo(ant[destino]);
        destino = ant[destino];
    }

    return rota;
}

int main()
{
    SLList grafo[num_vertices];

    int visitados[num_vertices], ant[num_vertices];
    float dist[num_vertices];

    for (int i = 0; i < num_vertices; i++)
    {
        visitados[i] = 0;
        ant[i] = -1;
        dist[i] = -1;
    }

    // 0 = TOLEDO
    // 4 = FOZ DO IGUACU

    grafo[0].addPonderado(1, 48);
    grafo[0].addPonderado(3, 33);

    grafo[1].addPonderado(0, 48);
    grafo[1].addPonderado(2, 52);

    grafo[2].addPonderado(3, 16);
    grafo[2].addPonderado(1, 52);
    grafo[2].addPonderado(4, 108);

    grafo[3].addPonderado(0, 33);
    grafo[3].addPonderado(2, 16);

    grafo[4].addPonderado(2, 108);

    for (int i = 0; i < num_vertices; i++)
    {
        cout << "| " << i << " | -> ";
        grafo[i].printALLPonderado();
    }

    SLList rota = menorCaminho(grafo, 0, 4, visitados, ant, dist);

    cout << "A seguinte lista apresenta a menor rota entre Toledo (0) e Foz (4):" << endl;

    rota.printALL();
}
