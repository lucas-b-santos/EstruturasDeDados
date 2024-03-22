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
  No(int chave)
  {
    this->chave = chave;
    esq = 0;
    dir = 0;
  }

  int getChave() const { return chave; }
  No *getEsq() const { return esq; }
  No *getDir() const { return dir; }

  void setEsq(No *no) { esq = no; }
  void setDir(No *no) { dir = no; }
  void setChave(int chave) { this->chave = chave; }
};

class ArvoreAVL
{
private:
  No *raiz;

  int fatorBalanceamento(No *raiz) const
  {
    return retornaAltura(raiz->getDir()) - retornaAltura(raiz->getEsq());
  }

  void rotacaoSimplesL(No *no)
  {
    No *filhoR = no->getDir();
    no->setDir(filhoR->getEsq());
    filhoR->setEsq(no);
    no = filhoR;
  }

  void rotacaoSimplesR(No *no)
  {
    No *filhoL = no->getEsq();
    no->setEsq(filhoL->getDir());
    filhoL->setDir(no);
    no = filhoL;
  }

  void auxRotacoesInserir(No *no)
  {
    if (fatorBalanceamento(no) > 1)
    {
      if (fatorBalanceamento(no->getDir()) < 0)
      {
        // Dupla para a esquerda (RL)
        rotacaoSimplesR(no->getDir());
        rotacaoSimplesL(no);
      }
      else
      {
        // Simples para esquerda (RR)
        rotacaoSimplesL(no);
      }
    }
    else if (fatorBalanceamento(no) < -1)
    {
      if (fatorBalanceamento(no->getEsq()) > 0)
      {
        // Dupla para a Direita (LR)
        rotacaoSimplesL(no->getEsq());
        rotacaoSimplesR(no);
      }
      else
      {
        // Simples para a Direita (LL)
        rotacaoSimplesR(no);
      }
    }
  }

  void auxRotacoesRemover(No *no)
  {
    if (fatorBalanceamento(no) > 1)
    {
      if (fatorBalanceamento(no->getEsq()) > 0)
      {
        // Dupla para a Direita (LR)
        rotacaoSimplesL(no->getEsq());
        rotacaoSimplesR(no);
      }
      else
      {
        // Simples para a Direita (LL)
        rotacaoSimplesR(no);
      }
    }
    else if (fatorBalanceamento(no) < -1)
    {

      if (fatorBalanceamento(no->getDir()) < 0)
      {
        // Dupla para a esquerda (RL)
        rotacaoSimplesR(no->getDir());
        rotacaoSimplesL(no);
      }
      else
      {
        // Simples para esquerda (RR)
        rotacaoSimplesL(no);
      }
    }
  }

  int inserirAux(No *no, int chave)
  {
    int res;

    if (no == 0)
    {
      no = new No(chave);
      return 1;
    }

    if (chave < no->getChave())
    {
      if ((res = inserirAux(no->getEsq(), chave)) == 1)
      {
        auxRotacoesInserir(no);
      }
    }
    else if (chave > no->getChave())
    {
      if ((res = inserirAux(no->getDir(), chave)) == 1)
      {
        auxRotacoesInserir(no);
      }
    }
    else
    {
      cout << "Numero ja existe...";
      return 0;
    }

    return res;
  }

  No *procuraMenor(No *atual) const
  {
    No *no1 = atual, *no2 = atual->getEsq();
    while (no2 != 0)
    {
      no1 = no2;
      no2 = no2->getEsq();
    }
    return no1;
  }

  int removeAux(No *no, int chave)
  {
    if (no == 0)
    {
      cout << "Arvore vazia..." << endl;
      return 0;
    }

    int res;

    if (chave < no->getChave())
    {
      if ((res = removeAux(no->getEsq(), chave)) == 1)
        auxRotacoesRemover(no);
    }

    if (no->getChave() < chave)
    {
      if ((res = removeAux(no->getDir(), chave)) == 1)
        auxRotacoesRemover(no);
    }

    if (no->getChave() == chave)
    {
      if (no->getEsq() == 0 || no->getDir() == 0)
      {
        // Nó tem 1 filho ou nenhum
        No *oldnode = no;
        if (no->getEsq() != 0)
          no = no->getEsq();
        else
          no = no->getDir();

        free(oldnode);
      }
      else
      { // nó tem 2 filhos

        No *tmp = procuraMenor(no->getDir());
        no->setChave(tmp->getChave());
        removeAux(no->getDir(), no->getChave());
        auxRotacoesRemover(no);
      }

      return 1;
    }

    return res;
  }

public:
  ArvoreAVL()
  {
    raiz = 0;
  }

  void inserir(int chave)
  {
    inserirAux(raiz, chave);
  }

  bool remover(int chave)
  {
    if (removeAux(raiz, chave))
      return true;
    else
      return false;
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
};

#endif
