#include "es8388.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// #include "driver/i2c.h"
#include "esp_log.h"

// #include "i2c.h"/
#define ES8388_ADDR 0x20  // 根据具体ES8388地址进行修改
#define I2C_MASTER_NUM I2C_NUM_0
#define I2C_MASTER_SCL_IO 42
#define I2C_MASTER_SDA_IO 41
#define I2C_MASTER_FREQ_HZ 400000
static const char *TAG = "ES8388";


// i2c_obj_t es8388_i2c_master;

// esp_err_t es8388_write_reg(uint8_t reg_addr, uint8_t data)
// {
//     i2c_buf_t buf[2] = {
//         {.len = 1, .buf = &reg_addr},
//         {.len = 1, .buf = &data},
//     };

//     i2c_transfer(&es8388_i2c_master, ES8388_ADDR >> 1, 2, buf, I2C_FLAG_STOP);

//     return ESP_OK;
// }

// void es8388_write_reg(uint8_t reg, uint8_t val) {
//     i2c_cmd_handle_t cmd = i2c_cmd_link_create();
//     i2c_master_start(cmd);
//     i2c_master_write_byte(cmd, (ES8388_ADDR << 1) | I2C_MASTER_WRITE, true);
//     i2c_master_write_byte(cmd, reg, true);
//     i2c_master_write_byte(cmd, val, true);
//     i2c_master_stop(cmd);
//     i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
//     i2c_cmd_link_delete(cmd);
//     // esp_err_t ret_val = ESP_OK;
//     //     if (ret_val != ESP_OK)
//     // {
//     //     while(1)
//     //     {
//     //         printf("ES8388初始化失败！！！\r\n");
//     //         vTaskDelay(500);
//     //     }
//     // }
//     // else
//     // {
//     //     printf("ES8388初始化成功！！！\r\n");
//     // }
// }

esp_err_t es8388_write_reg(uint8_t reg_addr, uint8_t data) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (ES8388_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg_addr, true);
    i2c_master_write_byte(cmd, data, true);
    i2c_master_stop(cmd);
    esp_err_t err = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return err;
}
void es8388_adda_cfg(uint8_t dacen, uint8_t adcen) {
    uint8_t tempreg = 0;

    tempreg |= !dacen << 0;
    tempreg |= !adcen << 1;
    tempreg |= !dacen << 2;
    tempreg |= !adcen << 3;
    es8388_write_reg(0x02, tempreg);
}

void es8388_output_cfg(uint8_t o1en, uint8_t o2en) {
    uint8_t tempreg = 0;
    tempreg |= o1en * (3 << 4);
    tempreg |= o2en * (3 << 2);
    es8388_write_reg(0x04, tempreg);
}

