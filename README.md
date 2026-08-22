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


### ESP-IDF component

The library is also published to the [ESP Component Registry](https://components.espressif.com/components/esphome/wireguard)
as `esphome/wireguard`, so an ESP-IDF project can add it with:

```console
idf.py add-dependency "esphome/wireguard"
```

The `esphome/libsodium` dependency is declared in `idf_component.yml` and is
resolved by the component manager.


### Plain CMake

Outside of ESP-IDF, `CMakeLists.txt` defines an ordinary static library target,
so a CMake project can consume it with `add_subdirectory()`:

```cmake
add_subdirectory(path/to/wireguard)
target_link_libraries(my_target PRIVATE esphome::wireguard)
```

The target is named `wireguard`, with `esphome::wireguard` as an alias. Include
directories and the warning suppressions are carried on the target.

This is not a portable host build. The sources still expect an ESP-family SDK,
so the consuming project has to put the following on the include path itself:

* lwip, mbedtls and libsodium (if a `sodium` target already exists it is linked
  automatically, otherwise link your own);
* unless building for ESP8266 (non-IDF) or LibreTiny, which have their own
  fallbacks in `esp_wireguard_err.h` and `esp_wireguard_log.h`, the ESP-IDF
  headers `esp_err.h`, `esp_log.h`, `esp_system.h` and `esp_netif.h`.

In other words, this branch of the build exists for toolchains such as
LibreTiny and the ESP8266 SDK that drive CMake themselves; an ESP-IDF project
should use the component build above rather than `add_subdirectory()`.


## Compatibility

This code targets only ESPHome and has been tested on the following platforms:

* ESP32 (with both frameworks)
* ESP8266
* LibreTiny (with `bk72` microcontrollers only)


## Tests

`tests/` holds two compile tests for the build files, both run by CI on every
pull request:

* `tests/idf` is a minimal ESP-IDF project that pulls the repository root in as
  a component and calls into `esp_wireguard.h`. It covers `CMakeLists.txt` and
  `idf_component.yml` together: the component manager resolves the declared
  `esphome/libsodium` dependency from the registry while configuring, and the
  link step fails if it did not. Build it with `idf.py -C tests/idf build`
  after running ESP-IDF's `export` script.

* `tests/cmake` is a consumer project for the plain CMake path. It only
  configures and never compiles, because as described above that branch is not
  a portable host build: the sources expect lwip, mbedtls, libsodium and an
  ESP-family SDK, none of which a stock host has. It checks what this
  repository is responsible for, which is that `add_subdirectory()` defines the
  documented targets, that the advertised include dirs contain the public
  headers, and that the optional `sodium` link is guarded correctly.

  ```console
  cmake -S tests/cmake -B build/cmake-test
  ```


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
