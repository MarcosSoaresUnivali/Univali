/*	
---Problemática------------------------------------------------------------------------------------
Uma fabricante de automóveis quer testar um novo sistema de monitoramento do comportamento de 
várias áreas de um veículo no qual ela está projetando. O projeto é novo e permitirá controle 
e noção do que está acontecendo em todo o veículo. A empresa, em um primeiro momento, dividiu 
o carro em 4 subsistemas:

    (i) motor; 
        No motor, deve-se monitorar e controlar a injeção eletrônica e temperatura interna. 

    (ii) frenagem; 
        Na frenagem, deve-se monitorar e controlar o ABS nas duas rodas dianteiras.                           

    (iii) equipamentos de suporte a vida; e 
        Nos equipamentos de suporte a vida, deve-se monitorar e controlar o airbag e cinto 
        de segurança.

    (iv) LVT (luz, vidros e travas).
        Já no LVT, deve-se monitorar e controlar a luz dos faróis dianteiros, sistema de 
        vidros elétricos do motorista e passageiro (só parte da frente) e trava das duas 
        portas da frente.

Todos os sistemas se conectam a um computador de bordo central, que deve fornecer, reter e 
controlar ações destes sistemas. Cada sistema conta com um sensor e um controlador 
(totalizando 12 sensores e 12 controladores).
---------------------------------------------------------------------------------------------------

*	
*	gcc -o m2_cb_auto m2_cb_auto.c -lpthread
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>

#define NUM_SENSORS 12
#define NUM_CONTROLLERS 12
#define CLOCK_MONOTONIC 1

//Deadlines request ms + 5s para check...
//----------------------microsec---second-
#define DEAD_INJECAO    500     + 5000000
#define DEAD_TEMPMOTOR  20      + 5000000
#define DEAD_ABS        100     + 5000000
#define DEAD_AIRBAG     100     + 5000000
#define DEAD_CINTOSEG   1000000 + 4000000
#define DEAD_LVT        1000000 + 4000000

#define LIM_WAITDISPLAY 500     + 5000000
#define LIM_WAITSENSOR  1       + 5000000
#define LIM_WAITCONTRO  5       + 5000000

// Estrutura para representar o estado dos sensores
typedef struct {
    int value;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Sensor;

//Global instancia
Sensor sensors[NUM_SENSORS];

// Função para simular aquisição de dados do sensor
void *sensor_thread(void *arg) {
    Sensor *sensor = (Sensor *)arg;
    struct timespec start, end;
    
    while (1) {
        // Simulando aquisição do sensor
        clock_gettime(CLOCK_MONOTONIC, &start);
        
        // O sensor demora 1 us para fazer a aquisição
        usleep(LIM_WAITSENSOR);

        // Atualizando o valor do sensor
        pthread_mutex_lock(&sensor->mutex);
        sensor->value = rand() % 100; // Simulação de leitura de dados
        pthread_mutex_unlock(&sensor->mutex);

        // Notificar o controlador
        pthread_cond_signal(&sensor->cond);
        
        // Medindo o tempo gasto na aquisição
        clock_gettime(CLOCK_MONOTONIC, &end);
        printf("\nSensor thread: Sensor %p adquirido em %lu us", sensor, (end.tv_nsec - start.tv_nsec) / 1000);
    }
    return NULL;
}

// Função para simular a ação do controlador
void *controller_thread(void *arg) {
    Sensor *sensor = (Sensor *)arg;
    
    while (1) {
        pthread_mutex_lock(&sensor->mutex);
        
        // Aguardar até que o sensor tenha uma leitura disponível
        while (sensor->value == -1) {
            pthread_cond_wait(&sensor->cond, &sensor->mutex);
        }
        
        // Simulando ação do controlador (5 us para agir)
        usleep(LIM_WAITCONTRO);
        
        // Exibir o estado do sensor
        printf("\nController thread: Sensor %p - Valor: %d", sensor, sensor->value);
        
        // Reiniciar o valor do sensor
        sensor->value = -1;
        
        pthread_mutex_unlock(&sensor->mutex);
    }
    return NULL;
}

void show_header(){
    char *pts; /* pointer to time string */
    time_t now; /* current time */
    char *ctime();
    (void) time(&now);
    printf("\n______________________________________________________________________");
    printf("\n   Computer BOARD MPS    ( o )  (><)  ( @ )     %s", ctime(&now));
    printf("\n______________________________________________________________________");
}

void show_values (){

    // Exibir o estado dos sensores
    for (int i = 0; i < NUM_SENSORS; i++) {
        //printf("Sensor %d - Valor: %d\n", i, sensors[i].value);
        if (i <= 1) {
            // 1 - Exibir o estado do sensor
            printf("\nSistema (1-Motor): Sensor %d - Valor: %d", i, sensors[i].value);
            printf("\n----------------------------------------------------------------------");
        }
        if (i >= 2 && i <= 3) {
            // 2 - Exibir o estado do sensor
            printf("\nSistema (2-Frenagem): Sensor %d - Valor: %d", i, sensors[i].value);
            printf("\n----------------------------------------------------------------------");
        }
        if (i >= 4 && i <= 5) {
            // Exibir o estado do sensor
            printf("\nSistema (3-Sup_a_Vida): Sensor %d - Valor: %d", i, sensors[i].value);
            printf("\n----------------------------------------------------------------------");
        }
        if (i >= 6 && i <= 7) {
            // Exibir o estado do sensor
            printf("\nSistema (4-LVT): Sensor %d - Valor: %d", i, sensors[i].value);
            printf("\n----------------------------------------------------------------------");
        }
    }    
}

// Função para interromper a simulação e mostrar o display...
void customHandlerCrtlTSTP(int signum) {
    printf("\nCustom handler: Received signal Crtl-Z");
    // Perform custom actions here
    show_header();
    show_values();
    // Wait (30 s para mostrar)
    usleep(LIM_WAITDISPLAY * 6);
}

// Função para exibir o estado dos sensores a cada 500 ms
void *display_thread() {
    show_header();
    while (1) {
        show_values();
        usleep(LIM_WAITDISPLAY);  // Atualização a cada 500 ms
    }
    // Wait (30 s para mostrar)
    usleep(LIM_WAITDISPLAY * 6);
    return NULL;
}

int main() {

    // Set the customHandler function to handle SIGTSTP (Ctrl-Z)
    signal(SIGTSTP, customHandlerCrtlTSTP);
    
    srand(time(NULL));
    
    //Sensor sensors[NUM_SENSORS];
    pthread_t sensor_threads[NUM_SENSORS];
    pthread_t controller_threads[NUM_CONTROLLERS];
    pthread_t display_thread_id;
    
    // Inicializar sensores
    for (int i = 0; i < NUM_SENSORS; i++) {
        sensors[i].value = -1;
        pthread_mutex_init(&sensors[i].mutex, NULL);
        pthread_cond_init(&sensors[i].cond, NULL);
        
        pthread_create(&sensor_threads[i], NULL, sensor_thread, &sensors[i]);
    }

    // Inicializar controladores
    for (int i = 0; i < NUM_CONTROLLERS; i++) {
        pthread_create(&controller_threads[i], NULL, controller_thread, &sensors[i % NUM_SENSORS]);
    }
    
    // Inicializar thread de display
    pthread_create(&display_thread_id, NULL, display_thread, &sensors);

    // Esperar indefinidamente (você pode definir um mecanismo de saída)
    pthread_exit(NULL);
    
    return 0;
}
