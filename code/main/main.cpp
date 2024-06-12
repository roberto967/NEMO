#include "main.h"

using namespace std;

// Pins do servo que abre a comporta de comida
#define SERVO_FG1_GPIO GPIO_NUM_26
#define SERVO_FG1_CHANNEL LEDC_CHANNEL_0

// Pins do sensor de proximidade
#define MAX_DISTANCE_CM 500 // 5m max
#define ECHO_GPIO GPIO_NUM_5
#define TRIGGER_GPIO GPIO_NUM_18

// Pins relacionados ao sensor de peso
#define AVG_SAMPLES 10        // Número de amostras para média
#define GPIO_DATA GPIO_NUM_4  // Data
#define GPIO_SCLK GPIO_NUM_23 // Serial Clock

servo_config_t servo_foodgate_1;

static const char *TAG_HX711 = "HX711_TEST";

static void weight_reading_task(void *arg) {
  HX711_init(GPIO_DATA, GPIO_SCLK, eGAIN_128);
  HX711_tare(); // Zera a balança

  float weight = 0;

  while (1) {
    weight = HX711_get_units(AVG_SAMPLES); // Lê o peso em gramas
    ESP_LOGI(TAG_HX711, "Peso: %f kg", weight);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup_servo(void) {
  servo_foodgate_1 = {
      .max_angle = 180,     // Ângulo máximo do servo
      .min_width_us = 500,  // Largura de pulso mínima para o ângulo mínimo
      .max_width_us = 2500, // Largura de pulso máxima para o ângulo máximo
      .freq = 50,           // Frequência do sinal PWM
      .timer_number = LEDC_TIMER_0, // Número do temporizador LEDC
      .channels =
          {
              .servo_pin =
                  {
                      SERVO_FG1_GPIO,
                  },
              .ch =
                  {
                      SERVO_FG1_CHANNEL,
                  },
          },
      .channel_number = 1, // Configurando apenas um canal
  };

  // Inicialização do servo da comporta de comida
  ESP_ERROR_CHECK(iot_servo_init(LEDC_HIGH_SPEED_MODE, &servo_foodgate_1));
}

// Função para abrir e fechar comporta de comida
void oc_foodgate(void) {
  // Abre a comporta
  ESP_ERROR_CHECK(
      iot_servo_write_angle(LEDC_HIGH_SPEED_MODE, SERVO_FG1_CHANNEL, 180.0f));
  ESP_LOGI("FOOD_GATE", "Food gate opened");

  // Espera o tempo que a comida cai pra poder fechar, precisa
  // sincronizar com o sensor de peso

  // Fecha a comporta
  ESP_ERROR_CHECK(
      iot_servo_write_angle(LEDC_HIGH_SPEED_MODE, SERVO_FG1_CHANNEL, 0.0f));
  ESP_LOGI("FOOD_GATE", "Food gate closed");
}

extern "C" void app_main(void) {
  // Inicializar o servo da comporta de comida
  setup_servo();
}
