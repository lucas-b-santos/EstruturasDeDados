#ifndef IMPLEMENTACAO_H_INCLUDED
#define IMPLEMENTACAO_H_INCLUDED

#include <cstring>
#include <iostream>
#include <stdlib.h>

using namespace std;

class DLLNode
{ // Classe nó da lista
public:
  DLLNode() { prox = ant = 0; }

  DLLNode(int el, DLLNode *prox = 0, DLLNode *ant = 0)
  {
    info = el;
    this->prox = prox;
    this->ant = ant;
  }

  int info;

  DLLNode *prox, *ant;
};

class IntDLList
{ // Classe lista
public:
  IntDLList()
  {
    topo = resto = 0;
    length = 0;
  }

  int isEmpty() { return topo == 0; }

  void addToTopo(int el)
  {
    if (topo == 0)
    {
      topo = new DLLNode(el);
      resto = topo;
    }
    else
    {
      topo->ant = new DLLNode(el, topo, 0);
      topo = topo->ant;
    }
    length++;
  }

  void addToResto(int el)
  {
    if (resto != 0)
    {
      resto->prox = new DLLNode(el, 0, resto);
      resto = resto->prox;
    }
    else
    {
      topo = resto = new DLLNode(el);
    }
    length++;
  }

  void deleteFromTopo()
  {
    if (resto == 0 && topo == 0)
    {
      cout << "Lista vazia";
      return;
    }
    DLLNode *tmp = topo;
    if (topo == resto)
    {
      topo = resto = 0;
    }
    else
    {
      topo = topo->prox;
      topo->ant = 0;
    }
    delete tmp;
    length--;
  }

  void deleteFromResto()
  {
    if (resto == 0 && topo == 0)
    {
      cout << "Lista vazia";
      return;
    }
    if (topo == resto)
    {
      delete topo;
      topo = resto = 0;
    }
    else
    {
      DLLNode *tmp;
      for (tmp = topo; tmp->prox != resto; tmp = tmp->prox)
      {
        delete resto;
      }
      resto = tmp;
      resto->prox = 0;
    }
    length--;
  }

  void printALL() const
  {
    for (DLLNode *tmp = topo; tmp != 0; tmp = tmp->prox)
    {
      cout << tmp->info << " |";
    }
    cout << endl;
  }

  void printALLDecrescente() const
  {
    for (DLLNode *tmp = resto; tmp != 0; tmp = tmp->ant)
    {
      cout << tmp->info << " |";
    }
    cout << endl;
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
      return;
    }

    for (DLLNode *tmp = topo->prox; tmp != 0; tmp = tmp->prox)
    {
      if (el <= tmp->info)
      {
        DLLNode *novo = new DLLNode(el);
        tmp->ant->prox = novo;
        novo->prox = tmp;
        novo->ant = tmp->ant;
        tmp->ant = novo;
        length++;
        return;
      }
    }

    addToResto(el);
  }

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
      return;
    }

    for (DLLNode *tmp = topo->prox; tmp->prox != 0; tmp = tmp->prox)
    {
      if (el == tmp->info)
      {
        tmp->ant->prox = tmp->prox;
        tmp->prox->ant = tmp->ant;
        delete tmp;
        length--;
        return;
      }
    }
    if (el == resto->info)
    {
      deleteFromResto();
    }
    else
    {
      cout << "Elemento nao encontrado..." << endl;
    }
  }

  bool isIn(int el) const
  {
    for (DLLNode *tmp = topo; tmp != 0; tmp = tmp->prox)
    {
      if (el == tmp->info)
        return true;
    }
    return false;
  }

  int getElemento(int indice) const
  {
    int cont = 0;
    for (DLLNode *tmp = topo; tmp != 0; tmp = tmp->prox)
    {
      if (cont == indice)
      {
        return tmp->info;
      }
      cont++;
    }
    return 0;
  }

  int getLength() const { return length; }

private:
  DLLNode *topo, *resto;
  int length;
};

class No
{

private:
  No *esq, *dir;
  int chave;

public:
  No(int chave, int altura = 0)
  {
    this->chave = chave;
    esq = 0;
    dir = 0;
  }
  int getChave() { return chave; }
  No *getEsq() { return esq; }
  No *getDir() { return dir; }
  void setEsq(No *no) { esq = no; }
  void setDir(No *no) { dir = no; }
};

