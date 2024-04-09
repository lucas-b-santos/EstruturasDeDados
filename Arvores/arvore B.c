#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

// Ordem da arvore
#define ORDEM 2

// arquivo que armazena os registros
#define namefile "_arquivo.dat"

int cont = -1, count = 0;

// Registro a ser armazenado
typedef struct
{
  int chave;
  char nome[20];
  int idade;
  int ApPage[2];
  int rank;
} Registro;

// Definicao do ponteiro que apontara para as paginas
typedef struct Pagina_str *Apontador;

// Estrutura da pagina
typedef struct Pagina_str
{
  int n;       // quantidade de registros armazenados na pagina
  int pageNum; // indice da pagina dentro do arquivo
  int num;
  Registro r[2 * ORDEM];      // registros armazenados na pagina
  Apontador p[2 * ORDEM + 1]; // ponteiros que apontam para as paginas filhas
} Pagina;

// No da lista simplesmente encadeada
struct Node
{
  Registro info;
  struct Node *prox;
};
typedef struct Node node;

// ###########################Assinaturas das funcoes que serao implementadas###################################

// encontra o antecessor de um elemento (maior elemento menor que o elemento em questao)
void Antecessor(Apontador Ap, int Ind, Apontador ApPai, int *Diminuiu);

// Realiza busca de um registro no arquivo
void Busca(Registro Reg, Apontador Ap);

// exibe paginas em ordem crescente
void em_ordem(Apontador raiz);

// acessa uma pagina dentro do arquivo e busca um registro dentro dela
void buscainFile(Registro Reg, Apontador pagina);

// verifica se a pagina e folha
int isleaf(Apontador a);

// verifica se o arquivo ja existe
int file_exists(const char *filename);

// Percorre arvore ate encontrar pagina ideal (caso exista) para inserir, ou cria nova caso necessario
void Insere(Registro Reg, Apontador *Ap);

// Insere registro na posicao correta dentro da pagina
void InsereNaPagina(Apontador Ap, Registro Reg, Apontador ApDir);

// Percorre arvore ate encontrar pagina ideal (caso exista) para inserir
void Ins(Registro Reg, Apontador Ap, int *Cresceu, Registro *RegRetorno, Apontador *ApRetorno);
int Imprime2(Apontador p, int Nivel, int aux, int n);
int Imprime(Apontador p, int N, int aux, int n);

// Exatamente igual a funcao insere, porem nao salva pagina no arquivo, apenas adiciona registro na pagina ideal
void InsertPosFile(Registro Reg, Apontador *Ap);

//busca o ultimo no inserido na lista, para assim setar seu proximo para o novo no inserido
node *busca(Registro info, node *LISTA);

// Exatamente igual a funcao Ins
void InsPosFile(Registro Reg, Apontador Ap, int *Cresceu, Registro *RegRetorno, Apontador *ApRetorno);

void pos_ordem(Apontador raiz);

//inicializa retirada de um registro da arvore
void Retira(int Ch, Apontador *Ap);

//encontra registro dentro da arvore e executa devidas operacoes de retirada
void Ret(int Ch, Apontador *Ap, int *Diminuiu);

//executa operacoes na arvore apos remover, de maneira a manter as propriedades da arvore-B
void Reconstitui(Apontador ApPag, Apontador ApPai, int PosPai, int *Diminuiu);

// percorre uma subarvore, de maneira a salvar todas as paginas a partir dela dentro do arquivo
void saveAux(Apontador p, int Nivel);

// salva uma pagina efetivamente no arquivo
void salvar(Apontador pagina, Registro Reg[]);

// recupera todos os registros a partir do arquivo
void recuperarReg(Apontador *arv, node *LISTA);

// insere no inicio da lista simplesmente encadeada
void insereInicio(Registro info, node *LISTA);

// inicia processo de inserir no inicio da lista
void execut(Registro info, node *LISTA);

// exibe lista
void exibe(node *LISTA);

typedef Apontador TipoDicionario;
void Inicializa(TipoDicionario *Dicionario)
{
  *Dicionario = NULL;
} /* Inicializa a estrutura */

// inicializa a lista encadeada
void inicia(node *LISTA)
{
  LISTA->prox = NULL;
}

// ############################### Estrutura 1 Btree

