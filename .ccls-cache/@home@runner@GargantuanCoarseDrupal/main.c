#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define TAM 256


typedef struct no{
  unsigned char caracter;
  int freq;
  struct no* dir;
  struct no* esq;
  struct no* prox;
}NO;

typedef struct{
  NO *inicio;
  int tam;
}Lista;


void inicializa_tab (unsigned int tab[]){
  int i;

  for(i = 0; i < TAM; i++){
    tab[i] = 0;
  }
}

void preenche_tab_freq(unsigned char texto[],unsigned int tab []){
  int i  = 0;

  while(texto[i] != '\0'){
    tab[texto[i]]++;
    i++;
  }
}

void imprimir_tab_freq (unsigned int tab[]){
  int i;

  for(i = 0; i < TAM; i++){
    if(tab[i] > 0)
      printf("\t%d = %u = %c\n",i, tab[i],i); //%u = %d para unsigned
  }
}

void criar_lista(Lista *lista){
  lista -> inicio = NULL;
  lista -> tam = 0;
}

void inserirOrdenado(Lista *lista, NO* no){

  NO *aux;
  
  //A lista esta vazia?
  if(lista -> inicio == NULL){
    lista -> inicio = no;
  }
  //tem frequencia menor q o inicio da lista
  else if(no -> freq < lista -> inicio -> freq){
    no -> prox = lista -> inicio;
    lista -> inicio = no;
  }
  else{
    aux = lista -> inicio;

    while(aux -> prox && aux -> prox -> freq <= no -> freq){
      aux = aux -> prox;
    }

    no -> prox = aux -> prox;
    aux -> prox = no;
  }  
  lista -> tam++;
}

void preencher_lista(unsigned int tab[], Lista* lista){

  int i;

  NO* novo;

  for(i = 0; i < TAM; i++){
    if(tab[i] > 0){
      novo = (NO*)malloc(sizeof(struct no));

      if(novo){
        novo -> caracter = i;
        novo -> freq = tab[i];
        novo -> dir = NULL;
        novo -> esq = NULL;
        novo -> prox = NULL;

        inserirOrdenado(lista, novo);
      }
      else{
        printf("\tErro ao alocar memoria em preencher_lista!\n");
        break;
      }
    }
  }
  
}

void imprimirLista(Lista* lista){

  NO* aux = lista -> inicio;

  printf("\tLista ordenada tamanho: %d\n", lista -> tam);
  
  while(aux){
    printf("\tCaractere: %c Frequência: %d\n",aux -> caracter, aux -> freq);
    aux = aux -> prox;
  }
  
}

NO* removerInicio(Lista *lista){
  NO* aux = NULL;
  
  if(lista -> inicio){
    aux = lista -> inicio;
    lista -> inicio = aux -> prox;
    aux -> prox = NULL;
    lista -> tam--;
  }
  
  return aux;
}

NO* montarArvore(Lista* lista){

  NO *primeiro, *segundo, *novo;
  
  while(lista -> tam > 1){
    primeiro = removerInicio(lista);
    segundo = removerInicio(lista);
    novo = (NO*)malloc(sizeof(struct no));

    if(novo){
      novo -> caracter = '+';
      novo -> freq = primeiro -> freq + segundo -> freq;
      novo -> esq = primeiro;
      novo -> dir = segundo;
      novo -> prox = NULL;

      inserirOrdenado(lista, novo);
    }
    else{
      printf("\tErro ao alocar memoria em montarArvore!\n");
      break;
    }
  }
  return lista -> inicio;
}

void imprimirArvore(NO *raiz, int tam){

  if(raiz-> esq == NULL && raiz -> dir == NULL){
    printf("\tFolha : %c\tAltura: %d\n", raiz -> caracter, tam);
  }
  else{
    imprimirArvore(raiz -> esq, tam + 1);
    imprimirArvore(raiz -> dir, tam + 1);
  }
}

//MONTAR DICIONARIO

int alturaArvore(NO *raiz){

  int esq, dir;
  
  if(raiz == NULL)
    return -1;
  else{
    esq = alturaArvore(raiz -> esq) + 1;
    dir = alturaArvore(raiz -> dir) + 1;
    
    if(esq > dir)
      return esq;
    else
      return dir;
  }
}

char** alocaDicionario(int colunas){
  char** dicionario;
  int i;
  
  dicionario = malloc(sizeof(char*) * TAM);
  
  for(i = 0; i < TAM; i++){
    dicionario[i] = calloc(colunas, sizeof(char));
  }

  return dicionario;
  
}

void gerarDicionario(char** dicionario, NO*raiz, char* caminho, int colunas){

  char esquerda[colunas], direita[colunas];

  if(raiz->esq == NULL && raiz -> dir == NULL){
    strcpy(dicionario[raiz->caracter],caminho);
  }
  else{
    strcpy(esquerda, caminho);
    strcpy(direita, caminho);

    strcat(esquerda, "0");
    strcat(direita, "1");

    gerarDicionario(dicionario, raiz->esq, esquerda, colunas);
    gerarDicionario(dicionario, raiz->dir, direita, colunas);
  }
}

void imprimeDicionario(char** dicionario){

  int i;

  printf("\n\tDicionario:\n");
  for(i = 0; i < TAM; i++){
    if(strlen(dicionario[i]) > 0)
      printf("\t%d: %s\n",i,dicionario[i]);
  }
  
}

