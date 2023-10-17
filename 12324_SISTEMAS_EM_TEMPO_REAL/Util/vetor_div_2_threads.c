#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

int vetor[100];

void* parte_um_vetor(void* arg) {
    int i;
    for (i = 0; i < 50; i++) {
        printf("[%s]: vetor[%2d] = %3d\n", (char*)arg, i, vetor[i]);
    }
    printf("%s terminou sua execucao\n", (char*)arg);
    pthread_exit(arg);
}

void* parte_dois_vetor(void* arg) {
    int i;
    for (i = 50; i < 100; i++) {
        printf("[%s]: vetor[%2d] = %3d\n", (char*)arg, i, vetor[i]);
    }
    printf("%s terminou sua execucao\n", (char*)arg);
    pthread_exit(arg);
}

int main() {

    pthread_t threads[2];
    int i;

    for (i = 0; i < 100; i++) {
        vetor[i] = i; // preenche o vetor com números de 0 a 99
    }

    pthread_create(&threads[0], NULL, parte_um_vetor, (void*)("Thread_1")); // cria a thread 1

    printf("Thread_1 iniciada...\n");

    pthread_join(threads[0], NULL); // espera a thread 1 finalizar a sua execução

    sleep(3);

    printf("Thread_2 iniciada...\n");

    pthread_create(&threads[1], NULL, parte_dois_vetor, (void*)("Thread_2")); // cria a thread 2

    pthread_join(threads[1], NULL); // espera a thread 2 finalizar a sua execução

    return 0;
}