void Antecessor(Apontador Ap, int Ind, Apontador ApPai, int *Diminuiu)
{
  // verifica se a pagina pai de Ap tem um ponteiro para a página a direita de Ap
  if (ApPai->p[ApPai->n] != NULL)
  {
    Antecessor(Ap, Ind, ApPai->p[ApPai->n], Diminuiu);
    if (*Diminuiu)
      Reconstitui(ApPai->p[ApPai->n], ApPai, ApPai->n, Diminuiu);
    return;
  }

  /* copia o último elemento da pagina pai (ApPai->r[ApPai->n - 1]) para a posicao do elemento que será removido (Ap->r[Ind - 1])
  garantindo que o antecessor seja corretamente copiado para a posicao onde estava o elemento removido. */
  Ap->r[Ind - 1] = ApPai->r[ApPai->n - 1];

  // decrementa o contador de elementos na pagina pai
  ApPai->n--;

  // atualiza flag "Diminuiu"
  *Diminuiu = ApPai->n < ORDEM;
} /* Antecessor */

void Busca(Registro Reg, Apontador Ap)
{
  int i;

  // caso pagina vazia
  if (Ap == NULL) //
  {
    printf("chave nao encontrada: %d\n", Reg.chave);
    return;
  }
  i = 1;

  // busca na pagina atual ate encontrar a chave ou a subarvore em que ela esta
  while (i < Ap->n && Reg.chave > Ap->r[i - 1].chave)
    i++;

  // caso encontrou, mostra chave e busca os dados do registro no arquivo
  if (Reg.chave == Ap->r[i - 1].chave)
  {
    printf("chave: %d \n", Reg.chave);
    buscainFile(Ap->r[i - 1], Ap);
    return;
  }

  // caso esteja na subarvore da esquerda, busca pela subarvore da esquerda
  if (Reg.chave < Ap->r[i - 1].chave)
    Busca(Reg, Ap->p[i - 1]);

  // caso contrario busca pela subarvore da direita
  else
    Busca(Reg, Ap->p[i]);
}

void buscainFile(Registro Reg, Apontador pagina)
{

  Registro reg[2 * ORDEM];
  int i;

  // abre arquivo
  FILE *arq = fopen(namefile, "rb");

  // caso falha na abertura, encerra programa
  if (arq == NULL)
    exit(1);

  // seta cursor para a pagina em questao
  fseek(arq, pagina->pageNum * (2 * ORDEM * sizeof(Registro)), SEEK_SET);

  // le informacoes para o registro
  fread(reg, (2 * ORDEM * sizeof(Registro)), 1, arq);

  // fecha arquivo
  fclose(arq);

  // acessa todos os registros da pagina, buscando o registro desejado
  for (i = 0; i < 2 * ORDEM; i++)
  {
    if (Reg.chave == reg[i].chave)
      printf("%s%d\n", reg[i].nome, reg[i].idade);
  }
}

void em_ordem(Apontador raiz)
{
  int i;
  if (raiz != NULL)
  {
    // exibe todas as chaves da respectiva pagina
    for (i = 0; i < raiz->n; i++)
    {

      em_ordem(raiz->p[i]);
      printf("%d ", raiz->r[i].chave);
      printf("\n");
    }

    em_ordem(raiz->p[i]);
  }
}

int file_exists(const char *filename)
{
  FILE *arquivo;

  if ((arquivo = fopen(filename, "rb")))
  {
    fclose(arquivo);
    return 1;
  }
  return 0;
}

int isleaf(Apontador a)
{
  if (a->p[0] == NULL)
    return 1;
  else
    return 0;
}

void InsereNaPagina(Apontador Ap, Registro Reg, Apontador ApDir)
{

  int k;
  int NaoAchouPosicao;

  // k = quantidade de elementos na pagina
  k = Ap->n;

  NaoAchouPosicao = k > 0;

  // percorre pagina a partir da maior chave
  while (NaoAchouPosicao)
  {

    // caso encontrou posicao, sai do while
    if (Reg.chave >= Ap->r[k - 1].chave)
    {
      NaoAchouPosicao = 0;
      break;
    }

    /*desloca chave e ponteiro para a direita, de maneira que se tenha
    espaco para inserir a chave na ordem correta*/
    Ap->r[k] = Ap->r[k - 1];
    Ap->p[k + 1] = Ap->p[k];

    // decrementa contador
    k--;

    // caso registro deva ser colocado na primeira posicao da pagina
    if (k < 1)
      NaoAchouPosicao = 0;
  }

  // Insere registro, seta o apontador direito e incrementa contador de elementos
  Ap->r[k] = Reg;
  Ap->p[k + 1] = ApDir;
  Ap->n++;
}

