#include <stdio.h>

#include "sensor.h"

static double sensor_lido = 0;

double sensor_get(void){ 
   double aux;
   
   //pthread_mutex_lock( &exclusao_mutua);
   aux = sensor_lido;
   //pthread_mutex_unlock( &exclusao_mutua);
   
   return aux;
}