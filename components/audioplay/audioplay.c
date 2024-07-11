#include "audioplay.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s.h"

const uint8_t pcm_data[] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, // ... 更多PCM数据
    // 假设这里有一个简单的示例PCM数据，实际使用中请替换为您的PCM音频数据
};
void audioplay_init(void) {
    // Initialize any audio buffers or settings here
}

void audioplay_start(void) {
    size_t bytes_written;
    // Play PCM data
    i2s_write(I2S_NUM_0, pcm_data, sizeof(pcm_data), &bytes_written, portMAX_DELAY);
}
