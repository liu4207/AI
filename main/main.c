#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "i2c.h"
#include "i2s.h"
#include "driver/gpio.h"
#include "driver/i2s.h"
#include "driver/i2s_std.h"
#include "driver/i2s_pdm.h"
#include "es8388.h"
#include "audioplay.h"
#include "esp_log.h"
static const char *TAG = "main";

void app_main(void) {
    // Initialize I2C
    i2c_master_init();

    // Initialize I2S
        ESP_LOGI(TAG, "Installing I2S driver...");

    i2s_init();
//  if (i2s_driver_install(I2S_NUM_0, &i2s_config_t, 0, NULL) != ESP_OK) {
//         ESP_LOGE(TAG, "Failed to install I2S driver");
//         return;
//     }    
//     if (i2s_set_pin(I2S_NUM_0, &i2s_pin_config_tt) != ESP_OK) {
//         ESP_LOGE(TAG, "Failed to set I2S pin configuration");
//         return;
//     }
    // Initialize ES8388
    es8388_init();


    es8388_set_volume(33);

    
    // Load and play PCM audio
        ESP_LOGI(TAG, "Initializing audio play...");
    audioplay_init();
        ESP_LOGI(TAG, "Starting audio play...");

    audioplay_start(I2S_NUM_0);

    while (1) {
        vTaskDelay(1000);
    }
}
