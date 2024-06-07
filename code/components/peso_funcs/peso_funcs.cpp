#include "peso_funcs.h"

// A função para a leitura da tara basicamente faz várias leituras do sensor e
// calcula uma média, que será utilizada como o valor de tara.
int32_t tare(hx711_t *dev, uint8_t times) {
  int32_t tare_value = 0;
  int32_t data = 0;

  for (size_t i = 0; i < times; i++) {
    hx711_read_data(dev, &data);
    tare_value += data;
  }
  tare_value /= times;

  return tare_value;
}

float calibrate(hx711_t *dev, float known_mass, uint8_t times,
                int32_t tare_value) {
  int32_t sum = 0;
  int32_t data = 0;

  // Assume que a tara já foi lida e armazenada anteriormente
  // e agora deve ser posto o peso conhecido no sensor.
  for (size_t i = 0; i < times; i++) {
    hx711_read_data(dev, &data);
    sum += data;
  }
  // Calcula a média das leituras
  float average_reading = (float)sum / times;

  // O fator de calibração é o peso conhecido dividido pela média das leituras
  // menos a tara
  float calibration_factor = known_mass / (average_reading - tare_value);

  return calibration_factor;
}