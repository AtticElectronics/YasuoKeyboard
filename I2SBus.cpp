#include "I2SBus.h"

I2SBus::I2SBus(uint32_t sample_rate, uint8_t bps, uint8_t bck, uint8_t din, uint8_t lck)
{
  // I2S 초기화를 위한 코드를 추가합니다.
  // I2S 설정
  SAMPLE_RATE = sample_rate;
  BPS = bps;
  // I2S_CHANNEL_FMT_ONLY_LEFT ,I2S_CHANNEL_FMT_RIGHT_LEFT
  i2s_config = {
      .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_TX),
      .sample_rate = SAMPLE_RATE,
      .bits_per_sample = (i2s_bits_per_sample_t)BPS,
      .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
      .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S),
      .intr_alloc_flags = 0,
      .dma_buf_count = 8,
      .dma_buf_len = 1024,
  };
  // I2S 핀 설정
  pin_config = {
      .bck_io_num = bck,   // BCK 핀
      .ws_io_num = lck,    // WS 핀
      .data_out_num = din, // DATA 핀
      .data_in_num = I2S_PIN_NO_CHANGE,
  };
}

I2SBus::~I2SBus()
{
  // I2S 정리를 위한 코드를 추가합니다.
  end();
}

void I2SBus::begin()
{
  // I2S 드라이버 설치 및 핀 설정
  i2s_driver_install(I2S_NUM, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM, &pin_config);
  i2s_set_clk(I2S_NUM, SAMPLE_RATE, (i2s_bits_per_sample_t)BPS, I2S_CHANNEL_MONO);
}

void I2SBus::end()
{
  // I2S 종료 및 정리
  i2s_zero_dma_buffer(I2S_NUM);
  i2s_stop(I2S_NUM);
  i2s_driver_uninstall(I2S_NUM);
}

void I2SBus::start()
{
  // I2S 전송 시작
  i2s_zero_dma_buffer(I2S_NUM);
  i2s_start(I2S_NUM);
}

void I2SBus::stop()
{
  // I2S 전송 중지
  i2s_stop(I2S_NUM);
}

void I2SBus::clear()
{
  // 버퍼비우기
  i2s_zero_dma_buffer(I2S_NUM);
}

void I2SBus::write(uint8_t *buffer, size_t bytes_read)
{
  i2s_write(I2S_NUM, buffer, bytes_read, &bytes_written, portMAX_DELAY);
}
