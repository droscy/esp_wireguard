# WireGuard implementation for ESPHome

This is an implementation of [WireGuard&reg;](https://www.wireguard.com/) VPN
for [ESPHome](https://esphome.io/), based on
[Wireguard Implementation for ESP-IDF](https://github.com/trombik/esp_wireguard)
(by [@trombik](https://github.com/trombik)).

[![PlatformIO Registry](https://badges.registry.platformio.org/packages/esphome/library/wireguard.svg)](https://registry.platformio.org/libraries/esphome/wireguard)


## Usage

Please refer to the official documentation of [WireGuard Component](https://esphome.io/components/wireguard)
in ESPHome website.

The library is published to the PlatformIO Registry as `esphome/wireguard`:

```ini
lib_deps = esphome/wireguard
```

> **Note:** this library was previously named `esp_wireguard` and published as
> `droscy/esp_wireguard`. Only the package and repository names changed; the C API
> (`esp_wireguard.h`, `esp_wireguard_init()`, ...) is unchanged.


## Compatibility

This code targets only ESPHome and has been tested on the following platforms:

* ESP32 (with both frameworks)
* ESP8266
* LibreTiny (with `bk72` microcontrollers only)


## References

For additional information see:

* the original feature-request [esphome/feature-requests#1444](https://github.com/esphome/feature-requests/issues/1444)
* the first pull-request [esphome/esphome#4256](https://github.com/esphome/esphome/pull/4256)
* `esp8266` support [esphome/esphome#6365](https://github.com/esphome/esphome/pull/6365)
* LibreTiny support [droscy/esp_wireguard#4](https://github.com/droscy/esp_wireguard/pull/4)
  (in the repository this library was forked from, before the move to
  [esphome-libs/wireguard](https://github.com/esphome-libs/wireguard))


## License

BSD 3-Clause License (SPDX ID: BSD-3-Clause)

This project is licensed under [BSD 3-Clause License](https://spdx.org/licenses/BSD-3-Clause.html)
except where explicitly written in files themselves or when other license files state differently.

"WireGuard" and the "WireGuard" logo are registered trademarks of Jason A. Donenfeld.
Please see ["WireGuard" Trademark Usage Policy](https://www.wireguard.com/trademark-policy/)
for additional information.


## Authors

* Simone Rossetto (simros85@gmail.com)
* Tomoyuki Sakurai (y@trombik.org)
* Daniel Hope (daniel.hope@smartalock.com)
* Kenta Ida (fuga@fugafuga.org)
* Matthew Dempsky
* D. J. Bernstein
