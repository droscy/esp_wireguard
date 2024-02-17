#if !defined(__ESP_WIREGUARD_LOG__H__)
#define __ESP_WIREGUARD_LOG__H__

#if defined(ESP8266) && !defined(IDF_VER)

// do nothing, simply prevent compilation errors if ESP_* macros are missing
#define _noop(x, ...) do {} while(0)

#ifndef ESP_LOGE
#define ESP_LOGE(tag, ...) _noop(tag, __VA_ARGS__)
#endif

#ifndef ESP_LOGW
#define ESP_LOGW(tag, ...) _noop(tag, __VA_ARGS__)
#endif

#ifndef ESP_LOGI
#define ESP_LOGI(tag, ...) _noop(tag, __VA_ARGS__)
#endif

#ifndef ESP_LOGD
#define ESP_LOGD(tag, ...) _noop(tag, __VA_ARGS__)
#endif

#ifndef ESP_LOGV
#define ESP_LOGV(tag, ...) _noop(tag, __VA_ARGS__)
#endif

#else  // defined(ESP8266) && !defined(IDF_VER)
#include <esp_log.h>
#endif  // defined(ESP8266) && !defined(IDF_VER)

#endif  // __ESP_WIREGUARD_LOG__H__
