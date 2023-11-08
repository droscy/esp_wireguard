#if !defined(__ESP_WIREGUARD_ERR__H__)
#define __ESP_WIREGUARD_ERR__H__

#if defined(ESP8266) && !defined(ESP_IDF_VERSION)
typedef int esp_err_t;

#define ESP_OK          0       /*!< esp_err_t value indicating success (no error) */
#define ESP_FAIL        -1      /*!< Generic esp_err_t code indicating failure */

#define ESP_ERR_NO_MEM              0x101
#define ESP_ERR_INVALID_ARG         0x102
#define ESP_ERR_INVALID_STATE       0x103
#define ESP_ERR_INVALID_SIZE        0x104
#define ESP_ERR_NOT_FOUND           0x105
#define ESP_ERR_NOT_SUPPORTED       0x106
#define ESP_ERR_TIMEOUT             0x107
#define ESP_ERR_INVALID_RESPONSE    0x108
#define ESP_ERR_INVALID_CRC         0x109
#define ESP_ERR_INVALID_VERSION     0x10A
#define ESP_ERR_INVALID_MAC         0x10B
#define ESP_ERR_NOT_FINISHED        0x10C
#define ESP_ERR_NOT_ALLOWED         0x10D

#define ESP_ERR_WIFI_BASE           0x3000
#define ESP_ERR_MESH_BASE           0x4000
#define ESP_ERR_FLASH_BASE          0x6000
#define ESP_ERR_HW_CRYPTO_BASE      0xc000
#define ESP_ERR_MEMPROT_BASE        0xd000

#else
#include <esp_err.h>
#endif

#endif  // __ESP_WIREGUARD_ERR__H__