int calculaTamString(char** dicionario, unsigned char *texto){
  int i = 0, tam = 0;

  while(texto[i] != '\0'){
    tam = tam + strlen(dicionario[texto[i]]);
    i++;
  }

  return tam + 1;
}

char* codificar(char** dicionario, unsigned char* texto){
  
  int i = 0, tam = calculaTamString(dicionario, texto);
  char *codigo = calloc(tam, sizeof(char));

  while(texto[i] != '\0'){
    strcat(codigo, dicionario[texto[i]]);
    i++;
  }

  return codigo;
}

char* decodificar(unsigned char texto[], NO*raiz){

  int i = 0;
  NO* aux = raiz;
  char* decodificado = calloc(strlen(texto), sizeof(char));
  char temp[2];

  while(texto[i] != '\0'){

    if(texto[i] == '0'){
      aux = aux -> esq;
    }
    else{
      aux = aux -> dir;
    }

    if(aux -> dir == NULL && aux -> esq == NULL){
      temp[0] = aux -> caracter;
      temp[1] = '\0';
      strcat(decodificado, temp);
      aux = raiz;
    }
    
    i++;
  }

  return decodificado;
}

//COMPACTAR

void compactar(unsigned char str[]){

  FILE *arquivo = fopen("compactado.wg", "wb");

  int i  = 0, j = 7;
  unsigned char mascara, byte = 0;
  
  if(arquivo){
    while(str[i] != '\0'){
      mascara = 1;
      if(str[i] == '1'){
        mascara = mascara << j;
        byte = byte | mascara;
      }
      j--;

      if(j < 0){
        fwrite(&byte, sizeof(unsigned char), 1, arquivo);
        byte = 0;
        j = 7;
      }
      i++;
    }
    if(j != 7){
      fwrite(&byte, sizeof(unsigned char), 1, arquivo);
    }
    fclose(arquivo);
  }
  else{
    printf("\n\nErro ao abrir ou criar o arquivo em COMPACTAR\n");
  }
  
}

unsigned int ehBitUm(unsigned char byte, int i){
  unsigned char mascara = (1 << i);

  return byte & mascara;
}

void descompactar(NO* raiz){

  FILE *arquivo = fopen("compactado.wg", "rb");
  NO*aux = raiz;
  
  unsigned char byte;
  int i;
  
  if(arquivo){
    
    while(fread(&byte,sizeof(unsigned char), 1, arquivo)){
      for(i = 7; i >= 0; i--){
        if(ehBitUm(byte, i))
          aux = aux -> dir;
        else
          aux = aux -> esq;

        if(aux -> esq == NULL && aux -> dir == NULL){
          printf("%c", aux->caracter);//pode escrever em arquivo ou o q quiser
          aux = raiz;
        }
      }
    }
    fclose(arquivo);
  }
  else{
    printf("\n\nErro ao abrir o arquivo em DESCOMPACTAR\n");
  }
  
}

int descobrirTam(){
  FILE *arq = fopen("teste.txt","r");
  int tam = 0;

  if(arq){
    while(fgetc(arq) != -1){
      tam++;
    }
    fclose(arq);
  }
  else{
    printf("\n\nErro ao abrir o arquivo em descobrirTam\n");
  }

  return tam;
}

void lerTexto(unsigned char *texto){
  FILE *arq = fopen("teste.txt","r");
  char letra;
  int i = 0;
  
  if(arq){
    while(!feof(arq)){
      letra = fgetc(arq);
      if(letra != -1){
        texto[i] = letra;
        i++;
      }
    }
    fclose(arq);
  }
  else{
    printf("\n\nErro ao abrir o arquivo em lerTexto\n");
  }  
}

int main(void) {

  setlocale(LC_ALL, "Portuguese");

  //unsigned char texto[] = "Vamos aprender programação";
  unsigned char *texto;
  unsigned int tab_freq[TAM];

  Lista lista;
  NO* arvore;
  int colunas, tam;
  char** dicionario;
  char* codificado, *decodificado;
  
  tam = descobrirTam();

  printf("\nQuantidade: %d\n", tam);

  texto = calloc(tam + 2, sizeof(unsigned char));

  lerTexto(texto);
  //printf("\n\nTEXTO:\n%s\n",texto);
  
  //printf("%s\n",texto);

  inicializa_tab(tab_freq);
  preenche_tab_freq(texto,tab_freq);
  imprimir_tab_freq(tab_freq);


  criar_lista(&lista);
  preencher_lista(tab_freq,&lista);
  imprimirLista(&lista);

  arvore = montarArvore(&lista);

  printf("\n\tArvore de Huffman\n");
  imprimirArvore(arvore, 0);

  //DICIONARIO
  
  colunas = alturaArvore(arvore) + 1;
  dicionario = alocaDicionario(colunas);
  gerarDicionario(dicionario, arvore, "",colunas);
  imprimeDicionario(dicionario);

  //codificado

  codificado = codificar(dicionario, texto);
  printf("\n\tTexto codificado: %s\n",codificado);

  //decodificado
  
  decodificado = decodificar(codificado, arvore);
  //printf("\n\tTexto decodificado : %s\n", decodificado);

  //COMPACTAR

  compactar(codificado);

  //descompactar

  //printf("\n\nARQUIVO DESCOMPACTADO!\n");
  descompactar(arvore);
  printf("\n\n");

  free(texto);
  free(codificado);
  free(decodificado);
  
  return 0;
}