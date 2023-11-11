#ifndef I2SBUS_H_
#define I2SBUS_H_

#include "driver/i2s.h"
#include <Arduino.h>

#define I2S_NUM (i2s_port_t)(0)
#define WAVE_HEADER_SIZE (44)

class I2SBus
{
public:
  uint32_t SAMPLE_RATE;
  uint32_t BPS;

  i2s_config_t i2s_config;
  i2s_pin_config_t pin_config;
  I2SBus(uint32_t sample_rate, uint8_t bps, uint8_t bck, uint8_t din, uint8_t lck);
  ~I2SBus();

  void begin();
  void start();
  void stop();
  void end();
  void clear();

  void write(uint8_t *buffer, size_t bytes_read);

private:
  size_t bytes_written;
  // I2S 설정과 관련된 변수 및 메서드를 선언합니다.
};

#endif // I2SBUS_H_
