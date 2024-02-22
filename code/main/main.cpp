#include "main.h"

using namespace std;

// configurando pra usar apenas um processador
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

const gpio_num_t SERVO_PIN = GPIO_NUM_5;

extern "C" void app_main(void)
{
    servo_config_t servo_cfg = {
        .max_angle = 180,             // Ângulo máximo do servo
        .min_width_us = 500,          // Largura de pulso mínima para o ângulo mínimo
        .max_width_us = 2500,         // Largura de pulso máxima para o ângulo máximo
        .freq = 50,                   // Frequência do sinal PWM
        .timer_number = LEDC_TIMER_0, // Número do temporizador LEDC
        .channels = {
            .servo_pin = {
                [0] = SERVO_PIN, // Configurando apenas o primeiro canal
            },
            .ch = {
                [0] = LEDC_CHANNEL_0, // Usando apenas o primeiro canal LEDC
            },
        },
        .channel_number = 1, // Configurando apenas um canal
    };

    // Inicialização do servo
    iot_servo_init(LEDC_HIGH_SPEED_MODE, &servo_cfg);

    iot_servo_write_angle(LEDC_HIGH_SPEED_MODE, 0, 0.0f);

    // Loop para mover o servo
    for (float angle = 0.0f; angle <= 180.0f; angle += 5.0f)
    {
        iot_servo_write_angle(LEDC_HIGH_SPEED_MODE, 0, angle);
        vTaskDelay(portTICK_PERIOD_MS);
    }

    // Desinicializar o servo
    iot_servo_deinit(LEDC_HIGH_SPEED_MODE);
}