void Ins(Registro Reg, Apontador Ap, int *Cresceu, Registro *RegRetorno, Apontador *ApRetorno)
{
  Apontador ApTemp;
  Registro Aux;
  int i, j;

  /*caso apontador da pagina nulo, se assume que arvore vai crescer
  em tamanho*/
  if (Ap == NULL)
  {
    *Cresceu = 1;
    *RegRetorno = Reg;
    *ApRetorno = NULL;
    return;
  }

  i = 1;

  // busca a posicao de insercao do registro dentro da pagina
  while (i < Ap->n && Reg.chave > Ap->r[i - 1].chave)
    i++;

  // caso chave ja exista na pagina
  if (Reg.chave == Ap->r[i - 1].chave)
  {
    printf("chave ja existente: %d \n", Reg.chave);
    *Cresceu = 0;
    return;
  }

  // caso chave deva ser inserida na esquerda
  if (Reg.chave < Ap->r[i - 1].chave)
    Ins(Reg, Ap->p[i - 1], Cresceu, RegRetorno, ApRetorno);

  // caso contrario
  else
    Ins(Reg, Ap->p[i], Cresceu, RegRetorno, ApRetorno);

  /*Chave ja existe ou foi inserida em pagina existente, retorna
  e nao executa os outros passos*/
  if (!*Cresceu)
    return;

  if (Ap->n < 2 * ORDEM) /*Verificando se a pagina tem espaco */
  {

    // caso tenha espaco, usa funcao para inserir na pagina
    InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
    *Cresceu = 0;
    return;
  }

  /* Split: Pagina tem que ser dividida */

  // Cria nova pagina
  ApTemp = (Apontador)malloc(sizeof(Pagina));

  // Seta valores iniciais da nova pagina
  ApTemp->n = 0;
  ApTemp->p[0] = NULL;
  cont++;
  ApTemp->pageNum = cont;

  // validacao para saber se registro ficara na pagina nova ou na pagina atual
  if (i <= ORDEM + 1)
  {
    InsereNaPagina(ApTemp, Ap->r[2 * ORDEM - 1], Ap->p[2 * ORDEM]);
    Ap->n--;
    InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
  }

  // caso contrario
  else
  {

    // adiciona registro na nova pagina
    InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno);
  }

  // adiciona os elementos que estavam faltando na nova pagina
  for (j = ORDEM + 2; j <= 2 * ORDEM; j++)
    InsereNaPagina(ApTemp, Ap->r[j - 1], Ap->p[j]);

  // adiciona nova pagina como filho a direita do registro que foi inserido
  Ap->n = ORDEM;
  ApTemp->p[0] = Ap->p[ORDEM + 1];
  *RegRetorno = Ap->r[ORDEM];
  *ApRetorno = ApTemp;

  // caso a pagina tenha sido dividida, atualiza o apontador direito da pagina p
  for (j = ORDEM; j < Ap->n + 2; j++)
  {
    Aux.chave = 0;
    Aux.rank = 0;
    Ap->r[j] = Aux;
  }
} /*Ins*/

void Insere(Registro Reg, Apontador *Ap)
{
  int Cresceu;
  Registro RegRetorno;
  Apontador ApRetorno;
  Apontador ApTemp;

  Ins(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno);
  if (Cresceu)
  { /* Se arvore cresce na altura pela raiz*/
    ApTemp = (Apontador)malloc(sizeof(Pagina));
    ApTemp->n = 1;
    ApTemp->r[0] = RegRetorno;
    ApTemp->p[1] = ApRetorno;
    cont++;
    ApTemp->pageNum = cont;
    ApTemp->p[0] = *Ap;
    *Ap = ApTemp;
  }
  // salva alteracoes no arquivo
  saveAux(*Ap, 2 * ORDEM);
} /*Insercao*/

