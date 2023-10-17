/*
Arquivo: q2_mutex_priority_ceiling.c
Autor: Felipe Viel
FunÃ§Ã£o do arquivo: cria 4 threads com pthreads, 3 para controlar itens que detectados em 3 esteiras e 1 para exibiÃ§Ã£o em display
                   cria N threads com OpenMP para calcular o vetor com 1500 pesos obtidos nas 3 esteiras
                   exemplifica o uso do protocolo teto de prioridade com a pthreads, alÃ©m de mostrar como atribuir prioridade as threads
Criado em 4 de novenbro de 2020
Modificado em 4 de novenbro de 2020

compilar usando: gcc -o q2_mutex_priority_ceiling q2_mutex_priority_ceiling.c -lpthread -fopenmp -lrt
especificar o nÃºmero de threads no terminal: export OMP_NUM_THREADS=4
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

#ifndef _POSIX_THREAD_PRIO_PROTECT
#error "This system does not support priority inherit protection in mutex"
#endif

pthread_mutex_t exclusao_mutua = PTHREAD_MUTEX_INITIALIZER;
pthread_mutexattr_t  mutexattr_prioceiling;
int high_prio;

double sensor_lido = 0;
int itens_pesados = 0;
double v_pesos[QTD_PESOS];
double pesagem_total = 0;
int pesagem = 0;

//double limite_atual = HUGE_VAL;

void atualiza_sensor(void *arg);
void atualiza_display();
double sum(double *v, int N);
void init_mutex_inh();

int main(){
   pthread_t t1, t2, t3, t4;
   pthread_attr_t tattr;
   int newprio = 20;
   struct sched_param param;
   int thread_id = 1;
   
   /*chama a funÃ§Ã£o para inicializar o mutex com a heranÃ§a de prioridade*/
   init_mutex_inh();
   
   /* inicializado com atributos padrÃ£o */
   pthread_attr_init (&tattr);

   /* obter o parÃ¢metro de programaÃ§Ã£o existente */
   pthread_attr_getschedparam (&tattr, &param);

   /* definir a prioridade; outros parÃ¢metros nÃ£o mudaram */
   param.sched_priority = 20;

   /* definindo o novo parÃ¢metro de escalonamento */
   pthread_attr_setschedparam (&tattr, &param);
  
   /*criando a thread para a esteira 1*/
   pthread_create(&t1, &tattr, (void *) atualiza_sensor, (void *)thread_id); 
   thread_id++;
   /* definir a prioridade; outros parÃ¢metros nÃ£o mudaram */
   param.sched_priority = 20;

   /* definindo o novo parÃ¢metro de escalonamento */
   pthread_attr_setschedparam (&tattr, &param);

   /*criando a thread para a esteira 2*/
   pthread_create(&t2, &tattr, (void *) atualiza_sensor, (void *)thread_id);
   thread_id++;
   /* definir a prioridade; outros parÃ¢metros nÃ£o mudaram */
   param.sched_priority = 2;

   /* definindo o novo parÃ¢metro de escalonamento */
   pthread_attr_setschedparam (&tattr, &param);

   /*criando a thread para a esteira 3*/
   pthread_create(&t3, &tattr, (void *) atualiza_sensor, (void *)thread_id);
   
   /* definir a prioridade; outros parÃ¢metros nÃ£o mudaram */
   param.sched_priority = 22;

   /* definindo o novo parÃ¢metro de escalonamento */
   pthread_attr_setschedparam (&tattr, &param);

   /*criando a thread para atualizar o display*/
   pthread_create(&t4, &tattr, (void *) atualiza_display, NULL);

   /*thread principal espera as threas criadas retornarem para encerrar o processo*/
   pthread_join(t1, NULL);
   pthread_join(t2, NULL);
   pthread_join(t3, NULL);
   pthread_join(t4, NULL);

   return 0;
}

void atualiza_sensor(void *arg){
   double lido = 0;
   int rtn;
   //pthread_id_np_t   tid;
   double peso_esteira = 1.0;
   
   while(1){
      if(pesagem == 0){//verifica se irÃ¡ se estÃ¡ sendo feita uma pesagem
         
         /*verificando teto de prioridade 
           retorna EINVAL (Error: Invalid Value) se nÃ£o conseguir bloquear por causa do teto
           retorn 0 caso consiga*/
         if((rtn = pthread_mutex_lock( &exclusao_mutua)) != 0){ /*Trava a seÃ§Ã£o critica - pode nÃ£o conseguir por causa do teto de prioridade*/
            fprintf(stderr,"\npthread_mutex_lock: %s",strerror(rtn)); //mensagem em caso de haver erro
         }else{   
            /*SeÃ§Ã£o critica - variÃ¡veis globais que armazenam todo o valor de detecÃ§Ãµes nas trÃªs esteiras e os pesos
            a atribuiÃ§Ã£o de "lido" Ã© para simular o dado que Ã© lido dos sensores das esteiras
            o v_pesos armazena os pesos e a itens_pesados indica quantidade*/
            sensor_lido += lido;//lido; 

            if(QTD_PESOS > itens_pesados){ //verificaÃ§Ã£o chegou ou nÃ£o na quantidade a ser pesada
               v_pesos[itens_pesados] = peso_esteira;
               itens_pesados++;
            }else{
               pesagem = 1; //inicia o processo de pesagem
               pesagem_total = sum(v_pesos, QTD_PESOS);
            }
#ifdef DEBUG
            printf("\nEntrei %d", (int *)arg);
#endif
            pthread_mutex_unlock( &exclusao_mutua); /*destrava a seÃ§Ã£o critica*/
         }
      }

      /*Simula o intervalo de tempo que itens passÃ£o pelo sensor da esteira*/
      usleep(1000 * 1000); 
      
      /*Simula diferentes quantidades de itens passando ao mesmo tempo pelo sensor da esteira*/
      lido++;

      /*Simula diferentes pesos de itens na esteira*/
      //peso_esteira+=0.5; 
   }
}


