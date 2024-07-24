#ifndef AUDIOPLAY_H
#define AUDIOPLAY_H
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s.h"
#include "esp_log.h"
// #include "ff.h"

void audioplay_init(void);
void audioplay_start(i2s_port_t i2s_num);
/* 音乐播放控制器 */
typedef struct
{
    uint8_t *tbuf;                          /* 临时数组,仅在24bit解码的时候需要用到 */
    // FIL *file;                              /* 音频文件指针 */

    uint8_t status;                         /* bit0:0,暂停播放;1,继续播放 */
                                            /* bit1:0,结束播放;1,开启播放 */
}__audiodev;



#endif // AUDIOPLAY_H