int Imprime2(Apontador p, int Nivel, int aux, int n)
{
  int i;

  if (p == NULL)
    return 0;
  if (Nivel == aux)
  {
    n++;
    printf("No: %d : ", p->pageNum);
    for (i = 0; i < p->n; i++)
    {
      if (p->p[i] != NULL)
        printf("Apontador: %d ", p->p[i]->pageNum);
      else
        printf("Apontador: null ");
      printf("chave: %d ", p->r[i].chave);
    }
    printf("\n");
    return n;
  }
  else
  {
    aux++;
    for (i = 0; i <= p->n; i++)
      Imprime2(p->p[i], Nivel, aux, n);
  }
  return 0;
}

int Imprime(Apontador p, int N, int aux, int n)
{
  int i;

  if (p == NULL)
    return 0;
  if (p->pageNum != aux)
  {
    p->num = N;
    aux = p->pageNum;
    return aux;
  }
  else
  {
    aux++;
    for (i = 0; i <= p->n; i++)
      Imprime(p->p[i], N, aux, n);
  }
  return 0;
}

void InsertPosFile(Registro Reg, Apontador *Ap)
{
  int Cresceu;
  Registro RegRetorno;
  Apontador ApRetorno;
  Apontador ApTemp;

  InsPosFile(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno);
  if (Cresceu)
  { /* Se arvore cresce na altura pela raiz */
    ApTemp = (Apontador)malloc(sizeof(Pagina));
    ApTemp->n = 1;
    ApTemp->r[0] = RegRetorno;
    ApTemp->p[1] = ApRetorno;
    cont++;
    ApTemp->pageNum = cont;
    ApTemp->p[0] = *Ap;
    *Ap = ApTemp;
  }
}

void InsPosFile(Registro Reg, Apontador Ap, int *Cresceu, Registro *RegRetorno, Apontador *ApRetorno)
{
  Apontador ApTemp;
  Registro Aux;
  int i, j;

  if (Ap == NULL) //
  {
    *Cresceu = 1;
    *RegRetorno = Reg;
    *ApRetorno = NULL;
    return;
  }
  i = 1;
  while (i < Ap->n && Reg.chave > Ap->r[i - 1].chave)
    i++;
  if (Reg.chave == Ap->r[i - 1].chave)
  {
    // printf("chave ja existente: %d \n", Reg.chave);
    *Cresceu = 0;
    return;
  }
  if (Reg.chave < Ap->r[i - 1].chave)
    InsPosFile(Reg, Ap->p[i - 1], Cresceu, RegRetorno, ApRetorno);
  else
    InsPosFile(Reg, Ap->p[i], Cresceu, RegRetorno, ApRetorno);

  if (!*Cresceu)
    return;

  if (Ap->n < 2 * ORDEM)
  { /* Verificando se a pagina tem espaco */
    InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
    *Cresceu = 0;
    return;
  }
  /* Split: Pagina tem que ser dividida */
  ApTemp = (Apontador)malloc(sizeof(Pagina));
  ApTemp->n = 0;
  ApTemp->p[0] = NULL;
  cont++;
  ApTemp->pageNum = cont;

  if (i <= ORDEM + 1)
  {
    InsereNaPagina(ApTemp, Ap->r[2 * ORDEM - 1], Ap->p[2 * ORDEM]);
    Ap->n--;
    InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
  }
  else
  {
    InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno);
  }

  for (j = ORDEM + 2; j <= 2 * ORDEM; j++)
    InsereNaPagina(ApTemp, Ap->r[j - 1], Ap->p[j]);

  Ap->n = ORDEM;
  ApTemp->p[0] = Ap->p[ORDEM + 1];
  *RegRetorno = Ap->r[ORDEM];
  *ApRetorno = ApTemp;
  for (j = ORDEM; j < Ap->n + 2; j++)
  {
    Aux.chave = 0;
    Aux.rank = 0;
    Ap->r[j] = Aux;
  }
}

void pos_ordem(Apontador raiz)
{
  int i;
  if (isleaf(raiz))
  {
    for (i = raiz->n - 1; i >= 0; i--)
    {
      printf("%d\n", raiz->r[i].chave);
    }
  }
  else
  {
    pos_ordem(raiz->p[raiz->n]);
    for (i = raiz->n - 1; i >= 0; i--)
    {
      printf("%d\n", raiz->r[i].chave);
      pos_ordem(raiz->p[i]);
    }
  }
}

