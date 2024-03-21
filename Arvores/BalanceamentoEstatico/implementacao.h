#ifndef IMPLEMENTACAO_H_INCLUDED
#define IMPLEMENTACAO_H_INCLUDED

#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

class DLLNode { // Classe nó da lista
public:
  DLLNode() { prox = ant = 0; }

  DLLNode(int el, DLLNode *prox = 0, DLLNode *ant = 0) {
    info = el;
    this->prox = prox;
    this->ant = ant;
  }

  int info;

  DLLNode *prox, *ant;
};

class IntDLList { // Classe lista
public:
  IntDLList() {
    topo = resto = 0;
    length = 0;
  }

  int isEmpty() { return topo == 0; }

  void addToTopo(int el) {
    if (topo == 0) {
      topo = new DLLNode(el);
      resto = topo;

    } else {
      topo->ant = new DLLNode(el, topo, 0);
      topo = topo->ant;
    }
    length++;
  }

  void addToResto(int el) {
    if (resto != 0) {
      resto->prox = new DLLNode(el, 0, resto);
      resto = resto->prox;
    } else {
      topo = resto = new DLLNode(el);
    }
    length++;
  }

  void deleteFromTopo() {
    if (resto == 0 && topo == 0) {
      cout << "Lista vazia";
      return;
    }
    DLLNode *tmp = topo;
    if (topo == resto) {
      topo = resto = 0;
    } else {
      topo = topo->prox;
      topo->ant = 0;
    }
    delete tmp;
    length--;
  }

  void deleteFromResto() {
    if (resto == 0 && topo == 0) {
      cout << "Lista vazia";
      return;
    }
    if (topo == resto) {
      delete topo;
      topo = resto = 0;
    } else {
      DLLNode *tmp;
      for (tmp = topo; tmp->prox != resto; tmp = tmp->prox) {
        delete resto;
      }
      resto = tmp;
      resto->prox = 0;
    }
    length--;
  }

  void printALL() const {
    for (DLLNode *tmp = topo; tmp != 0; tmp = tmp->prox) {
      cout << tmp->info << " |";
    }
    cout << endl;
  }

  void printALLDecrescente() const {
    for (DLLNode *tmp = resto; tmp != 0; tmp = tmp->ant) {
      cout << tmp->info << " |";
    }
    cout << endl;
  }

