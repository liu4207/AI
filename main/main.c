#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "i2c.h"
#include "i2s.h"
#include "es8388.h"
#include "audioplay.h"

void app_main(void) {
    // Initialize I2C
    i2c_master_init();

    // Initialize I2S
    i2s_init();

    // Initialize ES8388
    es8388_init();

    // Load and play PCM audio
    audioplay_init();
    audioplay_start();

    while (1) {
        vTaskDelay(1000);
    }
}