void Pesquisa(Registro *x, Apontador Ap)
{
  int i;

  if (Ap == NULL)
  {
    // printf("Erro: Registro nao existe\n");

    return;
  }
  i = 1;
  while (i < Ap->n && x->chave > Ap->r[i - 1].chave)
    i++;
  if (x->chave == Ap->r[i - 1].chave)
  {
    *x = Ap->r[i - 1];
    return;
  }
  if (x->chave < Ap->r[i - 1].chave)
    Pesquisa(x, Ap->p[i - 1]);
  else
    Pesquisa(x, Ap->p[i]);
} /* Pesquisa */

void Reconstitui(Apontador ApPag, Apontador ApPai, int PosPai, int *Diminuiu)
{
  Apontador Aux;
  int DispAux, j;

  if (PosPai < ApPai->n)
  { /* Aux = Pagina a direita de ApPag */
    Aux = ApPai->p[PosPai + 1];
    DispAux = (Aux->n - ORDEM + 1) / 2;
    ApPag->r[ApPag->n] = ApPai->r[PosPai];
    ApPag->p[ApPag->n + 1] = Aux->p[0];
    ApPag->n++;
    if (DispAux > 0)
    { /* Existe folga: transfere de Aux para ApPag */
      for (j = 1; j < DispAux; j++)
        InsereNaPagina(ApPag, Aux->r[j - 1], Aux->p[j]);
      ApPai->r[PosPai] = Aux->r[DispAux - 1];
      Aux->n -= DispAux;
      for (j = 0; j < Aux->n; j++)
        Aux->r[j] = Aux->r[j + DispAux];
      for (j = 0; j <= Aux->n; j++)
        Aux->p[j] = Aux->p[j + DispAux];
      *Diminuiu = 0;
    }
    else
    { /* Fusao: intercala Aux em ApPag e libera Aux */
      for (j = 1; j <= ORDEM; j++)
        InsereNaPagina(ApPag, Aux->r[j - 1], Aux->p[j]);
      free(Aux);
      for (j = PosPai + 1; j < ApPai->n; j++)
      { /* Preenche vazio em ApPai */
        ApPai->r[j - 1] = ApPai->r[j];
        ApPai->p[j] = ApPai->p[j + 1];
      }
      ApPai->n--;
      if (ApPai->n >= ORDEM)
        *Diminuiu = 0;
    }
  }
  else
  { /* Aux = Pagina a esquerda de ApPag */
    Aux = ApPai->p[PosPai - 1];
    DispAux = (Aux->n - ORDEM + 1) / 2;
    for (j = ApPag->n; j >= 1; j--)
      ApPag->r[j] = ApPag->r[j - 1];
    ApPag->r[0] = ApPai->r[PosPai - 1];
    for (j = ApPag->n; j >= 0; j--)
      ApPag->p[j + 1] = ApPag->p[j];
    ApPag->n++;
    if (DispAux > 0)
    { /* Existe folga: transfere de Aux para ApPag */
      for (j = 1; j < DispAux; j++)
        InsereNaPagina(ApPag, Aux->r[Aux->n - j], Aux->p[Aux->n - j + 1]);
      ApPag->p[0] = Aux->p[Aux->n - DispAux + 1];
      ApPai->r[PosPai - 1] = Aux->r[Aux->n - DispAux];
      Aux->n -= DispAux;
      *Diminuiu = 0;
    }
    else
    { /* Fusao: intercala ApPag em Aux e libera ApPag */
      for (j = 1; j <= ORDEM; j++)
        InsereNaPagina(Aux, ApPag->r[j - 1], ApPag->p[j]);
      free(ApPag);
      ApPai->n--;
      if (ApPai->n >= ORDEM)
        *Diminuiu = 0;
    }
  }
} /* Reconstitui */

