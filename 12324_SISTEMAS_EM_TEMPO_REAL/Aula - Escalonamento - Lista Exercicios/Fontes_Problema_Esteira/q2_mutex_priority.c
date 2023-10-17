/*
Arquivo: q2_mutex_priority.c
Autor: Felipe Viel
Funçao do arquivo: cria 4 threads com pthreads, 3 para controlar itens que detectados em 3 esteiras e 1 para exibiçao em display
                   cria N threads com OpenMP para calcular o vetor com 1500 pesos obtidos nas 3 esteiras
                   exemplifica o uso do protocolo herança de prioridade com a pthreads, além de mostrar como atribuir prioridade as threads
Criado em 28 de outubro de 2020
Modificado em 4 de novenbro de 2020

compilar usando: gcc -o q2_mutex_priority q2_mutex_priority.c -lpthread -fopenmp -lrt
especificar o naºmero de threads: export OMP_NUM_THREADS=4
*/

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sched.h>
#include <string.h>
#include <errno.h>
#include <omp.h>

#define QTD_PESOS 10
//#define DEBUG 1

#ifndef _POSIX_THREAD_PRIO_INHERIT
#error "This system does not support priority inherit protection in mutex"
#endif

pthread_mutex_t exclusao_mutua = PTHREAD_MUTEX_INITIALIZER;
pthread_mutexattr_t  mutexattr_prioinherit;

double sensor_lido = 0;
int itens_pesados = 0;
double v_pesos[QTD_PESOS];
double pesagem_total = 0;
int pesagem = 0;

//double limite_atual = HUGE_VAL;

void atualiza_sensor1();
void atualiza_sensor2();
void atualiza_sensor3();
void atualiza_display();
double sum(double *v, int N);
void init_mutex_inh();

int main(){
   pthread_t t1, t2, t3, t4;
   pthread_attr_t tattr;
   int newprio = 60;
   struct sched_param param;

   /* Trocar a política de escalonamento do processo para os tipos existentes na função sched_setscheduler: */
   param.sched_priority = newprio; // Defina a prioridade desejada
   int ret = sched_setscheduler(0, SCHED_FIFO, &param);

   /*
      ps -e -o uid,pid,ppid,pri,ni,cmd
      top -p <PID>

      1.b) Investigar a diferença/semelhança entre pthread_attr_setschedpolicy e pthread_attr_setschedparam:

         pthread_attr_setschedpolicy é usado para definir a política de escalonamento de uma thread, 
         como SCHED_FIFO ou SCHED_RR.
         pthread_attr_setschedparam é usado para definir os parâmetros de escalonamento de uma thread, 
         como a prioridade.  
   */

   
   /*chama a funçao para inicializar o mutex com a herança de prioridade*/
   init_mutex_inh();
   
   /* inicializado com atributos padrao */
   pthread_attr_init (&tattr);

   /* obter o parametro de programaçao existente */
   pthread_attr_getschedparam (&tattr, &param);

   /* definir a prioridade; outros parametros nao mudaram */
   param.sched_priority = 98;

   /* definindo o novo parametro de escalonamento */
   pthread_attr_setschedparam (&tattr, &param);
  
   /*criando a thread para a esteira 1*/
   pthread_create(&t1, &tattr, (void *) atualiza_sensor1, NULL); 

   /* definir a prioridade; outros parametros nao mudaram */
   param.sched_priority = 99;

   /* definindo o novo parametro de escalonamento */
   pthread_attr_setschedparam (&tattr, &param);

   /*criando a thread para a esteira 2*/
   pthread_create(&t2, NULL, (void *) atualiza_sensor2, NULL);

   /* definir a prioridade; outros parametros nao mudaram */
   param.sched_priority = 2;

   /* definindo o novo parametro de escalonamento */
   pthread_attr_setschedparam (&tattr, &param);

   /*criando a thread para a esteira 3*/
   pthread_create(&t3, NULL, (void *) atualiza_sensor3, NULL);

   /* definir a prioridade; outros parametros nao mudaram */
   param.sched_priority = 22;

   /* definindo o novo parametro de escalonamento */
   pthread_attr_setschedparam (&tattr, &param);

   /*criando a thread para atualizar o display*/
   pthread_create(&t4, NULL, (void *) atualiza_display, NULL);

   /*thread principal espera as threas criadas retornarem para encerrar o processo*/
   pthread_join(t1, NULL);
   pthread_join(t2, NULL);
   pthread_join(t3, NULL);
   pthread_join(t4, NULL);

   return 0;
}

