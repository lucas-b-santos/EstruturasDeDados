#include "implementacao.h"

// Felipe Sella Lopes, Lucas Batista dos Santos, Segundo Ano CC
// Estruturas de Dados-P2, professora Fabiane Sorbar

int main() {
  Arvore arvore;
  int op, chave;

  do {
    cout << "Escolha a acao:" << endl
         << "1:reservar poltrona" << endl
         << "2:imprimir pre ordem" << endl
         << "3:imprimir em ordem" << endl
         << "4:imprimir pos ordem" << endl
         << "5:remover reserva" << endl
         << "6:balancear" << endl
         << "7:mostrar altura" << endl
         << "0:sair" << endl;
    cout << "Opcao: ";
    cin >> op;

    switch (op) {
    case 1:
      cout << "Digite o numero da poltrona: ";
      cin >> chave;
      if (chave < 1) {
        cout << "Numero invalido de poltrona..." << endl;
        break;
      }
      arvore.inserir(chave);
      cout << endl;
      break;
    case 2:
      if (arvore.getRaiz() == 0) {
        cout << "Nao ha reservas..." << endl;
        cout << endl;
      } else {
        arvore.preOrdem(arvore.getRaiz());
        cout << endl;
      }
      break;
    case 3:
      if (arvore.getRaiz() == 0) {
        cout << "Nao ha reservas..." << endl;
        cout << endl;
      } else {
        arvore.emOrdem(arvore.getRaiz());
        cout << endl;
      }
      break;
    case 4:
      if (arvore.getRaiz() == 0) {
        cout << "Nao ha reservas..." << endl;
        cout << endl;
      } else {
        arvore.posOrdem(arvore.getRaiz());
        cout << endl;
      }
      break;
    case 5:
      if (arvore.getRaiz() == 0) {
        cout << "Nao ha reservas..." << endl;
        cout << endl;
      } else {
        cout << "Digite o numero da poltrona: ";
        cin >> chave;
        if (arvore.removeArvore(chave)) {
          cout << "Reserva removida com sucesso..." << endl;
          cout << endl;
        } else {
          cout << "Poltrona nao encontrada..." << endl;
          cout << endl;
        }
      }
      break;
    case 6:
      if (arvore.getRaiz() == 0) {
        cout << "Nao ha reservas..." << endl;
        cout << endl;
      } else {
        arvore.balancearArvore();
        cout << endl;
      }
      break;
    case 7:
      if (arvore.getRaiz() == 0) {
        cout << "Nao ha reservas..." << endl;
        cout << endl;
      } else {
        cout << "Altura da arvore: "
             << arvore.retornaAltura(arvore.getRaiz()) - 1 << endl;
        cout << endl;
      }
      break;

    case 0:
      return 0;
    default:
      cout << "Opcao invalida..." << endl;
    }
  } while (1);
}