void Ret(int Ch, Apontador *Ap, int *Diminuiu)
{
  int Ind, j;
  Apontador WITH;
  Registro Reg;

  // Apontador nulo, chave nao encontrada
  if (*Ap == NULL)
  {
    printf("chave nao encontrada: %i\n", Ch);
    *Diminuiu = 0;
    return;
  }
  WITH = *Ap;
  Ind = 1;

  // busca chave dentro da pagina atual
  while (Ind < WITH->n && Ch > WITH->r[Ind - 1].chave)
    Ind++;

  // encontrou chave na pagina
  if (Ch == WITH->r[Ind - 1].chave)
  {

    // seta informacoes para nulo no registro a ser retirado
    Reg.chave = 0;
    Reg.rank = 0;
    WITH->r[Ind - 1] = Reg;

    // verifica se apontador a esquerda do registro retirado eh nulo
    if (WITH->p[Ind - 1] == NULL)
    { /* Pagina folha */

      // decrementa contador de elementos
      WITH->n--;

      // indica que pagina esta subcarregada
      *Diminuiu = WITH->n < ORDEM;

      // desloca registros e ponteiros para a esquerda
      for (j = Ind; j <= WITH->n; j++)
      {
        WITH->r[j - 1] = WITH->r[j];
        WITH->p[j] = WITH->p[j + 1];
      }
      return;
    }

    // busca antecessor do registro removido
    Antecessor(*Ap, Ind, WITH->p[Ind - 1], Diminuiu);

    // caso diminuiu em altura
    if (*Diminuiu)
      Reconstitui(WITH->p[Ind - 1], *Ap, Ind - 1, Diminuiu);
    return;
  }
 
  //chave esta na ultima pagina filha (mais a direita)
  if (Ch > WITH->r[Ind - 1].chave)
    Ind++;

  //chama recursao na pagina filha
  Ret(Ch, &WITH->p[Ind - 1], Diminuiu);

  // caso diminuiu em altura
  if (*Diminuiu)
    Reconstitui(WITH->p[Ind - 1], *Ap, Ind - 1, Diminuiu);
} /* Ret */

void Retira(int Ch, Apontador *Ap)
{
  int Diminuiu;
  Apontador Aux;

  Ret(Ch, Ap, &Diminuiu);
  if (Diminuiu && (*Ap)->n == 0)
  { /* Arvore diminui na altura */
    Aux = *Ap;
    *Ap = Aux->p[0];
    free(Aux);
  }

} /* Retira */

void recuperarReg(Apontador *arv, node *LISTA)
{

  FILE *arq;
  Registro Reg[2 * ORDEM];
  node *tmp;
  int i = 0, j = 0, tam;

  // Abre arquivo
  arq = fopen(namefile, "rb");

  // caso abertura falhou, encerra programa
  if (arq == NULL)
    exit(1);

  // posiciona cursor no fim do arquivo
  fseek(arq, 0, SEEK_END);

  // armazena o tamanho do arquivo
  tam = ftell(arq);

  // seta o cursor para o inicio do arquivo
  rewind(arq);

  // fecha arquivo
  fclose(arq);

  // abre arquivo
  arq = fopen(namefile, "rb");

  // caso falha na abertura, encerra programa
  if (arq == NULL)
    exit(1);

  // le as informacoes, enquanto nao exceder o tamanho do arquivo
  while (j * (2 * ORDEM * sizeof(Registro)) < tam)
  {

    // posiciona cursor na j-esima pagina
    fseek(arq, j * (2 * ORDEM * sizeof(Registro)), SEEK_SET);

    // realiza leiura na j-esima pagina
    fread(Reg, 2 * ORDEM * sizeof(Registro), 1, arq);

    // armazena cada registro em uma lista
    for (i = 0; i < 2 * ORDEM; i++)
    {
      // valida para caso de registro nulo (chave == 0)
      if (Reg[i].chave > 0)
        execut(Reg[i], LISTA);
    }
    j++;
  }

  // fecha arquivo
  fclose(arq);

  tmp = LISTA->prox;

  // percorre lista, contruindo a arvore a partir dos registros armazenados nela
  while (tmp != NULL)
  {

    // chama funcao para inserir os registros na arvore
    InsertPosFile(tmp->info, arv);
    tmp = tmp->prox;
  }

  // libera lista
  free(tmp);
  free(LISTA);
}

void saveAux(Apontador p, int Nivel)
{
  int i, j;

  if (p == NULL)
    return;

  // salva os registros da pagina atual no arquivo
  for (i = 0; i < p->n; i++)
    salvar(p, p->r);

  // acessa todas as paginas filhas da pagina atual
  for (j = 0; j <= p->n; j++)
    saveAux(p->p[j], Nivel + 1);
}