void atualiza_sensor1(){
   double lido = 0;
   double peso_esteira = 1.0;
   while(1){
      if(pesagem == 0){//verifica se ira¡ se esta¡ sendo feita uma pesagem
         
         pthread_mutex_lock( &exclusao_mutua); /*Trava a seçao critica*/

         /*Seçao critica - varia¡veis globais que armazenam todo o valor de detecções nas tres esteiras e os pesos
         a atribuiçao de "lido" é para simular o dado que é lido dos sensores das esteiras
         o v_pesos armazena os pesos e a itens_pesados indica quantidade*/
         sensor_lido += lido;//lido; 

         if(QTD_PESOS > itens_pesados){ //verificaçao chegou ou nao na quantidade a ser pesada
            v_pesos[itens_pesados] = peso_esteira;
            itens_pesados++;
         }else{
            pesagem = 1; //inicia o processo de pesagem
            pesagem_total = sum(v_pesos, QTD_PESOS);
         }
#ifdef DEBUG
         printf("\nEntrei 1");
#endif
         pthread_mutex_unlock( &exclusao_mutua); /*destrava a seçao critica*/
      }

      /*Simula o intervalo de tempo que itens passao pelo sensor da esteira*/
      usleep(1000 * 1000); 
      
      /*Simula diferentes quantidades de itens passando ao mesmo tempo pelo sensor da esteira*/
      lido++;

      /*Simula diferentes pesos de itens na esteira*/
      //peso_esteira+=0.5; 
   }
}

void atualiza_sensor2(){
   double lido = 0;
   double peso_esteira = 1.0;
   while(1){
      if(pesagem == 0){//verifica se ira¡ se esta¡ sendo feita uma pesagem
         
         pthread_mutex_lock( &exclusao_mutua); /*Trava a seçao critica*/

         /*Seçao critica - varia¡veis globais que armazenam todo o valor de detecções nas tres esteiras e os pesos
         a atribuiçao de "lido" é para simular o dado que é lido dos sensores das esteiras
         o v_pesos armazena os pesos e a itens_pesados indica quantidade*/
         sensor_lido += lido;//lido; 

         if(QTD_PESOS > itens_pesados){ //verificaçao chegou ou nao na quantidade a ser pesada
            v_pesos[itens_pesados] = peso_esteira;
            itens_pesados++;
         }else{
            pesagem = 1; //inicia o processo de pesagem
            pesagem_total = sum(v_pesos, QTD_PESOS);
         }
#ifdef DEBUG
         printf("\nEntrei 2");
#endif
         pthread_mutex_unlock( &exclusao_mutua); /*destrava a seçao critica*/
      }

      /*Simula o intervalo de tempo que itens passao pelo sensor da esteira*/
      usleep(1000 * 1000); 
      
      /*Simula diferentes quantidades de itens passando ao mesmo tempo pelo sensor da esteira*/
      lido++;

      /*Simula diferentes pesos de itens na esteira*/
      //peso_esteira+=0.5; 
   }
}

