#ifndef SLLIST_H_INCLUDED
#define SLLIST_H_INCLUDED

#include <iostream>
#include <stdlib.h>
using namespace std;

class SLLNode
{ // classe tipo do node
public:
    SLLNode()
    {             // m�todo construtor sem par�metro
        prox = 0; // s� instancia o objeto
    }
    SLLNode(int el, SLLNode *ptr = 0)
    { // m�todo construtor com par�metro
        info = el;
        prox = ptr; // intancia o objeto, coloca a informa��o e o ponteiro pr�ximo aponta para NULL (0 substitui o NULL)
    }
    int info;   // informa��o do n�
    SLLNode *prox; // ponteiro que aponta para o pr�ximo
};

class SLList
{ // classe tipo da lista
public:
    int length;

    SLList()
    {                     // m�todo construtor sem par�metro
        topo = resto = 0; // instancia os ponteiros de in�cio(topo) e fim(resto) para NULL (0 substitui o NULL)
        length = 0;
    }

    ~SLList(){};
    //_________________________________________________________________________________________________________________________________________________________________________________________//

    int isEmpty()
    {                     // fun��o que verifica se a lista est� vazia
        return topo == 0; // somente retorna essa fun��o se o ponteiro do topo(in�cio) for NULL
    }
    //_________________________________________________________________________________________________________________________________________________________________________________________//

    void addToTopo(int el)
    {                                 // fun��o de adicionar n� inicial
        topo = new SLLNode(el, topo); // adiciona o n� inicial e faz ele apontar para o topo (in�cio da lista)
        if (resto == 0)
        {                 // se a lista estiver vazia
            resto = topo; // resto (final da lista) recebe topo (in�cio da lista);
        }
        length++;
    }
    //_________________________________________________________________________________________________________________________________________________________________________________________//

    void addToResto(int el)
    { // fun��o que adiciona o n� final
        if (resto != 0)
        {                                  // se a lista n�o estiver vazia
            resto->prox = new SLLNode(el); // o ponteiro de fim(resto), que est� no ultimo n�, aponta para o elemento novo, ou seja, conecta o n� anterior ao n� inserido no fim;
            resto = resto->prox;           // o ponteiro de fim recebe esse novo elemento, ou seja, o n� inserido passa a ser o �ltimo;
        }
        else
        {                                   // se a lista estiver vazia
            topo = resto = new SLLNode(el); // tanto o in�cio quanto o fim apontar�o para o n� inserido
        }
        length++;
    }

    //_________________________________________________________________________________________________________________________________________________________________________________________//

    void deleteFromTopo()
    { // fun��o de remover n� inicial
        if (resto == 0 && topo == 0)
        { // se a lista estiver vazia s� printa que t� vazia e retorna ( poderia usar if(isEmpty()) mas seria repeti��o de c�digo )
            cout << "Lista vazia";
            return;
        }
        SLLNode *tmp = topo; // cria n� tempor�rio que recebe o in�cio da lista
        if (topo == resto)
        {                     // se s� tem um n� na lista
            topo = resto = 0; // os ponteiros inicial e final s�o atribu�dos com NULL
        }
        else
        {                      // se houver mais que um n� na lista
            topo = topo->prox; // o ponteiro incial aponta agora para o segundo n�
        }
        delete tmp; // deleta o n� tempor�rio
        length--;
    }
    //_________________________________________________________________________________________________________________________________________________________________________________________//

    void deleteFromResto()
    { // funcao de remover n� final
        if (resto == 0 && topo == 0)
        { // se a lista estiver vazia s� printa que t� vazia e retorna ( poderia usar if(isEmpty()) mas seria repeti��o de c�digo )
            cout << "Lista vazia";
            return;
        }
        if (topo == resto)
        {                     // se s� tem um n� na lista
            delete topo;      // deleta o n� que tem
            topo = resto = 0; // os ponteiros inicial e final s�o atribu�dos com NULL
        }
        else
        {                 // se houver mais de um n�
            SLLNode *tmp; // cria n� tempor�rio
            for (tmp = topo; tmp->prox != resto; tmp = tmp->prox)
            {                 // acha o pen�ltimo (antecessor do final) n� da lista
                delete resto; // deleta o n� final
            }
            resto = tmp;     // o ponteiro final agora est� no pen�ltimo elemento
            resto->prox = 0; // e continua apontando para NULL
        }
        length--;
    }

    //_________________________________________________________________________________________________________________________________________________________________________________________//

    void printALL() const
    {                              // const para sinalizar que a funcao nao altera as variaveis, mantendo-as constantes
        cout << "| "; // pula linha

        for (SLLNode *tmp = topo; tmp != 0; tmp = tmp->prox)
        {                                   // percorre at� o n� ser nulo, ou seja, at� o final da lista
            cout << tmp->info << " | -> | "; // escreve o elemento do n�
        }
        cout << "NULL |" << endl; // pula linha
    }
    //_________________________________________________________________________________________________________________________________________________________________________________________//

    bool isIn(int el) const
    {
        for (SLLNode *tmp = topo; tmp != 0; tmp = tmp->prox)
        {
            if (el == tmp->info) return true;
        }
        return false;
    }


    void addOrdenado(int el)
    {
        if (topo == resto || el <= topo->info)
        {
            if (isEmpty())
            {
                addToTopo(el);
            }
            else if (el <= topo->info)
            {
                addToTopo(el);
            }
            else
            {
                addToResto(el);
            }
            length++;
            return;
        }

        SLLNode *aux = topo;
        SLLNode *novo = new SLLNode(el);
        for (SLLNode *tmp = topo->prox; tmp != 0; tmp = tmp->prox)
        {
            if (el <= tmp->info)
            {
                aux->prox = novo;
                novo->prox = tmp;
                length++;
                return;
            }
            aux = aux->prox;
        }

        resto->prox = novo;
        resto = novo;
        length++;
    }

    //_________________________________________________________________________________________________________________________________________________________________________________________//

    void removeSelecionado(int el)
    {
        if (topo == resto || el == topo->info)
        {
            if (isEmpty())
            {
                cout << "Lista vazia..." << endl;
            }
            else if (el == topo->info)
            {
                deleteFromTopo();
            }
            else
            {
                cout << "Elemento nao encontrado..." << endl;
            }
            length--;
            return;
        }

        SLLNode *aux = topo;
        for (SLLNode *tmp = topo->prox; tmp->prox != 0; tmp = tmp->prox)
        {
            if (el == tmp->info)
            {
                aux->prox = tmp->prox;
                delete tmp;
                length--;
                return;
            }
            aux = aux->prox;
        }

        if (el == resto->info)
        {
            delete resto;
            aux->prox = 0;
            resto = aux;
            length--;
        }
        else
        {
            cout << "Elemento nao encontrado..." << endl;
        }
    }

    //_________________________________________________________________________________________________________________________________________________________________________________________//

private:
    SLLNode *topo, *resto;
};

#endif // SLLIST_H_INCLUDED
