#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

float *v; //vetor
long long int tamanho; //tamanho do vetor
int NTHREADS; //numero de threads
float concmax,concmin; //variaveis para auxiliar no maior e menor valor do vetor

void sequencialzada(float *k) //funcao sequencial para achar maior e menor valor
{

    int i;
    float grande=k[0], pequeno=k[0];
    for (i = 0; i < tamanho; i+=NTHREADS-1) {
        if (k[i] > grande) grande = k[i];
        if (k[i] < pequeno) pequeno= k[i];}
        printf("Na sequencial: maior valor eh %.5f e o menor eh %.5f\n\n",grande,pequeno);
}

void * threadzada (void *arg) //funcao da thread para encontrar o maior e o menor vetor
{
    int i=0;
    int j = *(int *) arg;
    float maior = v[0];
    float menor = v[0];
     for (i = j; i < tamanho; i+=NTHREADS-1) { //pegando o vetor por partes de acordo com o numero de threads
        if (v[i] > maior) maior = v[i];
        if (v[i] < menor) menor = v[i];
        concmax=maior;
        concmin=menor;
    }
    if(j>=NTHREADS-1){ //printa quando estiver finalizando a ultima thread
        printf("Na thread: maior valor eh %.5f e o menor eh %.5f\n\n",concmax,concmin);
    }


    pthread_exit(NULL); //finalizando
}

int main( int argc, char *argv[] )
{

    int i;
    double inicio,fim,seq,conc; //variaveis de tempo
    concmax=0;
    concmin=1000;
    if(argc<3){
            printf("Digite: %s <numero de threads> <dimensao da matriz>\n\n", argv[0]);
            return 1;}

    NTHREADS=atoi(argv[1]);  //argumentos da linha de comando
    tamanho=atoll(argv[2]);

     if(NTHREADS>tamanho){ //conferindo a comparação de tamanho do vetor com thread
        printf("o numero de threads nao deveria ser maior que o tamanho\n\n");
        return 2;
    }

    pthread_t tid[NTHREADS]; //identificador da thread
    int init[NTHREADS];
    for(i=0;i<NTHREADS;i++)
    {
        init[i]=i;

    }

    v=malloc(sizeof(float) * tamanho);
     if(v==NULL){
        printf("erro no malloc");
        return 6;}

    for(i=0;i<tamanho;i++)
    {
        v[i]=(((float)rand()/(float)(RAND_MAX)) * 100); //gerando valores float aleatórios até 100
        //printf("valor numero %d = %.5f\n",i+1,v[i]); //tirar as barras para testar os numeros
    }

    puts("");

    GET_TIME(inicio);


    for(i=0;i<NTHREADS;i++){ //criando thread
        if(pthread_create(&tid[i],NULL,threadzada,(void*) (init+i))){
            printf("erro no pthread_create");
            return 4;}}

    for(i = 0; i < NTHREADS; i++) {  //funcao pthread join
    if (pthread_join(*(tid+i), NULL)) {
      puts("erro no pthread_join");
      return 5;}
  }

    GET_TIME(fim); //calculando os tempos
    conc=fim-inicio;
    GET_TIME(inicio);

    sequencialzada(v); //usando a funcao sequencial
    GET_TIME(fim);
    seq=fim-inicio;
    printf("O tempo concorrente eh %e\n\nO tempo sequencial eh %e\n\nO ganho de desempenho eh %e\n\n",conc,seq,seq/conc);
    pthread_exit(NULL);

    return 0;
    }
