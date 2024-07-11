#ifndef ES8388_H
#define ES8388_H
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
void es8388_init(void);
void es8388_set_volume(uint8_t volume);
#endif // ES8388_H
    