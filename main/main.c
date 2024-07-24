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
#include "xl9555.h"
// #include "exfuns.h"

#include "spi.h"
i2c_obj_t i2c0_master;
void app_main(void) {
        esp_err_t ret;

        uint8_t key = 0;

    // Initialize I2C
    // esp_err_t i2c_ret = i2c_master_init();
    // if (i2c_ret != ESP_OK) {
    //     ESP_LOGE("app_main", "I2C initialization failed");
    //     return;  // I2C initialization failed, handle error accordingly
    // }
    // // Initialize I2S
    //     ESP_LOGI(TAG, "Installing I2S driver...");
    i2c0_master = iic_init(I2C_NUM_0);                  /* 初始化IIC0 */
    spi2_init();                                        /* 初始化SPI */
    xl9555_init(i2c0_master);                           /* 初始化IO扩展芯片 */  

//  if (i2s_driver_install(I2S_NUM_0, &i2s_config_t, 0, NULL) != ESP_OK) {
//         ESP_LOGE(TAG, "Failed to install I2S driver");
//         return;
//     }    
//     if (i2s_set_pin(I2S_NUM_0, &i2s_pin_config_tt) != ESP_OK) {
//         ESP_LOGE(TAG, "Failed to set I2S pin configuration");
//         return;
//     }
    // Initialize ES8388
    // ESP_LOGI(TAG, "Initializing ES8388...");
    // if (es8388_init() != ESP_OK) {
    //     ESP_LOGE(TAG, "ES8388 initialization failed");
    //     return;
    // }
 es8388_init(i2c0_master);                           /* ES8388初始化 */
    es8388_adda_cfg(1, 0);                              /* 开启DAC关闭ADC */
    es8388_input_cfg(0);
    es8388_output_cfg(1, 1);                            /* DAC选择通道输出 */
    es8388_hpvol_set(33);                               /* 设置耳机音量 */
    es8388_spkvol_set(33);                              /* 设置喇叭音量 */
    xl9555_pin_write(0x0004,0);                      /* 打开喇叭 */
    i2s_init();

    //     // key = fonts_update_font(30, 50, 16, (uint8_t *)"0:", RED);  /* 更新字库 */
    // ret = exfuns_init();                                /* 为fatfs相关变量申请内存 */
    vTaskDelay(500);                                    /* 实验信息显示延时 */
    // Load and play PCM audio
    //     ESP_LOGI(TAG, "Initializing audio play...");
    // audioplay_init();
    //     ESP_LOGI(TAG, "Starting audio play...");


    while (1) {
    audioplay_start(I2S_NUM_0);


        vTaskDelay(1000);
    }
}
