#ifndef IMPLEMENTACAO_H_INCLUDED
#define IMPLEMENTACAO_H_INCLUDED

#include <iostream>
#include <stdlib.h>

using namespace std;

#define RED 1
#define BLACK 0

class No
{

private:
  No *esq, *dir;
  int chave, cor;

public:
  No(int chave)
  {
    this->chave = chave;
    esq = 0;
    dir = 0;
    cor = RED;
  }

  int getChave() const { return chave; }
  No *getEsq() const { return esq; }
  No *getDir() const { return dir; }
  int getColor() const { return cor; }

  void setEsq(No *no) { esq = no; }
  void setDir(No *no) { dir = no; }
  void setChave(int chave) { this->chave = chave; }
  void setColor(int cor) { this->cor = cor; }

  void trocaCor()
  {
    cor = !cor;
    if (esq)
    {
      esq->setColor(!esq->getColor());
    }
    if (dir)
    {
      dir->setColor(!dir->getColor());
    }
  }
};

class ArvoreRB
{
private:
  No *raiz;

  int retornaCor(No *no)
  {
    if (no == 0)
    {
      return BLACK;
    }

    return (no)->getColor();
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

  int fatorBalanceamento(No *raiz) const
  {
    return retornaAltura(raiz->getDir()) - retornaAltura(raiz->getEsq());
  }

  No *rotacaoSimplesL(No **no)
  {
    No *filhoR = (*no)->getDir();
    int color = (*no)->getColor();

    (*no)->setDir(filhoR->getEsq());
    filhoR->setEsq(*no);

    filhoR->setColor(color);
    (*no)->setColor(RED);
    return filhoR;
  }

  No *rotacaoSimplesR(No **no)
  {
    No *filhoL = (*no)->getEsq();
    int color = (*no)->getColor();

    (*no)->setEsq(filhoL->getDir());
    filhoL->setDir(*no);

    filhoL->setColor(color);
    (*no)->setColor(RED);
    return filhoL;
  }

  No *move2EsqRED(No **no)
  {
    No *filhoR = (*no)->getDir();

    (*no)->trocaCor();
    if (filhoR->getEsq()->getColor())
    {
      (*no)->setDir(rotacaoSimplesR(&filhoR));
      *no = rotacaoSimplesL(no);
      (*no)->trocaCor();
    }

    return *no;
  }

  No *move2DirRED(No **no)
  {
    (*no)->trocaCor();
    if ((*no)->getEsq()->getEsq()->getColor())
    {
      *no = rotacaoSimplesR(no);
      (*no)->trocaCor();
    }
    return *no;
  }

  No *balancear(No **no)
  {
    No *filhoR = (*no)->getDir();
    No *filhoL = (*no)->getEsq();

    // no vermelho eh sempre filho a esquerda
    if (filhoR->getColor())
      *no = rotacaoSimplesL(no);

    // Filho da direita e neto da esquerda sao vermelhos
    if (filhoL && filhoR->getColor() && filhoL->getEsq()->getColor())
      *no = rotacaoSimplesR(no);

    // 2 filhos vermelhos: troca cor!
    if (filhoL->getColor() && filhoR->getColor())
      (*no)->trocaCor();

    return *no;
  }

  void auxRotacoes(No **no, No **pai)
  {
    if (fatorBalanceamento(*no) > 1)
    {
      No *filhoR = (*no)->getDir();

      if (fatorBalanceamento(filhoR) < 0)
      {
        cout << "Realiza dupla para a esquerda em " << (*no)->getChave()
             << endl;

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
        cout << "Realiza simples para a esquerda em " << (*no)->getChave()
             << endl;

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
        cout << "Realiza simples para a direita em " << (*no)->getChave()
             << endl;

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

  No *inserirAux(No **no, int chave, int *resp)
  {
    if (*no == 0)
    {
      No *novo = new No(chave);
      *resp = 1;
      return novo;
    }

    No *filhoL = (*no)->getEsq();
    No *filhoR = (*no)->getDir();

    if (chave == (*no)->getChave())
      *resp = 0;
    else
    {
      if (chave < (*no)->getChave())
        (*no)->setEsq(inserirAux(&filhoL, chave, resp));
      else
        (*no)->setDir(inserirAux(&filhoR, chave, resp));
    }

    filhoL = (*no)->getEsq();
    filhoR = (*no)->getDir();

    // se filho direito eh vermelho e filho esquerdo eh preto
    if (retornaCor(filhoR) == RED && retornaCor(filhoL) == BLACK)
    {
      cout << "Realiza rotacao para esquerda em " << (*no)->getChave() << endl;
      *no = rotacaoSimplesL(no);
      filhoL = (*no)->getEsq();
      filhoR = (*no)->getDir();
    }

    // se filho esquerdo eh vermelho e filho esquerdo do esquerdo eh vermelho
    if (retornaCor(filhoL) == RED && retornaCor(filhoL->getEsq()) == RED)
    {
      cout << "Realiza rotacao para direita em " << (*no)->getChave() << endl;
      *no = rotacaoSimplesR(no);
      filhoL = (*no)->getEsq();
      filhoR = (*no)->getDir();
    }

    // se ambos filhos sao vermelhos
    if (retornaCor(filhoL) == RED && retornaCor(filhoR) == RED)
    {
      cout << "Troca cor de " << (*no)->getChave() << endl;
      (*no)->trocaCor();
    }

    return *no;
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

      if (pai)
      {
        flag2 = true;

        if ((*pai)->getDir() == (*no))
          flag = true;
        else
          flag = false;
      }

      if ((*no)->getEsq() == 0 ||
          (*no)->getDir() == 0)
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
  ArvoreRB() { raiz = 0; }

  int inserir(int chave)

  {
    int resp;

    raiz = inserirAux(&raiz, chave, &resp);

    if (raiz)
      raiz->setColor(BLACK);

    return resp;
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

      if (no->getColor())
        cout << "VERMELHO";
      else
        cout << "PRETO";
      cout << endl;

      emOrdem(no->getDir());
    }
  }

  void preOrdem(No *no) const
  {
    if (no != 0)
    {
      cout << no->getChave() << " ";

      if (no->getColor())
        cout << "VERMELHO";
      else
        cout << "PRETO";
      cout << endl;
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

      if (no->getColor())
        cout << "VERMELHO";
      else
        cout << "PRETO";
      cout << endl;
    }
  }
};

#endif