  void addOrdenado(int el) {
    if (topo == resto || el <= topo->info) {
      if (isEmpty()) {
        addToTopo(el);
      } else if (el <= topo->info) {
        addToTopo(el);
      } else {
        addToResto(el);
      }
      return;
    }

    for (DLLNode *tmp = topo->prox; tmp != 0; tmp = tmp->prox) {
      if (el <= tmp->info) {
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

  void removeSelecionado(int el) {
    if (topo == resto || el == topo->info) {
      if (isEmpty()) {
        cout << "Lista vazia..." << endl;
      } else if (el == topo->info) {
        deleteFromTopo();
      } else {
        cout << "Elemento nao encontrado..." << endl;
      }
      return;
    }

    for (DLLNode *tmp = topo->prox; tmp->prox != 0; tmp = tmp->prox) {
      if (el == tmp->info) {
        tmp->ant->prox = tmp->prox;
        tmp->prox->ant = tmp->ant;
        delete tmp;
        length--;
        return;
      }
    }
    if (el == resto->info) {
      deleteFromResto();
    } else {
      cout << "Elemento nao encontrado..." << endl;
    }
  }

  bool isIn(int el) const {
    for (DLLNode *tmp = topo; tmp != 0; tmp = tmp->prox) {
      if (el == tmp->info)
        return true;
    }
    return false;
  }

  int getElemento(int indice) const {
    int cont = 0;
    for (DLLNode *tmp = topo; tmp != 0; tmp = tmp->prox) {
      if (cont == indice) {
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

class No {

private:
  No *esq, *dir;
  int chave;

public:
  No(int chave, int altura = 0) {
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

class Arvore {
private:
  No *raiz;
  int length;

  void inserirAux(No *no, int chave) {
    if (chave < no->getChave()) {
      if (no->getEsq() == 0) {
        No *novo_no = new No(chave);
        no->setEsq(novo_no);
      } else {
        inserirAux(no->getEsq(), chave);
      }
    } else if (chave > no->getChave()) {
      if (no->getDir() == 0) {
        No *novo_no = new No(chave);
        no->setDir(novo_no);
      } else {
        inserirAux(no->getDir(), chave);
      }
    } else if (chave == no->getChave()) {
      cout << "Numero ja existe...";
    }
  }

  // funcao de remocao de no, recebe o no que queres remover
  No *removerNo(No *noatual) {
    No *no1, *no2;
    if (noatual->getEsq() == 0) {
      no2 = noatual->getDir();
      free(noatual);
      return no2;
    }
    no1 = noatual;
    no2 = noatual->getEsq();
    while (no2->getDir() != 0) {
      no1 = no2;
      no2 = no2->getDir();
    }
    if (no1 != noatual) {
      no1->setDir(no2->getEsq());
      no2->setEsq(noatual->getEsq());
    }
    no2->setDir(noatual->getDir());
    free(noatual);
    return no2;
  }

  // acessa arvore no modo pos ordem, adicionando em uma lista os elementos de
  // forma ordenada e depois destruindo o respectivo nó
  void destruirArvore(No *no, IntDLList *lista) {
    if (no != 0) {
      destruirArvore(no->getEsq(), lista);
      destruirArvore(no->getDir(), lista);
      lista->addOrdenado(no->getChave());
      removeArvore(no->getChave());
    }
  }

  // funcao que inicializa destruicao de arvore; retorna elementos ordenados em
  // uma lista
  IntDLList *initDestruirArvore(No *no) {
    IntDLList *lista = new IntDLList();
    destruirArvore(no, lista);
    return lista;
  }

  // funcao auxiliar; vai "fatiando" a lista a partir das medianas encontradas
  void balancearAux(IntDLList *lista) {
    int meio, length = lista->getLength();

    if (length == 0) {
      return;
    }

    if ((length % 2) == 0) {
      meio = length / 2 - 1;
    } else {
      meio = (length + 1) / 2 - 1;
    }

    int elemento = lista->getElemento(meio);

    lista->removeSelecionado(elemento);

    IntDLList *listaL = new IntDLList();
    IntDLList *listaR = new IntDLList();

    for (int i = 0; i < lista->getLength(); i++) {
      if (lista->getElemento(i) < elemento) {
        listaL->addOrdenado(lista->getElemento(i));
      } else {
        listaR->addOrdenado(lista->getElemento(i));
      }
    }

    inserir(elemento);
    balancearAux(listaL);
    balancearAux(listaR);
  }

public:
  Arvore() {
    raiz = 0;
    length = 0;
  }

  void inserir(int chave) {
    if (raiz == 0) {
      raiz = new No(chave);
    } else {
      inserirAux(raiz, chave);
    }
    length++;
  }

  No *getRaiz() { return raiz; }

  void emOrdem(No *no) {
    if (no != 0) {
      emOrdem(no->getEsq());
      cout << no->getChave() << " ";
      emOrdem(no->getDir());
    }
  }

  void preOrdem(No *no) {
    if (no != 0) {
      cout << no->getChave() << " ";
      preOrdem(no->getEsq());
      preOrdem(no->getDir());
    }
  }

  void posOrdem(No *no) {
    if (no != 0) {
      posOrdem(no->getEsq());
      posOrdem(no->getDir());
      cout << no->getChave() << " ";
    }
  }

  int retornaAltura(No *no) {
    if (no == 0) {
      return 0;
    }
    int altEsq = retornaAltura(no->getEsq());
    int altDir = retornaAltura(no->getDir());
    if (altEsq > altDir) {
      return altEsq + 1;
    } else {
      return altDir + 1;
    }
  }

  int removeArvore(int chave) {
    if (raiz == 0) {
      cout << "Arvore vazia..." << endl;
    }
    No *ant = 0;
    No *noatual = raiz;
    while (noatual != 0) {
      if (chave == noatual->getChave()) {
        if (noatual == raiz) {
          raiz = removerNo(noatual);
        } else {
          if (ant->getDir() == noatual) {
            ant->setDir(removerNo(noatual));
          } else {
            ant->setEsq(removerNo(noatual));
          }
        }
        length--;
        return 1;
      }
      ant = noatual;
      if (chave > noatual->getChave()) {
        noatual = noatual->getDir();
      } else {
        noatual = noatual->getEsq();
      }
    }
    return 0;
  }

  void balancearArvore() {
    if (length < 5) {
      cout << "Arvore possui quantidade muito pequena de elementos..." << endl;
    } else {
      IntDLList *lista = initDestruirArvore(raiz);
      balancearAux(lista);
      cout << "Arvore balanceada com sucesso ..." << endl;
    }
  }
};

#endif