void es8388_init() {
    // 配置I2C
    // i2c_config_t conf;
    // conf.mode = I2C_MODE_MASTER;
    // conf.sda_io_num = I2C_MASTER_SDA_IO;
    // conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    // conf.scl_io_num = I2C_MASTER_SCL_IO;
    // conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    // conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    // i2c_param_config(I2C_MASTER_NUM, &conf);
    // i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);

    // 复位ES8388
    // i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    // i2c_master_start(cmd);
    // i2c_master_write_byte(cmd, (ES8388_ADDR << 1) | I2C_MASTER_WRITE, true);
    // i2c_master_write_byte(cmd, 0x00, true); // Register address
    // i2c_master_write_byte(cmd, 0x80, true); // Reset command
    // i2c_master_stop(cmd);
    // i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    // i2c_cmd_link_delete(cmd);
  ESP_LOGI(TAG, "Initializing ES8388...");
    //  
     es8388_write_reg(0, 0x80);       /* 软复位ES8388 */
 
    vTaskDelay(100);                            /* 等待复位 */
    es8388_write_reg(0, 0x00);
    
     es8388_write_reg(0x01, 0x58);
     es8388_write_reg(0x01, 0x50);
     es8388_write_reg(0x02, 0xF3);
     es8388_write_reg(0x02, 0xF0);

     es8388_write_reg(0x03, 0x09);    /* 麦克风偏置电源关闭 */
     es8388_write_reg(0x00, 0x06);    /* 使能参考 500K驱动使能 */
     es8388_write_reg(0x04, 0x00);    /* DAC电源管理，不打开任何通道 */
     es8388_write_reg(0x08, 0x00);    /* MCLK不分频 */
     es8388_write_reg(0x2B, 0x80);    /* DAC控制 DACLRC与ADCLRC相同 */

     es8388_write_reg(0x09, 0x88);    /* ADC L/R PGA增益配置为+24dB */
     es8388_write_reg(0x0C, 0x4C);    /* ADC	数据选择为left data = left ADC, right data = left ADC  音频数据为16bit */
     es8388_write_reg(0x0D, 0x02);    /* ADC配置 MCLK/采样率=256 */
     es8388_write_reg(0x10, 0x00);    /* ADC数字音量控制将信号衰减 L  设置为最小！！！ */
     es8388_write_reg(0x11, 0x00);    /* ADC数字音量控制将信号衰减 R  设置为最小！！！ */

     es8388_write_reg(0x17, 0x18);    /* DAC 音频数据为16bit */
     es8388_write_reg(0x18, 0x02);    /* DAC 配置 MCLK/采样率=256 */
     es8388_write_reg(0x1A, 0x00);    /* DAC数字音量控制将信号衰减 L  设置为最小！！！ */
     es8388_write_reg(0x1B, 0x00);       /* DAC数字音量控制将信号衰减 R  设置为最小！！！ */
     es8388_write_reg(0x27, 0xB8);    /* L混频器 */
     es8388_write_reg(0x2A, 0xB8);    /* R混频器 */
    vTaskDelay(100);

    es8388_adda_cfg(1, 0); // Enable DAC, disable ADC
    es8388_output_cfg(1, 1); // Enable both output channels
    // 初始化其他寄存器
    // ... 根据需求添加其他寄存器初始化代码
    ESP_LOGI(TAG, "ES8388 initialization complete");

    vTaskDelay(pdMS_TO_TICKS(100));  // 延时100ms等待复位完成
}
void es8388_set_volume(uint8_t volume) {
    if (volume > 33) {
        volume = 33;
    }
      es8388_write_reg(0x2E, volume); // Left headphone volume                                          
    es8388_write_reg(0x2F, volume); // Right headphone volume
    es8388_write_reg(0x30, volume); // Left speaker volume
    es8388_write_reg(0x31, volume); // Right speaker volume
}/**
 * @brief       ES8388 ADC输出通道配置
 * @param       in : 输入通道
 *    @arg      0, 通道1输入
 *    @arg      1, 通道2输入
 * @retval      无
 */
void es8388_input_cfg(uint8_t in)
{
    es8388_write_reg(0x0A, (5 * in) << 4);   /* ADC1 输入通道选择L/R	INPUT1 */
}
/**
 * @brief       设置3D环绕声
 * @param       depth : 0 ~ 7(3D强度,0关闭,7最强)
 * @retval      无
 */
void es8388_3d_set(uint8_t depth)
{
    depth &= 0x7;       /* 限定范围 */
    es8388_write_reg(0x1D, depth << 2);    /* R7,3D环绕设置 */
}
// void es8388_adda_cfg(uint8_t dacen, uint8_t adcen) {
//     uint8_t tempreg = 0;
//     tempreg |= !dacen << 0;
//     tempreg |= !adcen << 1;
//     tempreg |= !dacen << 2;
//     tempreg |= !adcen << 3;
//     es8388_write_reg(0x02, tempreg);
// }

// void es8388_output_cfg(uint8_t o1en, uint8_t o2en) {
//     uint8_t tempreg = 0;
//     tempreg |= o1en * (3 << 4);
//     tempreg |= o2en * (3 << 2);
//     es8388_write_reg(0x04, tempreg);
// }