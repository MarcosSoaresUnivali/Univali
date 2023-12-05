#ifndef TOUCH_PAD_H
#define TOUCH_PAD_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/touch_sensor.h"
#include "soc/sens_periph.h"

#define WELL      0 // ids visiveis de cada sensor
#define PIPE_400  1 //
#define PIPE_800  2 //
#define PIPE_1200 3 //

void touch_pad_init_all();             // prototipo das funcoes publicas dos sensores
int get_sensor_status(unsigned id);    //
void reset_sensor_status(unsigned id); //

#endif // TOUCH_PAD_H