class Arvore
{
private:
  No *raiz;
  int length;

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
      cout << "Numero ja existe...";
    }
  }

  // funcao de remocao de no, recebe o no que queres remover
  No *removerNo(No *noatual)
  {
    No *no1, *no2;
    if (noatual->getEsq() == 0)
    {
      no2 = noatual->getDir();
      free(noatual);
      return no2;
    }
    no1 = noatual;
    no2 = noatual->getEsq();
    while (no2->getDir() != 0)
    {
      no1 = no2;
      no2 = no2->getDir();
    }
    if (no1 != noatual)
    {
      no1->setDir(no2->getEsq());
      no2->setEsq(noatual->getEsq());
    }
    no2->setDir(noatual->getDir());
    free(noatual);
    return no2;
  }

  int fatorBalanceamento(No *raiz) const
  {
    return retornaAltura(raiz->getDir()) - retornaAltura(raiz->getEsq());
  }

  void rotacaoSimplesL(No *no)
  {
    No *filhoR = no->getDir();
    no->setDir(filhoR->getEsq());
    filhoR->setEsq(no);
  }

  void rotacaoSimplesR(No *no)
  {
    No *filhoL = no->getEsq();
    no->setEsq(filhoL->getDir());
    filhoL->setDir(no);
  }

  void realizaBalanceamento(No *no)
  {
    if (no != 0)
    {
      arvoreBalanceada(no->getEsq());
      arvoreBalanceada(no->getDir());
      if (fatorBalanceamento(no) > 1)
      {
        if (fatorBalanceamento(no->getDir()) < 0)
        {
          rotacaoSimplesR(no->getDir());
          rotacaoSimplesL(no);
        }
        else
        {
          rotacaoSimplesL(no);
        }
      }
      else if (fatorBalanceamento(no) < -1)
      {
        if (fatorBalanceamento(no->getEsq()) > 0)
        {
          rotacaoSimplesL(no->getEsq());
          rotacaoSimplesR(no);
        }
        else
        {
          rotacaoSimplesR(no);
        }
      }
    }
  }

public:
  Arvore()
  {
    raiz = 0;
    length = 0;
  }

  void inserir(int chave)
  {
    if (raiz == 0)
    {
      raiz = new No(chave);
    }
    else
    {
      inserirAux(raiz, chave);
    }

    realizaBalanceamento(raiz);
    length++;
  }

  No *getRaiz() const { return raiz; }

  void emOrdem(No *no) const
  {
    if (no != 0)
    {
      emOrdem(no->getEsq());
      cout << no->getChave() << " ";
      emOrdem(no->getDir());
    }
  }

  void preOrdem(No *no) const
  {
    if (no != 0)
    {
      cout << no->getChave() << " ";
      preOrdem(no->getEsq());
      preOrdem(no->getDir());
    }
  }

  void posOrdem(No *no) const
  {
    if (no != 0)
    {
      posOrdem(no->getEsq());
      posOrdem(no->getDir());
      cout << no->getChave() << " ";
    }
  }
  
  int retornaAltura(No *no) const
  {
    if (no == 0)
    {
      return -1;
    }

    int altEsq = retornaAltura(no->getEsq());
    int altDir = retornaAltura(no->getDir());

    if (altEsq > altDir)
    {
      return altEsq + 1;
    }
    else
    {
      return altDir + 1;
    }
  }

  bool arvoreBalanceada(No *no) const
  {
    if (no != 0)
    {
      arvoreBalanceada(no->getEsq());
      arvoreBalanceada(no->getDir());
      if (fatorBalanceamento(no) > 1 || fatorBalanceamento(no) < -1)
      {
        return false;
      }
    }
    return true;
  }

  int removeArvore(int chave)
  {
    if (raiz == 0)
    {
      cout << "Arvore vazia..." << endl;
    }
    No *ant = 0;
    No *noatual = raiz;
    while (noatual != 0)
    {
      if (chave == noatual->getChave())
      {
        if (noatual == raiz)
        {
          raiz = removerNo(noatual);
        }
        else
        {
          if (ant->getDir() == noatual)
          {
            ant->setDir(removerNo(noatual));
          }
          else
          {
            ant->setEsq(removerNo(noatual));
          }
        }
        length--;
        return 1;
      }
      ant = noatual;
      if (chave > noatual->getChave())
      {
        noatual = noatual->getDir();
      }
      else
      {
        noatual = noatual->getEsq();
      }
    }
    return 0;
  }
};

#endif
