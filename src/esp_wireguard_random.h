#if !defined(__ESP_WIREGUARD_RANDOM__H__)
#define __ESP_WIREGUARD_RANDOM__H__

#if defined(ESP8266) && !defined(ESP_IDF_VERSION)
#include "osapi.h"
#define esp_fill_random(out, size) os_get_random(out, size)

#else  // defined(ESP8266) && !defined(ESP_IDF_VERSION)
#include <esp_system.h>
#endif  // defined(ESP8266) && !defined(ESP_IDF_VERSION)

#endif  // __ESP_WIREGUARD_RANDOM__H__
