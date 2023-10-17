#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

// Função para copiar um vetor por cópia direta
void copyArrayDirect(int source[], int dest[], int size) {
    for (int i = 0; i < size; i++) {
        dest[i] = source[i];
    }
}

// Função para copiar um vetor por referência via ponteiro
void copyArrayByReference(int *source, int *dest, int size) {
    for (int i = 0; i < size; i++) {
        dest[i] = source[i];
    }
}

// Função para realizar um laço de repetição
void loopExample(int start, int end) {
    for (int i = start; i <= end; i++) {
        // Faz algo aqui, pode ser um cálculo qualquer
    }
}

int main() {
    const int size = 1000000; // Tamanho do vetor
    int source[size]; // Vetor de origem

    // Preencher o vetor com valores de exemplo
    for (int i = 0; i < size; i++) {
        source[i] = rand() % 100;
    }

    int dest[size]; // Vetor de destino para cópia
    clock_t start_time, end_time;

    // Medir tempo para cópia direta
    start_time = clock();
    copyArrayDirect(source, dest, size);
    end_time = clock();
    printf("Tempo para cópia direta: %f segundos\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);

    // Medir tempo para cópia por referência via ponteiro
    start_time = clock();
    copyArrayByReference(source, dest, size);
    end_time = clock();
    printf("Tempo para cópia por referência: %f segundos\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);

    // Medir tempo para loop de 0 a 100
    start_time = clock();
    loopExample(0, 100);
    end_time = clock();
    printf("Tempo para loop de 0 a 100: %f segundos\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);

    // Medir tempo para loop de 100 a 0
    start_time = clock();
    loopExample(100, 0);
    end_time = clock();
    printf("Tempo para loop de 100 a 0: %f segundos\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);

    return 0;
}
