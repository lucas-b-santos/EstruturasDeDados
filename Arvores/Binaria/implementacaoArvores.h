#ifndef IMPLEMENTACAOARVORES_H_INCLUDED
#define IMPLEMENTACAOARVORES_H_INCLUDED

#include <iostream>
#include <stdlib.h>

using namespace std;

class No
{
private:
    No *esq, *dir;
    int chave;

public:
    No(int chave)
    {
        this->chave = chave;
        esq = 0;
        dir = 0;
    }

    int getChave()
    {
        return chave;
    }

    No *getEsq()
    {
        return esq;
    }
    No *getDir()
    {
        return dir;
    }

    void setEsq(No *no)
    {
        esq = no;
    }

    void setDir(No *no)
    {
        dir = no;
    }
};

class Arvore
{
private:
    No *raiz;

    void inserirAux(No *no, int chave)
    {
        if (chave < no->getChave())
        {
            if (no->getEsq() == 0)
            {
                No *novo_no = new No(chave);
                no->setEsq(novo_no);
            }
            else
            {
                inserirAux(no->getEsq(), chave);
            }
        }
        else if (chave > no->getChave())
        {
            if (no->getDir() == 0)
            {
                No *novo_no = new No(chave);
                no->setDir(novo_no);
            }
            else
            {
                inserirAux(no->getDir(), chave);
            }
        }
        else if (chave == no->getChave())
        {
            cout << "Numero ja existe..." << endl;
        }
    }

    No* remove_atual(No *atual)
    {
        No *no1, *no2;

        if (atual->getEsq() == 0)
        {
            no2 = atual->getDir();
            free(atual);
            return no2;
        }

        no1 = atual;
        no2 = atual->getEsq();

        while (no2->getDir() != 0)
        {
            no1 = no2;
            no2 = no2->getDir();
        }

        if (no1 != atual)
        {
            no1->setDir(no2->getEsq());
            no2->setEsq(atual->getEsq());
        }

        no2->setDir(atual->getDir());
        free(atual);
        return no2;
    }

public:
    Arvore()
    {
        raiz = 0;
    }

    void inserir(int chave)
    {
        if (raiz == 0)
            raiz = new No(chave);
        else
            inserirAux(raiz, chave);
    }

    No *getRaiz()
    {
        return raiz;
    }

    void emOrdem(No *no)
    {
        if (no != 0)
        {
            emOrdem(no->getEsq());
            cout << no->getChave() << " ";
            emOrdem(no->getDir());
        }
    }

    void preOrdem(No *no)
    {
        if (no != 0)
        {
            cout << no->getChave() << " ";
            preOrdem(no->getEsq());
            preOrdem(no->getDir());
        }
    }

    void posOrdem(No *no)
    {
        if (no != 0)
        {
            posOrdem(no->getEsq());
            posOrdem(no->getDir());
            cout << no->getChave() << " ";
        }
    }

    int remover(int chave) {
        No *ant = 0;
        No *atual = raiz;

        while (atual != 0) {
            if (chave == atual->getChave()) {
                if (atual == raiz) {
                    raiz = remove_atual(atual);
                } else {
                    if (ant->getDir() == atual) 
                        ant->setDir(remove_atual(atual));
                    else
                        ant->setEsq(remove_atual(atual));
                }
                return 1;
            } 
            ant = atual;

            if (chave > atual->getChave())
                atual = atual->getDir();
            else
                atual = atual->getEsq();
        }

        return 0;

    }
};

#endif // IMPLEMENTACAOARVORES_H_INCLUDED
