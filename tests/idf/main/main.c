/*
 * Compile test for the ESP-IDF component build.
 *
 * This is not a functional test: nothing here is expected to succeed at
 * runtime, and the firmware is never flashed. The point is to reference every
 * type, macro and function the public header exposes, so that a broken
 * component build fails at compile or link time.
 */
#include <time.h>

#include "esp_err.h"
#include "esp_wireguard.h"

/* The two DEFAULT macros are the awkward part of the public API: they use
 * designated initialisers and lwip's IPADDR4_INIT(), so they only compile if
 * the component's public include dirs are wired up. */
static wireguard_config_t s_config = ESP_WIREGUARD_CONFIG_DEFAULT();
static wireguard_ctx_t s_ctx = ESP_WIREGUARD_CONTEXT_DEFAULT();

void app_main(void)
{
    time_t handshake = 0;

    s_config.private_key = "private";
    s_config.public_key = "public";
    s_config.preshared_key = "preshared";
    s_config.address = "10.0.0.2";
    s_config.netmask = "255.255.255.0";
    s_config.endpoint = "wireguard.example.com";
    s_config.port = 51820;
    s_config.listen_port = 51820;
    s_config.fw_mark = 0;
    s_config.persistent_keepalive = 30;

    /* esp_wireguard_err.h lives in src/, not include/, and is reachable only
     * because src is registered as a public include dir. */
    if (esp_wireguard_init(&s_config, &s_ctx) != ESP_OK) {
        return;
    }
    if (esp_wireguard_connect(&s_ctx) == ESP_ERR_INVALID_IP) {
        return;
    }

    (void) esp_wireguard_set_default(&s_ctx);
    (void) esp_wireguard_peer_is_up(&s_ctx);
    (void) esp_wireguardif_peer_is_up(&s_ctx);
    (void) esp_wireguard_latest_handshake(&s_ctx, &handshake);
    (void) esp_wireguard_add_allowed_ip(&s_ctx, "192.168.1.0", "255.255.255.0");
    (void) esp_wireguard_restore_default(&s_ctx);
    (void) esp_wireguard_disconnect(&s_ctx);
}
