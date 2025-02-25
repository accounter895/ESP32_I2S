#include "my_i2s.h"

void My_I2S::my_inmp441_init(){
    const i2s_config_t inmp441_i2s_config = {
        .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX), // Receive, not transfer
        .sample_rate = SAMPLE_RATE,                         // 采样频率44KHz
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,       // 采样精度16位
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,        // 只接收左声道数据
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,  // I2S标准格式
        .intr_alloc_flags = 0,                              // Interrupt level 1
        .dma_buf_count = 8,                                 // 量化电平为8位(分辨率)
        .dma_buf_len = bufferlen,                           // samples per buffer
        .use_apll = false
    };

    const i2s_pin_config_t inmp441_pin_config = {
        .bck_io_num = INMP441_BCK,   
        .ws_io_num = INMP441_WS,    
        .data_out_num = -1, // not used (only for speakers)
        .data_in_num = INMP441_SD   
    };

    err = i2s_driver_install(I2S_PORT_INMP441, &inmp441_i2s_config, 0, NULL);
    if (err != ESP_OK) {
      Serial.printf("Failed installing driver: %d\n", err);
      while (true);
    }
    err = i2s_set_pin(I2S_PORT_INMP441, &inmp441_pin_config);
    if (err != ESP_OK) {
      Serial.printf("Failed setting pin: %d\n", err);
      while (true);
    }
    Serial.println("INMP441 driver installed.");
 
}

void My_I2S::my_max98357a_init(){
    const i2s_config_t max98357a_i2s_config = {
        .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_TX), // Transmit, not receive
        .sample_rate = SAMPLE_RATE,                         // 采样频率44KHz
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,       // 采样精度16位
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,        // 只发送左声道数据
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,  // I2S标准格式
        .intr_alloc_flags = 0,                              // Interrupt level 1
        .dma_buf_count = 8,                                 // 量化电平为8位(分辨率)
        .dma_buf_len = bufferlen,                           // samples per buffer
        .use_apll = false
    };

    const i2s_pin_config_t max98357a_pin_config = {
        .bck_io_num = MAX98357_BCLK,   
        .ws_io_num = MAX98357_LRC,    
        .data_out_num = MAX98357_DIN, // not used (only for speakers)
        .data_in_num = -1   
    };

    err = i2s_driver_install(I2S_PORT_MAX98357A, &max98357a_i2s_config, 0, NULL);
    if (err != ESP_OK) {
      Serial.printf("Failed installing driver: %d\n", err);
      while (true);
    }
    err = i2s_set_pin(I2S_PORT_MAX98357A, &max98357a_pin_config);
    if (err != ESP_OK) {
      Serial.printf("Failed setting pin: %d\n", err);
      while (true);
    }
    Serial.println("INMP441 driver installed.");
}

void My_I2S::inmp441_max98357_loop() {
    uint16_t data[1024];
    esp_err_t result;
    size_t bytes_read = 0;
    result = i2s_read(I2S_NUM_0, &data, sizeof(data), &bytes_read, portMAX_DELAY);
    //Serial.println(bytes_read);
    size_t bytes_write;
    result = i2s_write(I2S_NUM_1, &data, sizeof(data), &bytes_write, portMAX_DELAY);
}
