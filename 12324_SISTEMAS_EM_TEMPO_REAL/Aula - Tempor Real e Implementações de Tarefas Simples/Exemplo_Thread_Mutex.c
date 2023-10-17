/*
*	Ilustra a criacao de threads e uso de mutex
*	Compilar com:	gcc -lpthread -o pthreads1 pthreads1.c
*	ou
*			gcc -o pthreads1 pthreads1.c -lpthread
*
*/

#include	<pthread.h>
#include	<stdio.h>
#include	<unistd.h>
//#include  <conio.h>
#include <ctype.h>

int vidro = 0;
pthread_mutex_t em = PTHREAD_MUTEX_INITIALIZER;

/***
*	Codigo de thread leitura de teclado
***/
void thr_le_teclado(void) {
    char tecla;
    while (1) {
        if (kbhit()) {
            // Stores the pressed key in ch
            tecla = getch();
            // Terminates the loop
            // when escape is pressed
            if ((int)tecla == 118){ //v minusculo
                pthread_mutex_lock(&em);
                vidro = 1;
                pthread_mutex_unlock(&em);
            }
        }
    }
}
/***
*	Codigo de thread que verifica se a tecla lida equivale a do vidro 
***/
void thr_vidro(void) {
    while(1){
        pthread_mutex_lock(&em);
        if(vidro == 1){
            printf("Vidro acionado\n");
            vidro = 0;
        }
        pthread_mutex_unlock(&em);
    }
}

int main( int argc, char *argv[]) {
		
		//criação da estrutura de dados referente as threads usadas
    pthread_t t_le_teclado, t_vidro;
		//criação das threads
    pthread_create(&t_le_teclado, NULL, (void *) thr_le_teclado, NULL);
    pthread_create(&t_vidro, NULL, (void *) thr_vidro, NULL);
    
		//espera as threads para finalizar o programa
		pthread_join(t_le_teclado, NULL);
    pthread_join(t_vidro, NULL);

    return 0;
}