void atualiza_sensor3(){
   double lido = 0;
   double peso_esteira = 1.0;
   while(1){
      if(pesagem == 0){//verifica se ira¡ se esta¡ sendo feita uma pesagem
         
         pthread_mutex_lock( &exclusao_mutua); /*Trava a seçao critica*/

         /*Seçao critica - varia¡veis globais que armazenam todo o valor de detecções nas tres esteiras e os pesos
         a atribuiçao de "lido" é para simular o dado que é lido dos sensores das esteiras
         o v_pesos armazena os pesos e a itens_pesados indica quantidade*/
         sensor_lido += lido;//lido; 

         if(QTD_PESOS > itens_pesados){ //verificaçao chegou ou nao na quantidade a ser pesada
            v_pesos[itens_pesados] = peso_esteira;
            itens_pesados++;
         }else{
            pesagem = 1; //inicia o processo de pesagem
            pesagem_total = sum(v_pesos, QTD_PESOS);
         }
#ifdef DEBUG
         printf("\nEntrei 3");
#endif
         pthread_mutex_unlock( &exclusao_mutua); /*destrava a seçao critica*/
      }

      /*Simula o intervalo de tempo que itens passao pelo sensor da esteira*/
      usleep(1000 * 1000); 
      
      /*Simula diferentes quantidades de itens passando ao mesmo tempo pelo sensor da esteira*/
      lido++;

      /*Simula diferentes pesos de itens na esteira*/
      //peso_esteira+=0.5; 
   }
}

/*funçao para a thread realizar para exibi a quantidade e peso de itens detectados*/
void atualiza_display(){ 
   while(1){
      pthread_mutex_lock( &exclusao_mutua); /*Trava a seçao critica*/

      /*Seçao critica - varia¡veis globais que armazenan todo o valor de detecções nas tres esteiras
      o printf() simula a atualizaçao do display */
      printf("\nItens Total: %lf", sensor_lido);  
      if(pesagem == 1){ //verifica se ocorreu uma pesagem para atualizar o display
         printf("\nPeso total: %lf", pesagem_total);
         itens_pesados = 0;
         pesagem = 0;
      }
      pthread_mutex_unlock( &exclusao_mutua); /*destrava a seçao critica*/
      
      /* tempo para ataulizar o display e que é percepta­vel ao olho humano */
      usleep(1000 * 1000);            
   }
}

/*funçao para somar os pesos detectados
   input: v - vetor com QTD_PESOS
          N - QTD_PESOS
   output: sum - peso total
*/
double sum(double *v, int N){
   int i;
   double sum = 0;

   /* 
   cria-se va¡rias threads para processar cada uma uma parte do vetor
   private - indica que cada thread tem o seu pra³prio "i"
   reduction(op : list) - indica que cada thread calcula o seu "sum" (list)
               e posterioremten é feita a operaçao com todos os "sum", 
               sendo a operaçao (op) uma soma +
   */
   #pragma omp parallel for private(i) reduction(+ : sum)
   for(i = 0; i < N; i++){
      sum += v[i];
   }

   return sum;
}

/* funçao para inicializar o mutex com o protocolo de herança de prioridade */
void init_mutex_inh(){
   int rtn;
   int mutex_protocol;

   /* Qual é o protocolo padrao no host? - retirado de exemplo*/
   if ((rtn = pthread_mutexattr_init(&mutexattr_prioinherit)) != 0)
      fprintf(stderr,"pthread_mutexattr_init: %s",strerror(rtn)); //mensagem em caso de haver erro
   if ((rtn = pthread_mutexattr_getprotocol(&mutexattr_prioinherit, &mutex_protocol)) != 0)
      fprintf(stderr,"pthread_mutexattr_getprotocol: %s",strerror(rtn)); //mensagem em caso de haver erro

#ifdef DEBUG 
   if (mutex_protocol == PTHREAD_PRIO_PROTECT)
      printf("Default mutex protocol is PTHREAD_PRIO_PROTECT\n");
   else if (mutex_protocol == PTHREAD_PRIO_INHERIT)
      printf("Default mutex protocol is PTHREAD_PRIO_INHERIT\n");
   else if (mutex_protocol == PTHREAD_PRIO_NONE)
      printf("Default mutex protocol is PTHREAD_PRIO_NONE\n");
   else 
      printf("Default mutex protocol is unrecognized: %d\n");
#endif

   /* Define o protocolo do mutex como INHERIT - herança de prioridade*/  
   if ((rtn = pthread_mutexattr_setprotocol(&mutexattr_prioinherit, PTHREAD_PRIO_INHERIT)) != 0)
    fprintf(stderr,"pthread_mutexattr_setprotocol: %s",strerror(rtn)); //mensagem em caso de haver erro

   /* Inicialize mutex com o objeto de atributo */
   if ((rtn = pthread_mutex_init(&exclusao_mutua,&mutexattr_prioinherit)) != 0)
      fprintf(stderr,"pthread_mutexattr_init: %s",strerror(rtn)); //mensagem em caso de haver erro

#ifdef DEBUG 
   /*verificaçao para identificar se o protocolo foi realmente atribua­do */
   if ((rtn = pthread_mutexattr_getprotocol(&mutexattr_prioinherit, &mutex_protocol)) != 0)
      fprintf(stderr,"pthread_mutexattr_getprotocol: %s",strerror(rtn)); //mensagem em caso de haver erro

   if (mutex_protocol == PTHREAD_PRIO_PROTECT)
      printf("Default mutex protocol is PTHREAD_PRIO_PROTECT\n");
   else if (mutex_protocol == PTHREAD_PRIO_INHERIT)
      printf("Default mutex protocol is PTHREAD_PRIO_INHERIT\n");
   else if (mutex_protocol == PTHREAD_PRIO_NONE)
      printf("Default mutex protocol is PTHREAD_PRIO_NONE\n");
   else 
      printf("Default mutex protocol is unrecognized: %d\n");
#endif
}