/*funÃ§Ã£o para a thread realizar para exibi a quantidade e peso de itens detectados*/
void atualiza_display(){ 
   int rtn;
   /* //usado para ver o id da thread e verificar se bate com o teto de prioridade
   pthread_t         self;
   struct sched_param param;
   int priority;
   int policy;
   int ret;
   int prio_ceiling, old_priority;
   */
   
   while(1){
      /*verificando teto de prioridade 
        retorna EINVAL (Error: Invalid Value) se nÃ£o conseguir bloquear por causa do teto
        retorn 0 caso consiga*/
      
      /*******************forÃ§ando a barra, podemos verificar o teto 
      self = pthread_self();
      
      if(( rtn = pthread_getschedparam (self, &policy, &param)) != 0)
         fprintf(stderr,"\pthread_getschedparam: %s",strerror(rtn)); //mensagem em caso de haver erro
      
      priority = param.sched_priority; 

      if(( rtn = pthread_mutex_getprioceiling(&exclusao_mutua, &prio_ceiling)) != 0)
         fprintf(stderr,"\npthread_mutex_getprioceiling: %s",strerror(rtn)); //mensagem em caso de haver erro
      if(priority > prio_ceiling){
         if(( rtn = pthread_mutex_setprioceiling(&exclusao_mutua, priority, &old_priority)) != 0)
            fprintf(stderr,"\npthread_mutex_getprioceiling: %s",strerror(rtn));
      }
      *****************************/
      
      if((rtn = pthread_mutex_lock( &exclusao_mutua)) != 0){ /*Trava a seÃ§Ã£o critica - pode nÃ£o conseguir por causa do teto de prioridade*/
         fprintf(stderr,"\npthread_mutex_lock: %s",strerror(rtn)); //mensagem em caso de haver erro
      }else{ 

         /*SeÃ§Ã£o critica - variÃ¡veis globais que armazenan todo o valor de detecÃ§Ãµes nas trÃªs esteiras
         o printf() simula a atualizaÃ§Ã£o do display */
         printf("\nItens Total: %lf", sensor_lido);  
         if(pesagem == 1){ //verifica se ocorreu uma pesagem para atualizar o display
            printf("\nPeso total: %lf", pesagem_total);
            itens_pesados = 0;
            pesagem = 0;
         }
         pthread_mutex_unlock( &exclusao_mutua); /*destrava a seÃ§Ã£o critica*/
      }
      /* tempo para ataulizar o display e que Ã© perceptÃ­vel ao olho humano */
      usleep(1000 * 1000);            
   }
}

/*funÃ§Ã£o para somar os pesos detectados
   input: v - vetor com QTD_PESOS
          N - QTD_PESOS
   output: sum - peso total
*/
double sum(double *v, int N){
   int i;
   double sum = 0;

   /* 
   cria-se vÃ¡rias threads para processar cada uma uma parte do vetor
   private - indica que cada thread tem o seu prÃ³prio "i"
   reduction(op : list) - indica que cada thread calcula o seu "sum" (list)
               e posterioremten Ã© feita a operaÃ§Ã£o com todos os "sum", 
               sendo a operaÃ§Ã£o (op) uma soma +
   */
   #pragma omp parallel for shared(v) private(i) reduction(+ : sum) num_threads(4)
   for(i = 0; i < N; i++){
      sum += v[i];
   }

   return sum;
}

/* funÃ§Ã£o para inicializar o mutex com o protocolo de heranÃ§a de prioridade */
void init_mutex_inh(){
   int rtn;
   int mutex_protocol;

   /* Qual Ã© o protocolo padrÃ£o no host? - retirado de exemplo*/
   if ((rtn = pthread_mutexattr_init(&mutexattr_prioceiling)) != 0)
      fprintf(stderr,"pthread_mutexattr_init: %s",strerror(rtn)); //mensagem em caso de haver erro
   if ((rtn = pthread_mutexattr_getprotocol(&mutexattr_prioceiling, &mutex_protocol)) != 0)
      fprintf(stderr,"pthread_mutexattr_getprotocol: %s",strerror(rtn)); //mensagem em caso de haver erro
   
   /* pega a prioridade atual */
   high_prio = sched_get_priority_max(SCHED_FIFO);

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

   /* Define o protocolo do mutex como PROTECT - teto de prioridade*/  
   if ((rtn = pthread_mutexattr_setprotocol(&mutexattr_prioceiling, PTHREAD_PRIO_PROTECT)) != 0)
    fprintf(stderr,"pthread_mutexattr_setprotocol: %s",strerror(rtn)); //mensagem em caso de haver erro

   /* Define o o teto de prioridade inicial*/
   if ((rtn = pthread_mutexattr_setprioceiling(&mutexattr_prioceiling, high_prio)) != 0)
      fprintf(stderr,"pthread_mutexattr_setprotocol: %s",strerror(rtn)); //mensagem em caso de haver erro

   /* Inicialize mutex com o objeto de atributo */
   if ((rtn = pthread_mutex_init(&exclusao_mutua,&mutexattr_prioceiling)) != 0)
      fprintf(stderr,"pthread_mutexattr_init: %s",strerror(rtn)); //mensagem em caso de haver erro

#ifdef DEBUG 
   /*verificaÃ§Ã£o para identificar se o protocolo foi realmente atribuÃ­do */
   if ((rtn = pthread_mutexattr_getprotocol(&mutexattr_prioceiling, &mutex_protocol)) != 0)
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