#include "implementacao.h"

// Lucas Batista dos Santos, Segundo Ano CC
// Estruturas de Dados-P2, professora Fabiane Sorbar

#define LENGTH 17

int main()
{
  ArvoreRB arvore;

  int op, chave;

  int insercoes[LENGTH] = {7, 5, 3, 25, 10, 50, 30, 8, 12, 9, 15, 88, 28, 27, 51, 90, 87};

  for (int i = 0; i < LENGTH; i++)
    arvore.inserir(insercoes[i]);

  // cout << "==========================" << endl;
  // arvore.preOrdem(arvore.getRaiz());
  // cout << "==========================" << endl;

  // for (int i = 0; i < LENGTH; i++)
  //   if (arvore.remover(insercoes[i]))
  //   {
  //     cout << insercoes[i] << " removido..." << endl;
  //     arvore.preOrdem(arvore.getRaiz());
  //   }

  do
  {
    cout << "==========================" << endl;
    cout << "Escolha a acao:" << endl
         << "1:inserir" << endl
         << "2:imprimir pre ordem" << endl
         << "3:imprimir em ordem" << endl
         << "4:imprimir pos ordem" << endl
         << "5:remover" << endl
         << "6:consultar" << endl
         << "0:sair" << endl
         << "Opcao: ";

    cin >> op;
    cout << "==========================" << endl;

    switch (op)
    {
    case 1:
      cout << "Digite o numero da chave: ";
      cin >> chave;

      if (arvore.inserir(chave))
        cout << "Elemento inserido com sucesso: " << chave;
      else
        cout << "Elemento ja existe na arvore: " << chave;

      cout << endl;
      break;
    case 2:
      if (arvore.getRaiz() == 0)
        cout << "Arvore vazia..." << endl;
      else
      {
        arvore.preOrdem(arvore.getRaiz());
        cout << endl;
      }
      break;
    case 3:
      if (arvore.getRaiz() == 0)
        cout << "Arvore vazia..." << endl;
      else
      {
        arvore.emOrdem(arvore.getRaiz());
        cout << endl;
      }
      break;

    case 4:
      if (arvore.getRaiz() == 0)
        cout << "Arvore vazia..." << endl;
      else
      {
        arvore.posOrdem(arvore.getRaiz());
        cout << endl;
      }
      break;

    case 5:
      if (arvore.getRaiz() == 0)
        cout << "Arvore vazia..." << endl;
      else
      {
        cout << "Digite o numero da chave: ";
        cin >> chave;
        if (arvore.remover(chave))
          cout << "Chave removida com sucesso: " << chave << endl;
        else
          cout << "Chave nao encontrada: " << chave << endl;
      }
      break;

    case 6:
      if (arvore.getRaiz() == 0)
        cout << "Arvore vazia..." << endl;
      else
      {
        cout << "Digite o numero da chave: ";
        cin >> chave;
        if (arvore.consulta(chave))
          cout << "Elemento existe na arvore: " << chave << endl;
        else
          cout << "Elemento nao encontrado: " << chave << endl;
      }
      break;

    case 0:
      return 0;

    default:
      cout << "Opcao invalida..." << endl;
    }
  } while (1);
}
