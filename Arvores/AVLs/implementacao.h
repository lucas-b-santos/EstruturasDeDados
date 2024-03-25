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

  int fatorBalanceamento(No *raiz) const
  {
    return retornaAltura(raiz->getDir()) - retornaAltura(raiz->getEsq());
  }

  No *rotacaoSimplesL(No **no)
  {
    No *filhoR = (*no)->getDir();
    (*no)->setDir(filhoR->getEsq());
    filhoR->setEsq(*no);
    *no = filhoR;
    return filhoR;
  }

  No *rotacaoSimplesR(No **no)
  {
    No *filhoL = (*no)->getEsq();
    (*no)->setEsq(filhoL->getDir());
    filhoL->setDir(*no);
    *no = filhoL;
    return filhoL;
  }

  void auxRotacoes(No **no, No **pai)
  {
    if (fatorBalanceamento(*no) > 1)
    {
      No *filhoR = (*no)->getDir();

      if (fatorBalanceamento(filhoR) < 0)
      {
        cout << "Realiza dupla para a esquerda em " << (*no)->getChave() << endl;

        // Dupla para a esquerda (RL)
        (*no)->setDir(rotacaoSimplesR(&filhoR));
        if (pai == 0)
          rotacaoSimplesL(no);
        else if ((*pai)->getDir() == *no)
          (*pai)->setDir(rotacaoSimplesL(no));
        else
          (*pai)->setEsq(rotacaoSimplesL(no));
      }

      else
      {
        cout << "Realiza simples para a esquerda em " << (*no)->getChave() << endl;

        // Simples para esquerda (RR)
        if (pai == 0)
          rotacaoSimplesL(no);
        else if ((*pai)->getDir() == *no)
          (*pai)->setDir(rotacaoSimplesL(no));
        else
          (*pai)->setEsq(rotacaoSimplesL(no));
      }
    }

    else if (fatorBalanceamento(*no) < -1)

    {
      No *filhoL = (*no)->getEsq();

      if (fatorBalanceamento(filhoL) > 0)
      {
        cout << "Realiza dupla para a direita em " << (*no)->getChave() << endl;

        // Dupla para a Direita (LR)
        (*no)->setEsq(rotacaoSimplesL(&filhoL));
        if (pai == 0)
          rotacaoSimplesR(no);
        else if ((*pai)->getDir() == *no)
          (*pai)->setDir(rotacaoSimplesR(no));
        else
          (*pai)->setEsq(rotacaoSimplesR(no));
      }
      else
      {
        cout << "Realiza simples para a direita em " << (*no)->getChave() << endl;

        // Simples para a Direita (LL)
        if (pai == 0)
          rotacaoSimplesR(no);
        else if ((*pai)->getDir() == *no)
          (*pai)->setDir(rotacaoSimplesR(no));
        else
          (*pai)->setEsq(rotacaoSimplesR(no));
      }
    }
  }

  int inserirAux(No **no, No **pai, int chave, bool flag = true)
  {
    int res;

    if (*no == 0)
    {
      *no = new No(chave);

      if (pai != 0)
      {
        if (flag)
        {
          (*pai)->setDir(*no);
          cout << "Inserindo " << (*pai)->getDir()->getChave() << " como filho a direita de " << (*pai)->getChave() << endl;
        }

        else
        {
          (*pai)->setEsq(*no);
          cout << "Inserindo " << (*pai)->getEsq()->getChave() << " como filho a esquerda de " << (*pai)->getChave() << endl;
        }
      }

      return 1;
    }

    if (chave < (*no)->getChave())
    {
      No *filhoL = (*no)->getEsq();

      if ((res = inserirAux(&filhoL, no, chave, false)) == 1)
        auxRotacoes(no, pai);
    }

    else if (chave > (*no)->getChave())
    {
      No *filhoR = (*no)->getDir();

      if ((res = inserirAux(&filhoR, no, chave)) == 1)
        auxRotacoes(no, pai);
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

  int removeAux(No **no, No **pai, int chave)
  {
    if (*no == 0)
      return 0;

    int res;

    No *filhoL = (*no)->getEsq();
    No *filhoR = (*no)->getDir();

    if (chave < (*no)->getChave())
    {
      if ((res = removeAux(&filhoL, no, chave)) == 1)
        auxRotacoes(no, pai);
    }

    if ((*no)->getChave() < chave)
    {
      if ((res = removeAux(&filhoR, no, chave)) == 1)
        auxRotacoes(no, pai);
    }

    if ((*no)->getChave() == chave)
    {
      bool flag, flag2 = false;

      if (pai) {
        flag2 = true;

        if ((*pai)->getDir() == (*no))
          flag = true;
        else
          flag = false;
      }

      if ((*no)->getEsq() == 0 || (*no)->getDir() == 0)
      { // no tem um 1 filho ou nenhum

        No *oldnode = *no;

        if ((*no)->getEsq() != 0)
          (*no) = (*no)->getEsq();
        else
          (*no) = (*no)->getDir();

        free(oldnode);

        if (!flag2)
          return 1;

        if (flag)
          (*pai)->setDir(*no);
        else
          (*pai)->setEsq(*no);
      }
      else
      { // no tem 2 filhos
        No *tmp = procuraMenor((*no)->getDir());
        (*no)->setChave(tmp->getChave());
        removeAux(&filhoR, no, (*no)->getChave());
        auxRotacoes(no, pai);
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
    inserirAux(&raiz, 0, chave);
  }

  bool remover(int chave)
  {
    if (removeAux(&raiz, 0, chave))
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
};

#endif