void salvar(Apontador pagina, Registro Reg[])
{

  FILE *arq;

  // se arquivo nao existe
  if (!file_exists(namefile))
  {
    // cria e abre o arquivo
    arq = fopen(namefile, "wb");

    // caso abertura falhou, encerra programa
    if (arq == NULL)
      exit(1);

    // posiciona cursor na posicao onde serao salvos os registros
    fseek(arq, pagina->pageNum * (2 * ORDEM * sizeof(Registro)), SEEK_SET);

    // escreve registros no arquivo
    fwrite(Reg, (2 * ORDEM * sizeof(Registro)), 1, arq);

    // fecha arquivo
    fclose(arq);
  }

  else
  {
    // abre arquivo
    arq = fopen(namefile, "r+b");

    // caso abertura falhou, encerra programa
    if (arq == NULL)
      exit(1);

    // posiciona cursor na posicao onde serao salvos os registros
    fseek(arq, pagina->pageNum * (2 * ORDEM * sizeof(Registro)), SEEK_SET);

    // escreve registros no arquivo
    fwrite(Reg, (2 * ORDEM * sizeof(Registro)), 1, arq);

    // fecha arquivo
    fclose(arq);
  }
}

// ############################### //Estrutura 1 Btree

// ############################### //Estrutura 2 lista encadeada
void insereInicio(Registro info, node *LISTA)
{
  node *novo = (node *)malloc(sizeof(node));
  node *tmp = busca(info, LISTA);
  if (LISTA->prox == NULL)
  {
    novo->info = info;
    novo->prox = LISTA->prox;
    LISTA->prox = novo;
  }
  else
  {
    novo->prox = tmp->prox;
    novo->info = info;
    tmp->prox = novo;
  }
  // exibe(LISTA);
}

node *busca(Registro info, node *LISTA)
{
  node *atual, *antNode;
  atual = LISTA->prox;
  antNode = LISTA;
  while (atual != NULL && info.rank > atual->info.rank)
  {
    antNode = atual;
    atual = atual->prox;
  }
  return antNode;
}

void exibe(node *LISTA)
{
  if (!LISTA)
  {
    return;
  }
  node *tmp;
  tmp = LISTA->prox;

  while (tmp != NULL)
  {
    printf(" %d ", tmp->info.chave);
    tmp = tmp->prox;
  }
  printf("\n");
}

void execut(Registro info, node *LISTA)
{
  // insere registro no inicio da lista
  insereInicio(info, LISTA);
}
// ############################# //Estrutura 2 lista encadeada

int main()
{
  Apontador *arv;
  Registro reg;
  char tecla;
  int chave, i, num = 0;
  arv = (Apontador *)malloc(sizeof(Apontador));
  node *LISTA = (node *)malloc(sizeof(node));
  Inicializa(arv);
  inicia(LISTA);
  if (file_exists(namefile))
  {
    recuperarReg(arv, LISTA);
  }
  printf("i - insercao \n");
  printf("r - remove \n");
  printf("o - exibir \n");
  printf("c - consultar \n");
  while (1)
  {
    scanf("%c", &tecla);
    if (tecla == 'e')
      break;
    switch (tecla)
    {
    case 'i':
      printf("insercao \n");
      scanf("%d", &chave);
      reg.chave = chave;
      count++;
      reg.rank = count;
      fflush(stdin);
      getchar();
      fgets(reg.nome, sizeof(reg.nome), stdin);
      scanf("%d", &reg.idade);
      if (reg.chave <= 0)
      {
        count--;
        break;
      }
      Insere(reg, arv);
      break;
    case 'r':
      printf("remove \n");
      scanf("%d", &chave);
      reg.chave = chave;
      Retira(reg.chave, arv);
      remove(namefile);
      saveAux(*arv, 2 * ORDEM);
      break;
    case 'o':
      printf("exibir \n");
      em_ordem(*arv);
      break;
    case 'p':
      for (i = 0; i < 2 * ORDEM; i++)
      {
        num = Imprime2(*arv, i, 0, num);
      }
      break;
    case 'c':
      printf("consulta \n");
      getchar();
      scanf("%d", &chave);
      reg.chave = chave;
      Busca(reg, *arv);
      break;
    }

    fflush(stdin);
  }
  return 0;
}
