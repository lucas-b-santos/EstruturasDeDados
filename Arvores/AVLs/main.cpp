#include "implementacao.h"

// Lucas Batista dos Santos, Segundo Ano CC
// Estruturas de Dados-P2, professora Fabiane Sorbar

int main()
{
  ArvoreAVL arvore;
  int op, chave;

  int insercoes[] = {7, 5, 3, 25, 10, 50, 30, 8, 20, 1, 64, 12, 18, 66, 38, 95, 58, 59, 70, 68, 39, 62, 60, 43, 16, 67, 34, 35};

  for (int i = 0; i < 28; i++)
    arvore.inserir(insercoes[i]);

  cout << "==========================" << endl;

  for (int i = 0; i < 28; i++)
    arvore.remover(insercoes[i]);

  do
  {
    cout << "Escolha a acao:" << endl
         << "1:inserir" << endl
         << "2:imprimir pre ordem" << endl
         << "3:imprimir em ordem" << endl
         << "4:imprimir pos ordem" << endl
         << "5:remover" << endl
         << "0:sair" << endl;
    cout << "Opcao: ";
    cin >> op;

    switch (op)
    {
    case 1:
      cout << "Digite o numero da chave: ";
      cin >> chave;
      arvore.inserir(chave);
      cout << endl;
      break;
    case 2:
      if (arvore.getRaiz() == 0)
      {
        cout << "Arvore vazia..." << endl;
        cout << endl;
      }
      else
      {
        arvore.preOrdem(arvore.getRaiz());
        cout << endl;
      }
      break;
    case 3:
      if (arvore.getRaiz() == 0)
      {
        cout << "Arvore vazia..." << endl;
        cout << endl;
      }
      else
      {
        arvore.emOrdem(arvore.getRaiz());
        cout << endl;
      }
      break;
    case 4:
      if (arvore.getRaiz() == 0)
      {
        cout << "Arvore vazia..." << endl;
        cout << endl;
      }
      else
      {
        arvore.posOrdem(arvore.getRaiz());
        cout << endl;
      }
      break;

    case 5:
      if (arvore.getRaiz() == 0)
      {
        cout << "Arvore vazia..." << endl;
        cout << endl;
      }
      else
      {
        cout << "Digite o numero da chave: ";
        cin >> chave;
        if (arvore.remover(chave))
        {
          cout << "Removido com sucesso..." << endl;
          cout << endl;
        }
        else
        {
          cout << "Chave nao encontrada..." << endl;
          cout << endl;
        }
      }
      break;

    case 0:
      return 0;

    default:
      cout << "Opcao invalida..." << endl;
    }
  } while (1);
}
