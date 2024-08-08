#include "main.h"

#include "esp_log.h"
#include "esp_mac.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rom/gpio.h"
#include <esp_netif.h>
#include <esp_wifi.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include "wifi_manager.h"

using namespace std;

/* @brief tag used for ESP serial console messages */
static const char TAG[] = "main";

/**
 * @brief RTOS task that periodically prints the heap memory available.
 * @note Pure debug information, should not be ever started on production code!
 * This is an example on how you can integrate your code with wifi-manager
 */
void monitoring_task(void *pvParameter) {
  for (;;) {
    ESP_LOGI(TAG, "free heap: %" PRIu32, esp_get_free_heap_size());
    vTaskDelay(pdMS_TO_TICKS(10000));
  }
}

class DisconectBtn {
private:
  gpio_num_t BTN_DISC = GPIO_NUM_0;
  const char *TAG = "DisconectBtn";
  xQueueHandle btn_evt_queue;

  void setup() {
    // Instala o serviço de interrupção
    gpio_install_isr_service(0);
    // Adiciona a interrupção ao pino
    gpio_isr_handler_add(this->BTN_DISC, gpio_isr_handler, this);
    // Configura pino para interrupção
    gpio_pad_select_gpio(this->BTN_DISC);
    // Configura o pino do Botão como Entrada
    gpio_set_direction(this->BTN_DISC, GPIO_MODE_INPUT);
    // Configura o resistor de Pulldown para o botão (por padrão a entrada
    // estará em Zero)
    gpio_pulldown_en(this->BTN_DISC);
    // Desabilita o resistor de Pull-up por segurança.
    gpio_pullup_dis(this->BTN_DISC);
    // Configura pino para interrupção
    gpio_set_intr_type(this->BTN_DISC, GPIO_INTR_NEGEDGE);
    // Cria a fila de eventos
    btn_evt_queue = xQueueCreate(2, sizeof(int));
  }

public:
  DisconectBtn(gpio_num_t BTN_DISC) {
    this->BTN_DISC = BTN_DISC;
    setup();
  }

  void setButtonPin(gpio_num_t pin) { BTN_DISC = pin; }
  gpio_num_t getButtonPin() { return BTN_DISC; }

  void disconec_interrup(void *pvParameter) {
    // De-bouncing
    int estado = gpio_get_level(this->BTN_DISC);
    if (estado == 1) {
      gpio_isr_handler_remove(this->BTN_DISC);
      while (gpio_get_level(this->BTN_DISC) == estado) {
        vTaskDelay(50 / portTICK_PERIOD_MS);
      }

      // Desconecta e esquece a rede
      wifi_manager_disconnect_async();

      // Iniciar o AP para nova seleção de rede
      ESP_LOGI(
          TAG,
          "斜め七十七度の並びで泣く泣く嘶くナナハン七台難なく並べて長眺め...");
      wifi_manager_send_message(WM_ORDER_START_AP, NULL);

      // Habilitar novamente a interrupção
      vTaskDelay(50 / portTICK_PERIOD_MS);
      gpio_isr_handler_add(this->BTN_DISC, gpio_isr_handler, this);
    }
  }

  void start() {
    xTaskCreate(&button_task, "button_task", 2048, this, 10, NULL);
  }

  static void button_task(void *pvParameter) {
    DisconectBtn *btn = static_cast<DisconectBtn *>(pvParameter);
    int pino;
    for (;;) {
      if (xQueueReceive(btn->btn_evt_queue, &pino, portMAX_DELAY)) {
        btn->disconec_interrup(NULL);
      }
    }
  }

  static void IRAM_ATTR gpio_isr_handler(void *args) {
    DisconectBtn *btn = static_cast<DisconectBtn *>(args);
    int pino = btn->BTN_DISC;
    xQueueSendFromISR(btn->btn_evt_queue, &pino, NULL);
  }
};

/**
 * @brief this is an example of a callback that you can set up in your own app
 * to get notified of wifi manager event.
 */
void cb_connection_ok(void *pvParameter) {
  ip_event_got_ip_t *param = (ip_event_got_ip_t *)pvParameter;

  /* transform IP to human readable string */
  char str_ip[16];
  esp_ip4addr_ntoa(&param->ip_info.ip, str_ip, IP4ADDR_STRLEN_MAX);

  ESP_LOGI(TAG, "I have a connection and my IP is %s!", str_ip);

  uint8_t baseMac[6];
  esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
  if (ret == ESP_OK) {
    ESP_LOGI(
        TAG,
        "I have a connection and my MAC is: %02x:%02x:%02x:%02x:%02x:%02x!\n",
        baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
  } else {
    ESP_LOGE(TAG, "Failed to read MAC address");
  }
}

extern "C" void app_main(void) {
  /* start the wifi manager */
  wifi_manager_start();

  /* register a callback as an example to how you can integrate your code with
   * the wifi manager */
  wifi_manager_set_callback(WM_EVENT_STA_GOT_IP, &cb_connection_ok);

  /* create and start the disconnect button */
  DisconectBtn *disconnect_btn = new DisconectBtn(GPIO_NUM_5);
  disconnect_btn->start();

  /* your code should go here. Here we simply create a task on core 2 that
   * monitors free heap memory */
  // xTaskCreatePinnedToCore(&monitoring_task, "monitoring_task", 2048, NULL, 1,
  //                         NULL, 1);
}