/*

1.a) Trocar a política de escalonamento do processo para os tipos existentes na função sched_setscheduler:

Use a função sched_setscheduler para definir a política de escalonamento do processo. 
Você pode fazer isso no início do main() antes de criar as threads.
Exemplo de como definir a política de escalonamento para FIFO:
--------------------------------------------------------------------
struct sched_param param;
param.sched_priority = 50; // Defina a prioridade desejada
int ret = sched_setscheduler(0, SCHED_FIFO, &param);
--------------------------------------------------------------------

1.b) Investigar a diferença/semelhança entre pthread_attr_setschedpolicy e pthread_attr_setschedparam:

 - pthread_attr_setschedpolicy é usado para definir a política de escalonamento de uma thread, 
como SCHED_FIFO ou SCHED_RR.
 - pthread_attr_setschedparam é usado para definir os parâmetros de escalonamento de uma thread, 
 como a prioridade.

1.c) Repita o exercício "a", mas agora para as threads criadas com a função investigada em "b":

Para cada thread usar pthread_attr_setschedpolicy e pthread_attr_setschedparam para definir 
a política de escalonamento e os parâmetros de escalonamento individualmente.

Exemplo de como configurar a política e os parâmetros de escalonamento para uma thread 
criada com pthread_create:
--------------------------------------------------------------------------------------
pthread_t thread;
pthread_attr_t attr;
struct sched_param param;

pthread_attr_init(&attr);
pthread_attr_setschedpolicy(&attr, SCHED_FIFO); // Defina a política de escalonamento desejada
param.sched_priority = 50; // Defina a prioridade desejada
pthread_attr_setschedparam(&attr, &param);

pthread_create(&thread, &attr, (void *)sua_funcao, NULL);
-------------------------------------------------------------------------------------------------

1.d) Modifique a afinidade do CPU para o processo:
Para definir a afinidade de CPU para o processo, usar a função sched_setaffinity. 
Esta função permite definir quais CPUs o processo pode ser executado.
Exemplo de como definir a afinidade para o CPU 0 e 1:
----------------------------------------------------------------------------------
cpu_set_t cpuset;
CPU_ZERO(&cpuset);
CPU_SET(0, &cpuset);
CPU_SET(1, &cpuset);

if (sched_setaffinity(0, sizeof(cpu_set_t), &cpuset) == -1) {
    perror("sched_setaffinity");
}
-----------------------------------------------------------------------------------

Certifique-se de que as políticas e prioridades que você define para suas threads 
estejam de acordo com os recursos disponíveis no sistema e com os requisitos do 
seu aplicativo. Além disso, tenha cuidado ao definir a afinidade do CPU, pois isso 
pode afetar o desempenho geral do sistema.

*/