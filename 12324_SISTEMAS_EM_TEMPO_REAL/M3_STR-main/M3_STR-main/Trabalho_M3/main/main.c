#include <stdio.h>
#include <time.h>
#include "esp_log.h"

#include "touch_pad.h"

#define DEBUG // define permite coleta dos tempos de execucao

#ifdef DEBUG
    #define DEBUG_CURRENT_PIPE 0          // seleciona sensor para analise
    #define DEBUG_CURRENT_SENSOR PIPE_400 //

    /*
     * A constante DEBUG_MODE define o tipo de analise de tempo realizada durante a execucao.
     *  - Quando DEBUG_MODE vale 0, todos os sensores estarao sob analise, e o programa
     *    apenas indicara quando alguma task perder a deadline definida em DEBUG_EXPECTED_DELAY.
     *  - Quando DEBUG_MODE vale 1, apenas o sensor selecionado para analise tera o tempo de
     *    execucao indicado, e o programa ira imprimir o tempo de execucao de cada ciclo.
     */
    #define DEBUG_MODE 0

    #define DEBUG_EXPECTED_DELAY 200 // deadline das tasks do sensor
#endif

#define SENSOR_STATUS_DELAY_MS 190 // constantes de delays
#define DISPLAY_DELAY_MS 5000      //

#define TIMESPEC_TO_MS(ts) ( (uint64_t) ts.tv_sec*1000 + ts.tv_nsec/1000000) // macro para obter tempo em milisegundos das medicoes


const char* TAG = "PIPE_SENSORS"; // tag para log do esp

int sensors[3][4] = {}; // matriz com status de todos os sensores

void display_task() { // task para display
    const char* nome[4] = {"Poço", "400m", "800m", "1200m"};

    while(1) {
        printf("\n");

        for (int i = 0; i < 3; i++){ 
            printf("Poço %d:\n", i + 1);

            for (int j = 0; j < 4; j++){
                printf("    Sensor %s:\t", nome[j]);

                if(!j)
                    printf("Estado: %s\n\n", (sensors[i][j] ? "Mal funcionamento" : "Estável") );
                else {
                    printf("Óleo: %s\n\t\t\t", (sensors[i][j]%2 ? "Mal funcionamento" : "Estável") );
                    printf("Gás: %s\n", (sensors[i][j]/2 ? "Mal funcionamento" : "Estável") );
                }
            }

            printf("\n");
        }

        vTaskDelay(DISPLAY_DELAY_MS / portTICK_PERIOD_MS); // dorme pelo tempo de delay definido
    }
}

int check_sensor_status(int pipe_id, int sensor_id) { // funcao para verificar status do sensor
    int result;

    if(!pipe_id) // apenas duto 0 sendo simulado pelo esp
        result = get_sensor_status(sensor_id); // adquire status atual do sensor
    else // para outros dutos, define sensor como estavel
        result = 0;

    return result; // retorna resultado
}

void apply_countermeasure(int pipe_id, int sensor_id) { // funcao que aplica contramedida para sensores instaveis
    const int sensor_name = sensor_id*400;
    char* pipe_name = (char *) malloc(100 * sizeof(char) );

    sprintf(pipe_name, "DUTO_%d", pipe_id);

    if(!sensor_id)                                                                     //
        ESP_LOGE(TAG, "Sensor do poço instável! (%s)", pipe_name);                     // mensagens diferentes para poco/duto
    else                                                                               //
        ESP_LOGE(TAG, "Sensor do duto em %dm instável! (%s)", sensor_name, pipe_name); //

    ESP_LOGI(TAG, "Aplicando contramedida..."); // aplica delay para contramedida
    vTaskDelay(2 / portTICK_PERIOD_MS);         //

    reset_sensor_status(sensor_id); // reinicia status do sensor
}

void sensor_monitoring_task(void* arg) { // task para monitorar os sensores
    int pipe_id = *( (int *) arg)/10;
    int sensor_id = *( (int *) arg)%10;
    TickType_t last_wake_up_tick;

    #ifdef DEBUG
        struct timespec start, finish; // variaveis de debug
        uint64_t exec;                 //

        if(pipe_id == DEBUG_CURRENT_PIPE && sensor_id == DEBUG_CURRENT_SENSOR)
            ESP_LOGW(TAG, "Task atualmente sobre DEBUG: duto=%d; sensor=%d", pipe_id, sensor_id);
    #endif

    while(1) {
        #ifdef DEBUG
            clock_gettime(CLOCK_REALTIME, &start); // captura tempo de inicio
        #endif

        last_wake_up_tick = xTaskGetTickCount(); // tick inicial da task

        if(sensor_id) // aplica delay que simula comunicacao com o sensor
            vTaskDelay( (20*sensor_id + 1) / portTICK_PERIOD_MS);
        else
            vTaskDelay( (75 + 1) / portTICK_PERIOD_MS);

        sensors[pipe_id][sensor_id] = check_sensor_status(pipe_id, sensor_id); // capta status do sensor

        if(sensors[pipe_id][sensor_id]) // se instavel
            apply_countermeasure(pipe_id, sensor_id); // aplica contramedida
        
        vTaskDelayUntil(&last_wake_up_tick, SENSOR_STATUS_DELAY_MS / portTICK_PERIOD_MS); // espera ate fim do periodo

        #ifdef DEBUG
            clock_gettime(CLOCK_REALTIME, &finish); // capta tempo de fim

            exec = TIMESPEC_TO_MS(finish) - TIMESPEC_TO_MS(start); // calcula tempo de execucao

            #if DEBUG_MODE // para DEBUG_MODE = 1
                if(pipe_id == DEBUG_CURRENT_PIPE && sensor_id == DEBUG_CURRENT_SENSOR)
                    ESP_LOGW(TAG, "Tempo de execução do sensor %d no duto %d: %llu", sensor_id, pipe_id, exec); // imprime tempo de execucao na tela
            #else          // para DEBUG_MODE = 0
                if(exec > DEBUG_EXPECTED_DELAY) { // se perder deadline
                    ESP_LOGW(TAG, "Tempo de execução excedeu o limite de %d!", DEBUG_EXPECTED_DELAY);           // indica perda na tela
                    ESP_LOGW(TAG, "Tempo de execução do sensor %d no duto %d: %llu", sensor_id, pipe_id, exec); //
                }
            #endif
        #endif
    }
}

void app_main(void)
{
    int args[3][4]; // vetor com id das tasks dos sensores

    touch_pad_init_all(); // inicializa touchpads

    for(int pipe_id = 0; pipe_id < 3; pipe_id++) {           // cria tasks para cada sensor
        for(int sensor_id = 0; sensor_id < 4; sensor_id++) { //
            char* task_name = (char *) malloc(100 * sizeof(char) );

            args[pipe_id][sensor_id] = 10*pipe_id + sensor_id; // gera id da task baseado no duto e sensor correspondente

            sprintf(task_name, "sensor_monitoring_task_%d", args[pipe_id][sensor_id]); // cria nome para task

            // inicia task do sensor
            xTaskCreatePinnedToCore(&sensor_monitoring_task, task_name, 2048, (void *) &args[pipe_id][sensor_id], (4 - sensor_id)*10, NULL, sensor_id%2);
        }
    }

    xTaskCreate(&display_task, "display_task", 2048, NULL, 5, NULL); // inicia task para display

    while(1) {
        vTaskDelay(10000 / portTICK_PERIOD_MS); // delay permanente, serve como task idle
    }
}
