/*
Arquivo: q2.c
Autor: Felipe Viel
Função do arquivo: cria 4 threads com pthreads, 3 para controlar itens que detectados em 3 esteiras e 1 para exibição em display
                   cria N threads com OpenMP para calcular o vetor com 1500 pesos obtidos nas 3 esteiras
Criado em 28 de outubro de 2020
Modificado em 4 de novembro de 2020

compilar usando: gcc -o q2 q2.c -lpthread -fopenmp
especificar o nÃºmero de threads: export OMP_NUM_THREADS=4
*/

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <omp.h>

#define QTD_PESOS 10

pthread_mutex_t exclusao_mutua = PTHREAD_MUTEX_INITIALIZER;

double sensor_lido = 0;
int itens_pesados = 0;
double v_pesos[QTD_PESOS];
double pesagem_total = 0;
int pesagem = 0;


void atualiza_sensor();
void atualiza_display();
double sum(double *v, int N);

int main(){
   pthread_t t1, t2, t3, t4;

   /*cria as threads sem parametros e sem argumentos*/
   pthread_create(&t1, NULL, (void *) atualiza_sensor, NULL); /*criando a thread para a esteira 1*/
   pthread_create(&t2, NULL, (void *) atualiza_sensor, NULL); /*criando a thread para a esteira 2*/
   pthread_create(&t3, NULL, (void *) atualiza_sensor, NULL); /*criando a thread para a esteira 3*/
   pthread_create(&t4, NULL, (void *) atualiza_display, NULL); /*criando a thread para atualizar o display*/

   /*thread principal espera as threas criadas retornarem para encerrar o processo*/
   pthread_join(t1, NULL);
   pthread_join(t2, NULL);
   pthread_join(t3, NULL);
   pthread_join(t4, NULL);

   return 0;
}

/*função para a thread realizar a atualização de itens detectados*/
void atualiza_sensor(){
   double lido = 0;
   double peso_esteira = 1.0;
   while(1){
      if(pesagem == 0){ //verifica se irÃ¡ se estÃ¡ sendo feita uma pesagem
         
         pthread_mutex_lock( &exclusao_mutua); /*Trava a sessão critica*/
         
         /*Sessão critica - variáveis globais que armazenam todo o valor de detecções nas tres esteiras e os pesos
         a atribuição de "lido" é para simular o dado que é lido dos sensores das esteiras
         o v_pesos armazena os pesos e a itens_pesados indica quantidade*/
         sensor_lido += lido;//lido; 
         if(QTD_PESOS > itens_pesados){ //verificação chegou ou não na quantidade a ser pesada
            v_pesos[itens_pesados] = peso_esteira;
            itens_pesados++;
         }else{
            pesagem = 1; //inicia o processo de pesagem
            pesagem_total = sum(v_pesos, QTD_PESOS);
         }

         pthread_mutex_unlock( &exclusao_mutua);/*destrava a sessão critica*/
      }

      /*Simula o intervalo de tempo que itens passarão pelo sensor da esteira*/
      usleep(1000 * 1000); 

      /*Simula diferentes quantidades de itens passando ao mesmo tempo pelo sensor da esteira*/
      lido++;

      /*Simula diferentes pesos de itens na esteira*/
      //peso_esteira+=0.5; 
   }
}

/*função para a thread realizar para exibi a quantidade e peso de itens detectados*/
void atualiza_display(){ 
   while(1){
      pthread_mutex_lock( &exclusao_mutua); /*Trava a sessão critica*/

      /*Sessão critica - variáveis globais que armazenan todo o valor de detecções nas tres esteiras
      o printf() simula a atualização do display */
      printf("\nItens Total: %lf", sensor_lido);  
      if(pesagem == 1){ //verifica se ocorreu uma pesagem para atualizar o display
         printf("\nPeso total: %lf", pesagem_total);
         itens_pesados = 0;
         pesagem = 0;
      }
      pthread_mutex_unlock( &exclusao_mutua); /*destrava a sessão critica*/
      
      /* tempo para ataulizar o display e que é percepti­vel ao olho humano */
      usleep(1000 * 1000);            
   }
}

/*função para somar os pesos detectados
   input: v - vetor com QTD_PESOS
          N - QTD_PESOS
   output: sum - peso total
*/
double sum(double *v, int N){
   int i;
   double sum = 0;

   /* 
   cria-se várias threads para processar cada uma uma parte do vetor
   private - indica que cada thread tem o seu próprio "i"
   reduction(op : list) - indica que cada thread calcula o seu "sum" (list)
               e posterioremten é feita a operação com todos os "sum", 
               sendo a operação (op) uma soma +
   */
   #pragma omp parallel for private(i) reduction(+ : sum)
   for(i = 0; i < N; i++){
      sum += v[i];
   }

   return sum;
}