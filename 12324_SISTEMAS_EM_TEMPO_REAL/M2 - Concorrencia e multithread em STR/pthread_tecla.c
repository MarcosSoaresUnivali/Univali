/*	
*	Ilustra a criacao de threads e uso de mutex
*	
*	gcc -o pthread_tecla pthread_tecla.c -lpthread
*
*/
#include	<pthread.h>
#include    <stdio.h>
#include    <stdlib.h>
#include	<unistd.h>
#include    <stdbool.h>

pthread_mutex_t em = PTHREAD_MUTEX_INITIALIZER;
int soma = 0;
int numero = 0;
char tecla_pressionada[] = "X";
bool novo_numero = false;

void tecla_tecla_pressionada(void) {
	while(1){
    printf("\nThread tecla pressioanda");
		pthread_mutex_lock(&em);
		tecla_pressionada[0] = getchar();
		if(tecla_pressionada[0] >= 48 && tecla_pressionada[0] <= 57){
			numero = atoi(tecla_pressionada);
			novo_numero = true;
		}
		pthread_mutex_unlock(&em);
	}
}


void thread_soma_numero(void) {
	while(1){
        printf("\nThread soma");
		pthread_mutex_lock(&em);
		if(novo_numero){
			soma+=numero;
			novo_numero = false;
		}
		pthread_mutex_unlock(&em);
	}
}

void thread_display(void) {
	while(1){
        printf("\nThread display");
		pthread_mutex_lock(&em);
		printf("\nSoma: %d\nTecla pressionada: %c", soma, tecla_pressionada[0]);
        pthread_mutex_unlock(&em);
		sleep(1);
	}
}


int main( int argc, char *argv[]) {

  pthread_t t1, t2, t3;
  printf("\nCriando Threads");
  pthread_create(&t1, NULL, (void *) tecla_tecla_pressionada, NULL);
  pthread_create(&t2, NULL, (void *) thread_soma_numero, NULL);
  pthread_create(&t3, NULL, (void *) thread_display, NULL);
    
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
}