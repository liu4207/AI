#include "wavplay.h"
#include "driver/i2s.h"
#include "esp_log.h"
#include <stdio.h>

static const char *TAG = "WAVPLAY";

esp_err_t wavplay_play(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return ESP_FAIL;
    }

    fseek(f, 44, SEEK_SET); // 跳过WAV头

    char *buffer = malloc(1024);
    size_t bytes_read, bytes_written;
    while ((bytes_read = fread(buffer, 1, 1024, f)) > 0) {
        i2s_write(I2S_NUM_0, buffer, bytes_read, &bytes_written, portMAX_DELAY);
    }

    free(buffer);
    fclose(f);
    return ESP_OK;
}
