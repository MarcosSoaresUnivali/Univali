/*
Arquivo: q1.c
Autor: Felipe Viel
Função do arquivo: cria 4 threads, 3 para controlar itens que detectados em 3 esteiras e 1 para exibição em display
Criado em 28 de outubro de 2020
Modificado em 4 de novenbro de 2020

compilar usando: gcc -o q1 q1.c -lpthread
*/

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

//#include "sensor.h"

pthread_mutex_t exclusao_mutua = PTHREAD_MUTEX_INITIALIZER;

double sensor_lido = 0;

//double limite_atual = HUGE_VAL;

void atualiza_sensor();
void atualiza_display();

int main(){
   pthread_t t1, t2, t3, t4;

   
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
   while(1){
      pthread_mutex_lock( &exclusao_mutua); /*Trava a sessão critica*/

      /*Sessão critica - variável global que armazena todo o valor de detecções nas tres esteiras
      a atribuição de "lido" é para simular o dado que é lido dos sensores das esteiras*/
      sensor_lido += lido;//uart1_get_value() 

      pthread_mutex_unlock( &exclusao_mutua);/*destrava a sessão critica*/

      /*Simula o intervalo de tempo que itens passarão pelo sensor da esteira*/
      usleep(1000 * 1000); 

      /*Simula diferentes quantidades de itens passando ao mesmo tempo pelo sensor da esteira*/
      lido++;
   }
}


void atualiza_display(){ 
   while(1){
      pthread_mutex_lock( &exclusao_mutua); /*Trava a sessão critica*/

      /*Sessão critica - variável global que armazena todo o valor de detecções nas tres esteiras
      o printf() simula a atualização do display */
      printf("\nPeso total: %lf", sensor_lido);  

      pthread_mutex_unlock( &exclusao_mutua);/*destrava a sessão critica*/

      /* tempo para atualizar o display e que é percepti­vel ao olho humano */
      usleep(1000 * 1000);            
   }
}