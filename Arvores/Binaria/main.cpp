#include "implementacaoArvores.h"

int main()
{
    Arvore arvore;
    int valor, opc;

    do
    {
        cout << "[1] Inserir valor" << endl;
        cout << "[2] Em ordem" << endl;
        cout << "[3] Pre ordem" << endl;
        cout << "[4] Pos ordem" << endl;
        cout << "[5] Remover valor" << endl;
        cout << "[0] Sair" << endl;
        cin >> opc;

        switch (opc)
        {
        case 1:
            cout << "Valor: ";
            cin >> valor;
            arvore.inserir(valor);

            break;

        case 2:
            arvore.emOrdem(arvore.getRaiz());
            cout << endl;
            break;

        case 3:
            arvore.preOrdem(arvore.getRaiz());
            cout << endl;
            break;

        case 4:
            arvore.posOrdem(arvore.getRaiz());
            cout << endl;
            break;

        case 5:
            if (arvore.getRaiz() == 0)
            {
                cout << "Arvore vazia..." << endl;
                break;
            }

            cout << "Valor: ";
            cin >> valor;

            if (arvore.remover(valor))
            {
                cout << "Valor removido com sucesso..." << endl;
            }
            else
            {
                cout << "Valor nao encontrado..." << endl;
            }

            break;

        case 0:
            return 0;

        default:
            cout << "Opcao invalida!" << endl;
            break;
        }
    } while (1);
}
