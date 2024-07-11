#include "i2s.h"
#include "driver/i2s.h"
#include "driver/i2s_std.h"
#include "driver/i2s_pdm.h"
#include "esp_log.h"
static const char *TAG = "i2s";

void i2s_trx_start(void)
{
    i2s_start(I2S_NUM_0);
    }
    void i2s_trx_stop(void)
    {
        i2s_stop(I2S_NUM_0);
        }
void i2s_deinit(void)
{
    i2s_driver_uninstall(I2S_NUM_0);
    
}
void i2s_init() {
    i2s_config_t i2s_config = {
        .mode = I2S_MODE_MASTER | I2S_MODE_TX ,
        .sample_rate = 44100,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = 0,//ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 256,
        .use_apll = false,
        .tx_desc_auto_clear = true,
        .fixed_mclk = 0
    };
    i2s_pin_config_t pin_config = {
        .bck_io_num = 46,
        .ws_io_num = 9,
        .data_out_num = 10,
        .data_in_num = 14,
        .mck_io_num = 3 // 设置主时钟引脚
    };


    // i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    // i2s_set_pin(I2S_NUM_0, &pin_config);
    // i2s_set_clk(I2S_NUM_0, 44100, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_STEREO);
  ESP_LOGI(TAG, "Installing I2S driver...");
    if (i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to install I2S driver");
        return;
    }

    ESP_LOGI(TAG, "Setting I2S pin configuration...");
    if (i2s_set_pin(I2S_NUM_0, &pin_config) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set I2S pin configuration");
        return;
    }

    ESP_LOGI(TAG, "Setting I2S clock...");
    if (i2s_set_clk(I2S_NUM_0, 44100, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_STEREO) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set I2S clock");
        return;
    }

    ESP_LOGI(TAG, "I2S initialization complete");


size_t i2s_tx_write(uint8_t *buffer, uint32_t frame_size)
{
    size_t bytes_written;
    i2s_write(I2S_NUM_0, buffer, frame_size, &bytes_written, 100);
    return bytes_written;
    }
    // size_t i2s_rx_read(uint8_t *buffer, uint32_t frame_size)
    // {
    //     size_t bytes_written;
    //     i2s_read(I2S_NUM, buffer, frame_size, &bytes_written, 1000);
    //     return bytes_written;
    //     }


}
