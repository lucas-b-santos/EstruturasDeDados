#include "SLList.h"

#define num_vertices 5

int procuraMenorDistancia(float *dist, int *visitado)
{
   int menor = -1, primeiro = 1;
   for (int i = 0; i < num_vertices; i++) {
    if (dist[i] >= 0 && visitado[i] == 0) {
        if(primeiro) {
            menor = i;
            primeiro = 0;
        } else {
            if (dist[menor] > dist[i])
                menor = i;
        }
    }

   }
   return menor;
}

void menorCaminho(SLList *gr, int ini, int *visitados, int *ant, float *dist) {
    dist[ini] = 0;
    int indice, vert, cont = num_vertices;
    while (cont > 0) {
        vert = procuraMenorDistancia(dist, visitados);
        if (vert == -1)
            break;

        visitados[vert] = 1;
        cont--;
        for (int i = 0; i < gr[vert].length; i++) {
            indice = gr[vert].getElemento(i);
            if(dist[indice] < 0) {
                dist[indice] = dist[vert] + 1;
                ant[indice] = vert;
            } else {
                if (dist[indice] > dist[vert] + 1) {
                    dist[indice] = dist[vert] + 1;
                    ant[indice] = vert;
                }
            }
        }
    }
}

int main()
{
    SLList grafo[num_vertices];
    int visitados[num_vertices], ant[num_vertices];
    float dist[num_vertices];

    for (int i = 0; i < num_vertices; i++) {
            visitados[i] = 0;
            ant[i] = -1;
            dist[i] = -1;
    }

    //0 = TOLEDO
    //4 = FOZ DO IGUAÇU

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

//    menorCaminho(grafo, 2, visitados, ant, dist);
//
//    cout << "ARRAY DISTANCIAS = | ";
//
//        for (int i = 0; i < num_vertices; i++)
//    {
//        cout << dist[i] << " | ";
//    }
//
//    cout << endl;
//
//    cout << "ARRAY ANTERIOR = | ";
//
//    for (int i = 0; i < num_vertices; i++)
//    {
//        cout << ant[i] << " | ";
//    }
//
//    cout << endl;
//
//    cout << "ARRAY VISITADO = | ";
//
//    for (int i = 0; i < num_vertices; i++)
//    {
//        cout << visitados[i] << " | ";
//    }
//
//    cout << endl;
}
