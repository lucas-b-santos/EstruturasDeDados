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

    return no->getColor();
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
    if (retornaCor(filhoR->getEsq()) == RED)
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
    if (retornaCor((*no)->getEsq()->getEsq()) == RED)
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
    if (retornaCor(filhoR) == RED)
    {
      *no = rotacaoSimplesL(no);
      filhoR = (*no)->getDir();
      filhoL = (*no)->getEsq();
      cout << "direito de " << (*no)->getChave() << " eh vermelho, rotaciona " << (*no)->getChave() << " para esquerda" << endl;
    }

    // Filho da direita e neto da esquerda sao vermelhos
    if (filhoL != 0 && retornaCor(filhoR) == RED && retornaCor(filhoL->getEsq()) == RED)
    {
      *no = rotacaoSimplesR(no);
      filhoR = (*no)->getDir();
      filhoL = (*no)->getEsq();
      cout << "Filho da direita e neto da esquerda de " << (*no)->getChave() << " sao vermelhos, rotaciona " << (*no)->getChave() << " para esquerda" << endl;
    }

    // 2 filhos vermelhos: troca cor!
    if (retornaCor(filhoL) == RED && retornaCor(filhoR) == RED)
    {
      (*no)->trocaCor();
      cout << "Os dois filhos de " << (*no)->getChave() << " sao vermelhos, troca cor de " << (*no)->getChave() << endl;
    }

    return *no;
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

  No *removerMenor(No **no)
  {

    No *filhoL = (*no)->getEsq();
    if (filhoL == 0)
    {
      free(*no);
      return 0;
    }
    if (retornaCor(filhoL) == BLACK && retornaCor(filhoL->getEsq()) == BLACK)
    {
      *no = move2EsqRED(no);
      filhoL = (*no)->getEsq();
    }

    filhoL = removerMenor(&filhoL);
    (*no)->setEsq(filhoL);
    return balancear(no);
  }

  No *removeAux(No **no, int chave)
  {

    No *filhoL = (*no)->getEsq();
    No *filhoR = (*no)->getDir();

    cout << "acessando no " << (*no)->getChave() << endl;

    if (chave < (*no)->getChave())
    {

      if (retornaCor(filhoL) == BLACK && filhoL)
        if (retornaCor(filhoL->getEsq()) == BLACK)
        {
          cout << "esquerdo de " << (*no)->getChave() << " eh preto e esquerdo do esquerdo eh preto" << endl;
          cout << "move " << (*no)->getChave() << " para esquerda" << endl;
          *no = move2EsqRED(no);
          filhoL = (*no)->getEsq();
        }

      (*no)->setEsq(removeAux(&filhoL, chave));
    }
    else
    {
      cout << "teste 1" << endl;

      if (retornaCor((*no)->getEsq()) == RED)
      {
        cout << "esquerdo de " << (*no)->getChave() << " eh vermelho, rotacao para direita em " << (*no)->getChave() << endl;
        *no = rotacaoSimplesR(no);
        filhoL = (*no)->getEsq();
        filhoR = (*no)->getDir();

        preOrdem(raiz);
      }

      cout << "teste 2" << endl;

      if (chave == (*no)->getChave() && !(*no)->getDir())
      {
        cout << "no encontrado: " << (*no)->getChave() << ", nao possui filho a direita, libera no e retona 0 " << endl;
        free(*no);
        return 0;
      }

      cout << "teste 3" << endl;

      if (retornaCor(filhoR) == BLACK && filhoR)
        if (retornaCor(filhoR->getEsq()) == BLACK)
        {
          cout << "no " << (*no)->getChave() << " possui filho direito preto e filho esquerdo do direito preto, move "
               << (*no)->getChave() << " para direita" << endl;

          *no = move2DirRED(no);
          filhoL = (*no)->getEsq();
          filhoR = (*no)->getDir();

          preOrdem(raiz);
        }

      cout << "teste 4" << endl;

      if (chave == (*no)->getChave())
      {
        preOrdem(raiz);
        No *x = procuraMenor(filhoR);
        cout << "teste a, menor filho = " << x->getChave() << endl;

        (*no)->setChave(x->getChave());
        cout << "teste b" << endl;

        preOrdem(raiz);

        (*no)->setDir(removerMenor(&filhoR));

        preOrdem(raiz);
      }
      else
      {
        filhoR->setDir(removeAux(&filhoR, chave));
      }

      // preOrdem(raiz);

      cout << "teste 5" << endl;
    }

    cout << "balanceando no " << (*no)->getChave() << endl;

    // return balancear(no);
    return *no;
  }

  void consultaAux(No *no, int chave, int *res) const
  {
    if (no != 0)
    {
      if (no->getChave() == chave)
      {
        *res = 1;
        return;
      }
      consultaAux(no->getEsq(), chave, res);
      consultaAux(no->getDir(), chave, res);
    }
  }

public:
  ArvoreRB() { raiz = 0; }

  int consulta(int chave) const
  {
    int res = 0;
    consultaAux(raiz, chave, &res);
    return res;
  }

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
    if (consulta(chave))
    {
      raiz = removeAux(&raiz, chave);
      if (raiz)
        raiz->setColor(BLACK);
      return true;
    }
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
