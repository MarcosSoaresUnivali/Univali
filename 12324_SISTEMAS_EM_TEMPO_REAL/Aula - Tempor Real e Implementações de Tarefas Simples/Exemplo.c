#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
/* TO USE WITH LINUX IN PLACE OF INTRIN.H
UNCOMMENT TO USE
*/
//#include <immintrin.h>
//#include <ammintrin.h>

void func1(){
    int x, y, z;

    y = 3;
    z = 4;
    x = y+z;
}

int func2(){
    int x, y, z;

    y = 3;
    z = 4;
    x = y+z;

    return x;
}

char func3(char param){
    char x, y, z;
    if(param = 'a'){
        y = 3;
        z = 4;
        x = y+z;
    }else{
        y = 5;
        z = 5;
        x = y+z;
    }
    if(x == 10){
        return 'a';
    }else{
        return 'b';
    }
}


void func4(float param){
    printf("\n The parameter is: %.2f", param);
}

short func5(){
    int temp_param;
    printf("\nValue to temp_param will be 15: ");
    temp_param = 15;

    return ((short)temp_param);

}

// Função para realizar um laço de repetição
void loopExample(int start, int end) {
    for (int i = start; i <= end; i++) {
        // Faz algo aqui, pode ser um cálculo qualquer
        char param_1 = 'b';
        short param_2;
        int param_3;
        float param_4 = 50.0;
        
        uint64_t begin, end;

        func1();
        param_3 = func2();
        param_1 = func3(param_1);
        func4(param_4);
        param_2 = func5();

    }
}

/* clock() 
int main() {
    const int num_samples = 100;
    clock_t start_time, end_time;
    clock_t total_time = 0;
    clock_t best_time = __INT_MAX__;
    clock_t worst_time = 0;

    for (int i = 0; i < num_samples; i++) {
        start_time = clock();
        
        // Chamada para a função que você deseja medir
        loopExample(0, 100);

        end_time = clock();
        clock_t elapsed_time = end_time - start_time;

        total_time += elapsed_time;

        if (elapsed_time < best_time) {
            best_time = elapsed_time;
        }

        if (elapsed_time > worst_time) {
            worst_time = elapsed_time;
        }
    }

    double average_time = (double)total_time / num_samples / CLOCKS_PER_SEC;

    printf("Average time: %.6f seconds\n", average_time);
    printf("Best time: %lu clock ticks\n", best_time);
    printf("Worst time: %lu clock ticks\n", worst_time);

    return 0;
}
*/

/* time() */ 
int main() {
    const int num_samples = 100;
    time_t begin, end;
    double total_time = 0;
    double best_time = __DBL_MAX__;
    double worst_time = 0;

    for (int i = 0; i < num_samples; i++) {
        begin = time(NULL);
        
        // Chamada para a função que você deseja medir
        loopExample(0, 100);

        end = time(NULL);
        double elapsed_time = difftime(end, begin);

        total_time += elapsed_time;

        if (elapsed_time < best_time) {
            best_time = elapsed_time;
        }

        if (elapsed_time > worst_time) {
            worst_time = elapsed_time;
        }
    }

    double average_time = total_time / num_samples;

    printf("Average time: %.6f seconds\n", average_time);
    printf("Best time: %.6f seconds\n", best_time);
    printf("Worst time: %.6f seconds\n", worst_time);

    return 0;
}


/* clock_gettime()
int main() {
    const int num_samples = 100;
    struct timespec start, end;
    double total_time = 0;
    double best_time = __DBL_MAX__;
    double worst_time = 0;
    const double BILLION = 1000000000.0;
    #define CLOCK_REALTIME			0

    for (int i = 0; i < num_samples; i++) {
        clock_gettime(CLOCK_REALTIME, &start);
        
        // Chamada para a função que você deseja medir
        loopExample(0, 100);

        clock_gettime(CLOCK_REALTIME, &end);
        double elapsed_time = (end.tv_sec - start.tv_sec) +
                              (end.tv_nsec - start.tv_nsec) / BILLION;

        total_time += elapsed_time;

        if (elapsed_time < best_time) {
            best_time = elapsed_time;
        }

        if (elapsed_time > worst_time) {
            worst_time = elapsed_time;
        }
    }

    double average_time = total_time / num_samples;

    printf("Average time: %.6f seconds\n", average_time);
    printf("Best time: %.6f seconds\n", best_time);
    printf("Worst time: %.6f seconds\n", worst_time);

    return 0;
}

*/

/* intrin.h 
// Linux/GCC
uint64_t rdtsc(){
    unsigned int lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

int main() {
    const int num_samples = 100;
    uint64_t begin, end, total_cycles = 0;
    uint64_t best_cycles = UINT64_MAX;
    uint64_t worst_cycles = 0;

    for (int i = 0; i < num_samples; i++) {
        begin = rdtsc();

        // Chamada para a função que você deseja medir
        loopExample(0, 100);

        end = rdtsc();
        uint64_t cycles_spent = end - begin;

        total_cycles += cycles_spent;

        if (cycles_spent < best_cycles) {
            best_cycles = cycles_spent;
        }

        if (cycles_spent > worst_cycles) {
            worst_cycles = cycles_spent;
        }
    }

    double average_cycles = (double)total_cycles / num_samples;

    printf("Average cycles: %.0f\n", average_cycles);
    printf("Best cycles: %lu\n", best_cycles);
    printf("Worst cycles: %lu\n", worst_cycles);

    return 0;
}

*/