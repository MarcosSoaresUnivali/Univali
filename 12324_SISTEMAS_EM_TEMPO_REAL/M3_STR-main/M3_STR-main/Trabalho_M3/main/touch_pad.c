#include "touch_pad.h"

#define WELL_TOUCH_PAD_ID      0 //
#define PIPE_400_TOUCH_PAD_ID  4 // ids internos dos touch pads para cada sensor
#define PIPE_800_TOUCH_PAD_ID  7 //
#define PIPE_1200_TOUCH_PAD_ID 9 //
#define TOUCH_PAD_NO_USE 0              //
#define TOUCH_PAD_THRESHOLD 400         // variaveis de inicializacao dos sensores
#define TOUCH_PAD_FILTER_PERIOD 10      //

void touch_intr_handler(void* arg); // prototipo do tratador de interrupcao

int pipe_sensor_status[4] = {}; // vetor que guarda status dos sensores em uso quando ocorre interrupcao

void touch_pad_init_all() { // funcao para inicializacao dos 4 touchpads usados para simular os sensores (apenas duto 0)
    touch_pad_init();                                                              //
    touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V); // inicializacao padrao geral
    touch_pad_set_fsm_mode(TOUCH_FSM_MODE_TIMER);                                  //

    touch_pad_config(WELL_TOUCH_PAD_ID, TOUCH_PAD_NO_USE);             //
    touch_pad_config(PIPE_400_TOUCH_PAD_ID, TOUCH_PAD_NO_USE);         //
    touch_pad_config(PIPE_800_TOUCH_PAD_ID, TOUCH_PAD_NO_USE);         // configuracao de cada pad usado
    touch_pad_config(PIPE_1200_TOUCH_PAD_ID, TOUCH_PAD_NO_USE);        //
                                                                       //
    touch_pad_set_thresh(WELL_TOUCH_PAD_ID, TOUCH_PAD_THRESHOLD);      //
    touch_pad_set_thresh(PIPE_400_TOUCH_PAD_ID, TOUCH_PAD_THRESHOLD);  //
    touch_pad_set_thresh(PIPE_800_TOUCH_PAD_ID, TOUCH_PAD_THRESHOLD);  //
    touch_pad_set_thresh(PIPE_1200_TOUCH_PAD_ID, TOUCH_PAD_THRESHOLD); //

    touch_pad_set_trigger_mode(TOUCH_TRIGGER_BELOW);   //
    touch_pad_intr_enable();                           // inicializacao e configuracao das interrupcoes
    touch_pad_isr_register(&touch_intr_handler, NULL); //

    touch_pad_filter_start(TOUCH_PAD_FILTER_PERIOD); // configuracao do filtro do touchpad
}

void touch_intr_handler(void* arg) { // tratador de interrupcao do touchpad (nao visivel no .h)
    uint16_t touch_value;
    uint32_t pad_intr = touch_pad_get_status(); // adquire status de todos os touch pads

    touch_pad_read_filtered(WELL_TOUCH_PAD_ID, &touch_value);                                     // le valor
    pipe_sensor_status[0] = (touch_value/100 + 1)*( (pad_intr >> WELL_TOUCH_PAD_ID)      & 0x01); // verifica se ativo. caso sim, da valor de 1 a 3 baseado no valor lido

    touch_pad_read_filtered(PIPE_400_TOUCH_PAD_ID, &touch_value);                                 //
    pipe_sensor_status[1] = (touch_value/100 + 1)*( (pad_intr >> PIPE_400_TOUCH_PAD_ID)  & 0x01); //
                                                                                                  // repete para todos os sensores
    touch_pad_read_filtered(PIPE_800_TOUCH_PAD_ID, &touch_value);                                 //
    pipe_sensor_status[2] = (touch_value/100 + 1)*( (pad_intr >> PIPE_800_TOUCH_PAD_ID)  & 0x01); //
                                                                                                  //
    touch_pad_read_filtered(PIPE_1200_TOUCH_PAD_ID, &touch_value);                                //
    pipe_sensor_status[3] = (touch_value/100 + 1)*( (pad_intr >> PIPE_1200_TOUCH_PAD_ID) & 0x01); //

    touch_pad_clear_status(); // reinicia status dos pads no esp
}

/*void touch_intr_handler(void* arg) {
    uint32_t pad_intr = touch_pad_get_status();

    pipe_sensor_status[0] = (pad_intr >> WELL_TOUCH_PAD_ID)      & 0x01; // versao simplificada do tratador de interrupcao
    pipe_sensor_status[1] = (pad_intr >> PIPE_400_TOUCH_PAD_ID)  & 0x01; // apenas indica se pad esta ativo ou nao
    pipe_sensor_status[2] = (pad_intr >> PIPE_800_TOUCH_PAD_ID)  & 0x01; //
    pipe_sensor_status[3] = (pad_intr >> PIPE_1200_TOUCH_PAD_ID) & 0x01; //

    touch_pad_clear_status();
}*/

int get_sensor_status(unsigned id) { // retorna status do sensor com id requisitado
    return pipe_sensor_status[id];
}

void reset_sensor_status(unsigned id) { // reinicia status do sensor com id requisitado
    pipe_sensor_status[id] = 0;
}
