#include "main.h"

using namespace std;

static const gpio_num_t SERVO_FG1_PIN = GPIO_NUM_5;
static const ledc_channel_t SERVO_FG1_CHANNEL = LEDC_CHANNEL_0;

servo_config_t servo_foodgate_1 = {
    .max_angle = 180,     // Ângulo máximo do servo
    .min_width_us = 500,  // Largura de pulso mínima para o ângulo mínimo
    .max_width_us = 2500, // Largura de pulso máxima para o ângulo máximo
    .freq = 50,           // Frequência do sinal PWM
    .timer_number = LEDC_TIMER_0, // Número do temporizador LEDC
    .channels =
        {
            .servo_pin =
                {
                    SERVO_FG1_PIN,
                },
            .ch =
                {
                    SERVO_FG1_CHANNEL,
                },
        },
    .channel_number = 1, // Configurando apenas um canal
};

// Função para abrir e fechar comporta de comida
void open_foodgate() {
  iot_servo_write_angle(LEDC_HIGH_SPEED_MODE, 0, 180.0f);

  // dai espera o tempo que a comida cai pra poder fechar, vai precisar
  // sincronizar com o sensor de peso
}

extern "C" void app_main(void) {

  // Inicialização do servo da comporta de comida
  iot_servo_init(LEDC_HIGH_SPEED_MODE, &servo_foodgate_1);

  // Desinicializar o servo da comporta de comida
  iot_servo_deinit(LEDC_HIGH_SPEED_MODE);
}
