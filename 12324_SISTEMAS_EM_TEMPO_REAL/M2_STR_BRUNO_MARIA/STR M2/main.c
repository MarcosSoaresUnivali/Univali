#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <x86intrin.h>
#include <time.h>
#include <inttypes.h>
#include "kbhit.h"

#pragma intrinsic(__rdtsc)

bool car_system_sensors[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
bool car_system_actuator[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//0: electronic injection | 1: internal temperature | 
//2: ABS right whell | 3: ABS left whell |
//4: airbag | 5: seat belt | 

//6: front headlight light right | 7: front headlight light left | 
//8: power window system right | 9: power window system left
//10: two door lock right | 11: two door lock left 

//TIME
struct timespec start, endEL, endIT, endABS, endAIR, endSB, endLVT, endFHL, endPWS, endTDL;
uint64_t begin, endCycleEL, cycles_spentEL, endCycleIT, cycles_spentIT;
uint64_t endCycleABS, cycles_spentABS, endCycleAIR, cycles_spentAIR;
uint64_t endCycleSB, cycles_spentSB, endCycleLVT, cycles_spentLVT;
uint64_t endCycleFHL, cycles_spentFHL, endCyclePWS, cycles_spentPWS;
uint64_t endCycleTDL, cycles_spentTDL;
double elapsedEL, elapsedIT, elapsedABS, elapsedAIR, elapsedSB, elapsedFHL, elapsedPWS, elapsedTDL, elapsedLVT;

pthread_mutex_t em = PTHREAD_MUTEX_INITIALIZER;

void read_sensor(int index){
      car_system_sensors[index] = !car_system_sensors[index];
      //printf("\nReading the sensor!"); 
}

void execute_actuator(int index){
    car_system_actuator[index] = !car_system_actuator[index];
    //printf("\nActive the actuator!");
}

double get_time_clock(struct timespec end, struct timespec start){
    clock_gettime(CLOCK_MONOTONIC, &end);
    return (end.tv_sec - start.tv_sec) + 1e-9 * (end.tv_nsec - start.tv_nsec);
}

uint64_t get_time_cycle(uint64_t begin, uint64_t endCycle){
    endCycle = __rdtsc();
    return (endCycle - begin);
}

void thr_read_key(void) {
    char key;
    while (1) {
        usleep(16); //16us 
        if (kbhit()) {
            // Stores the pressed key in ch
            key = getchar();
            // Terminates the loop
            // when escape is pressed
            if ((int)key == 122){ //z electronic injection
                pthread_mutex_lock(&em);
                begin = __rdtsc();
                clock_gettime(CLOCK_MONOTONIC, &start);
                car_system_sensors[0] = 1;
                pthread_mutex_unlock(&em);
            } else if ((int)key == 120){ //x internal temperature
                pthread_mutex_lock(&em);
                begin = __rdtsc();
                clock_gettime(CLOCK_MONOTONIC, &start);
                car_system_sensors[1] = 1;
                pthread_mutex_unlock(&em);
            } else if ((int)key == 99){ //c ABS
                pthread_mutex_lock(&em);
                begin = __rdtsc();
                clock_gettime(CLOCK_MONOTONIC, &start);
                car_system_sensors[2] = 1;
                car_system_sensors[3] = 1;
                pthread_mutex_unlock(&em);
            } else if ((int)key == 118){ //v airbag
                pthread_mutex_lock(&em);
                begin = __rdtsc();
                clock_gettime(CLOCK_MONOTONIC, &start);
                car_system_sensors[4] = 1;
                pthread_mutex_unlock(&em);
            } else if ((int)key == 98){ //b seat belt
                pthread_mutex_lock(&em);
                begin = __rdtsc();
                clock_gettime(CLOCK_MONOTONIC, &start);
                car_system_sensors[5] = 1;
                pthread_mutex_unlock(&em);
            } else if ((int)key == 110){ //n front headlight light
                pthread_mutex_lock(&em);
                begin = __rdtsc();
                clock_gettime(CLOCK_MONOTONIC, &start);
                car_system_sensors[6] = 1;
                car_system_sensors[7] = 1;
                pthread_mutex_unlock(&em);
            } else if ((int)key == 109){ //m power window system
                pthread_mutex_lock(&em);
                begin = __rdtsc();
                clock_gettime(CLOCK_MONOTONIC, &start);
                car_system_sensors[8] = 1;
                car_system_sensors[9] = 1;
                pthread_mutex_unlock(&em);
            } else if ((int)key == 44){ //, two door lock
                pthread_mutex_lock(&em);
                begin = __rdtsc();
                clock_gettime(CLOCK_MONOTONIC, &start);
                car_system_sensors[10] = 1;
                car_system_sensors[11] = 1;
                pthread_mutex_unlock(&em);
            } else if ((int)key == 46){ //. ative everything
                pthread_mutex_lock(&em);
                begin = __rdtsc();
                clock_gettime(CLOCK_MONOTONIC, &start);
                car_system_sensors[0] = 1;
                car_system_sensors[1] = 1;
                car_system_sensors[2] = 1;
                car_system_sensors[3] = 1;
                car_system_sensors[4] = 1;
                car_system_sensors[5] = 1;
                car_system_sensors[6] = 1;
                car_system_sensors[7] = 1;
                car_system_sensors[8] = 1;
                car_system_sensors[9] = 1;
                car_system_sensors[10] = 1;
                car_system_sensors[11] = 1;
                pthread_mutex_unlock(&em);
            }
        }
    }
}

void thr_electronic_injection(void) {
    while(1){
        pthread_mutex_lock(&em);
        if(car_system_sensors[0] == 1){
            read_sensor(0);
            execute_actuator(0);
            car_system_sensors[0] = 0;
            //TIME
            elapsedEL = get_time_clock(endEL, start);
            cycles_spentEL = get_time_cycle(begin, endCycleEL);
        }
        pthread_mutex_unlock(&em);
    }
}

void thr_internal_temp(void) {
    while(1){
        pthread_mutex_lock(&em);
        if(car_system_sensors[1] == 1){
            read_sensor(1);
            execute_actuator(1);
            car_system_sensors[1] = 0;
            //TIME
            elapsedIT = get_time_clock(endIT, start);
            cycles_spentIT = get_time_cycle(begin, endCycleEL);
        }
        pthread_mutex_unlock(&em);
    }
}

void thr_abs(void) {
    while(1){
        pthread_mutex_lock(&em);
        if(car_system_sensors[2] == 1 && car_system_sensors[3] == 1){
            read_sensor(2);
            read_sensor(3);
            execute_actuator(2);
            execute_actuator(3);
            car_system_sensors[2] = 0;
            car_system_sensors[3] = 0;
            //TIME
            elapsedABS = get_time_clock(endABS, start);
            cycles_spentABS = get_time_cycle(begin, endCycleABS);
        }
        pthread_mutex_unlock(&em);
    }
}

void thr_airbag(void) {
    while(1){
        pthread_mutex_lock(&em);
        if(car_system_sensors[4] == 1){
            read_sensor(4);
            execute_actuator(4);
            car_system_sensors[4] = 0;
            //TIME
            elapsedAIR = get_time_clock(endABS, start);
            cycles_spentAIR = get_time_cycle(begin, endCycleAIR);
        }
        pthread_mutex_unlock(&em);
    }
}

void thr_seat_belt(void) {
    while(1){
        pthread_mutex_lock(&em);
        if(car_system_sensors[5] == 1){
            read_sensor(5);
            execute_actuator(5);
            car_system_sensors[5] = 0;
            //TIME
            elapsedSB = get_time_clock(endSB, start);
            cycles_spentSB = get_time_cycle(begin, endCycleSB);
        }
        pthread_mutex_unlock(&em);
    }
}

void thr_front_headlight_light(void) {
    while(1){
        pthread_mutex_lock(&em);
        if(car_system_sensors[6] == 1 && car_system_sensors[7] == 1){
            read_sensor(6);
            read_sensor(7);
            execute_actuator(6);
            execute_actuator(7);
            car_system_sensors[6] = 0;
            car_system_sensors[7] = 0;
            //TIME
            elapsedFHL = get_time_clock(endFHL, start);
            cycles_spentFHL = get_time_cycle(begin, endCycleFHL);
        }
        pthread_mutex_unlock(&em);
    }
}

void thr_power_window_sys(void) {
    while(1){
        pthread_mutex_lock(&em);
        if(car_system_sensors[8] == 1 && car_system_sensors[9] == 1){
            read_sensor(8);
            read_sensor(9);
            execute_actuator(8);
            execute_actuator(9);
            car_system_sensors[8] = 0;
            car_system_sensors[9] = 0;
            //TIME
            elapsedPWS = get_time_clock(endFHL, start);
            cycles_spentPWS = get_time_cycle(begin, endCyclePWS);
        }
        pthread_mutex_unlock(&em);
    }
}

void thr_two_door_lock(void) {
    while(1){
        pthread_mutex_lock(&em);
        if(car_system_sensors[10] == 1 && car_system_sensors[11] == 1){
            read_sensor(10);
            read_sensor(11);
            execute_actuator(10);
            execute_actuator(11);
            car_system_sensors[10] = 0;
            car_system_sensors[11] = 0;
            //TIME
            elapsedTDL = get_time_clock(endTDL, start);
            cycles_spentTDL = get_time_cycle(begin, endCycleTDL);
        }
        pthread_mutex_unlock(&em);
    }
}

void thread_display(void) {
	while(1){
		pthread_mutex_lock(&em);
        system("clear");
        printf("\nENGINE: Actuator electronic injection: %d | Actuator internal temperature: %d\n", car_system_actuator[0], car_system_actuator[1]);
        printf("BRAKE: ABS right: %d | ABS left: %d\n", car_system_actuator[2], car_system_actuator[3]);
        printf("LSE: Airbag: %d | Seat belt: %d\n", car_system_actuator[4], car_system_actuator[5]);
        printf("LVT: FHL right: %d | FHL left: %d | PWS right: %d | PWS left: %d | TDL right: %d | TDL left: %d\n", car_system_actuator[6], car_system_actuator[7], car_system_actuator[8], car_system_actuator[9], car_system_actuator[10], car_system_actuator[11]);
        
        printf("\nDeadline electronic injection: Spent cycles: %" PRIu64 " | Elapsed time: %.6f seconds\n", cycles_spentEL, elapsedEL);
        printf("\nDeadline internal temperature: Spent cycles: %" PRIu64 " | Elapsed time: %.6f seconds\n", cycles_spentIT, elapsedIT);
        printf("\nDeadline ABS: Spent cycles: %" PRIu64 " | Elapsed time: %.6f seconds\n", cycles_spentABS, elapsedABS);
        printf("\nDeadline AIRBAG: Spent cycles: %" PRIu64 " | Elapsed time: %.6f seconds\n", cycles_spentAIR, elapsedAIR);
        printf("\nDeadline seat belt: Spent cycles: %" PRIu64 " | Elapsed time: %.6f seconds\n", cycles_spentSB, elapsedSB);
        cycles_spentLVT = cycles_spentPWS + cycles_spentSB + cycles_spentTDL;
        elapsedLVT = elapsedPWS + elapsedSB + elapsedTDL;
        printf("\nDeadline LVT: Spent cycles: %" PRIu64 " | Elapsed time: %.6f seconds\n", cycles_spentLVT, elapsedLVT);
        pthread_mutex_unlock(&em);
        usleep(500000); //500ms
	}
}

int main(int argc, char *argv[]) {
    //criação da estrutura de dados referente as threads usadas
    pthread_t t_electronic_injection, t_internal_temp, t_abs, t_airbag, t_seat_belt, t_front_headlight_light;
	pthread_t t_power_window_sys, t_two_door_lock, t_read_key, t_display;

    //criação das threads
    pthread_create(&t_read_key, NULL, (void *) thr_read_key, NULL);
    pthread_create(&t_electronic_injection, NULL, (void *) thr_electronic_injection, NULL);
    pthread_create(&t_internal_temp, NULL, (void *) thr_internal_temp, NULL);
    pthread_create(&t_abs, NULL, (void *) thr_abs, NULL);
    pthread_create(&t_airbag, NULL, (void *) thr_airbag, NULL);
    pthread_create(&t_seat_belt, NULL, (void *) thr_seat_belt, NULL);
    pthread_create(&t_front_headlight_light, NULL, (void *) thr_front_headlight_light, NULL);
    pthread_create(&t_power_window_sys, NULL, (void *) thr_power_window_sys, NULL);
    pthread_create(&t_two_door_lock, NULL, (void *) thr_two_door_lock, NULL);
    pthread_create(&t_display, NULL, (void *) thread_display, NULL);
    
	//espera as threads para finalizar o programa
	pthread_join(t_read_key, NULL);
    pthread_join(t_electronic_injection, NULL);
    pthread_join(t_internal_temp, NULL);
    pthread_join(t_abs, NULL);
    pthread_join(t_airbag, NULL);
    pthread_join(t_seat_belt, NULL);
    pthread_join(t_front_headlight_light, NULL);
    pthread_join(t_power_window_sys, NULL);
    pthread_join(t_two_door_lock, NULL);
    pthread_join(t_display, NULL);
    return 0;
}