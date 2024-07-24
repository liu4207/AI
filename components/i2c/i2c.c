#include "i2c.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
static const char *TAG = "i2c";

esp_err_t i2c_master_init(void) {
        int i2c_master_port = I2C_NUM_0;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = 41,  // SDA引脚
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = 42,  // SCL引脚
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 400000    // I2C时钟速度
    };
    esp_err_t err = i2c_param_config(I2C_NUM_0, &conf);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "I2C param config failed: %s", esp_err_to_name(err));
        return err;
    }else{
        ESP_LOGI(TAG, "I2C param config succeed...");
        // return err;
    }
     err=i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0);
     if (err != ESP_OK) {
        ESP_LOGE(TAG, "I2C driver install failed: %s", esp_err_to_name(err));
        return err;
           }else{
         ESP_LOGI(TAG, "I2C driver installed successfully");
    }

    return err;
}

esp_err_t i2c_transfer(i2c_obj_t *self, uint16_t addr, size_t n, i2c_buf_t *bufs, unsigned int flags)
{
    int data_len = 0;
    esp_err_t ret = ESP_FAIL;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();                                                       /* 创建一个命令链接,将一系列待发送给从机的数据填充命令链接 */

    /* 根据器件通信时序去决定flags参数,进而选择如下代码不同的执行情况 */
    if (flags & I2C_FLAG_WRITE)
    {
        i2c_master_start(cmd);                                                                          /* 启动位 */
        i2c_master_write_byte(cmd, addr << 1, ACK_CHECK_EN);                                            /* 从机地址 + 写操作位 */
        i2c_master_write(cmd, bufs->buf, bufs->len, ACK_CHECK_EN);                                      /* len个数据 */
        data_len += bufs->len; 
        --n;
        ++bufs;
    }

    i2c_master_start(cmd);                                                                              /* 启动位 */
    i2c_master_write_byte(cmd, addr << 1 | (flags & I2C_FLAG_READ), ACK_CHECK_EN);                      /* 从机地址 + 读/写操作位 */

    for (; n--; ++bufs)
    {
        if (flags & I2C_FLAG_READ)
        {
            i2c_master_read(cmd, bufs->buf, bufs->len, n == 0 ? I2C_MASTER_LAST_NACK : I2C_MASTER_ACK); /* 读取数据 */
        }
        else
        {
            if (bufs->len != 0)
            {
                i2c_master_write(cmd, bufs->buf, bufs->len, ACK_CHECK_EN);                              /* len个数据 */
            }
        }
        data_len += bufs->len;
    }

    if (flags & I2C_FLAG_STOP)
    {
        i2c_master_stop(cmd);                                                                           /* 停止位 */
    }

    ret = i2c_master_cmd_begin(self->port, cmd, 100 * (1 + data_len) / portTICK_PERIOD_MS);             /* 触发I2C控制器执行命令链接,即命令发送 */
    i2c_cmd_link_delete(cmd);                                                                           /* 释放命令链接使用的资源 */

    return ret;
}
