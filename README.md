# WireGuard implementation for ESPHome

This is an implementation of [WireGuard&reg;](https://www.wireguard.com/) VPN
for [ESPHome](https://esphome.io/), based on
[Wireguard Implementation for ESP-IDF](https://github.com/trombik/esp_wireguard)
(by [@trombik](https://github.com/trombik)).

[![PlatformIO Registry](https://badges.registry.platformio.org/packages/droscy/library/esp_wireguard.svg)](https://registry.platformio.org/libraries/droscy/esp_wireguard)


## Usage

Please refer to the official documentation of [WireGuard Component](https://esphome.io/components/wireguard)
in ESPHome website.


## Compatibility

This code targets only ESPHome and has been tested on `esp32` boards (with both frameworks `esp-idf` and `Arduino`)
and on `esp8266` boards.


## References

For additional information see:

* the original feature-request [esphome/feature-requests#1444](https://github.com/esphome/feature-requests/issues/1444)

* the first pull-request [esphome/esphome#4256](https://github.com/esphome/esphome/pull/4256)


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
