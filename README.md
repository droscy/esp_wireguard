# WireGuard implementation for ESPHome

This is an implementation of [WireGuard&reg;](https://www.wireguard.com/) VPN
for [ESPHome](https://esphome.io/), based on
[Wireguard Implementation for ESP-IDF](https://github.com/trombik/esp_wireguard)
(by [@trombik](https://github.com/trombik)).

The branch `trombik/main` will be kept in sync with
[@trombik](https://github.com/trombik)'s `main` branch.

The branch `main` is where I push my most "stable" code.

[![PlatformIO Registry](https://badges.registry.platformio.org/packages/droscy/library/esp_wireguard.svg)](https://registry.platformio.org/libraries/droscy/esp_wireguard)


## Status

The code is alpha.

A single tunnel to a WireGuard peer has been working.


## Supported boards and frameworks

The code works only on `esp32` boards with both frameworks:
`esp-idf` and `Arduino`.

The original [@trombik](https://github.com/trombik)'s code was designed
for `esp-idf` only but it seems to work on `Arduino` too.


## Usage

Add the following configuration to your ESPHome `yaml` file:

```yaml
# Define wireguard external source
external_components:
  - source:
      type: git
      url: https://github.com/droscy/esphome
      ref: wireguard/main
    components:
      - wireguard
      - wireguard_status
      - wireguard_handshake

# Setup a time source.
# Do not use 'homeassistant' platform if Home Assistant is on the remote
# peer because the time synchronization is a prerequisite to establish
# the vpn link.
time:
  - platform: sntp

# Setup WireGuard
wireguard:
  address: x.y.z.w
  private_key: private_key=
  peer_endpoint: wg.server.example
  peer_public_key: public_key=

  # optional netmask (this is the default if omitted)
  netmask: 255.255.255.255

  # optional custom port (this is the wireguard default)
  peer_port: 51820

  # optional pre-shared key
  peer_preshared_key: shared_key=

  # optional keepalive in seconds (disabled by default)
  peer_persistent_keepalive: 25s

  # optional list of allowed ip/mask (the default is to allow any host if omitted)
  peer_allowed_ips:
    - x.y.z.0/24
    - l.m.n.o/32  # the /32 can be omitted for single host
    - [...]

  # reboot the board if remote peer in unreachable (default to 15min, set to 0s to disable)
  reboot_timeout: 15min
```

### Sensors

The `wireguard_status` binary sensor can be used to check if remote peer is online:

```yaml
binary_sensor:
  - platform: wireguard_status
    name: 'WireGuard Status'

    # optional (default to 10s)
    update_interval: 10s
```

The `wireguard_handshake` sensor can be used to track the timestamp of the
latest completed handshake:

```yaml
sensor:
  - platform: wireguard_handshake
    name: 'WireGuard Latest Handshake'

    # optional (default to 60s)
    update_interval: 60s
```


## References

For additional information see:

* the original feature-request [esphome/esphome#1444](https://github.com/esphome/feature-requests/issues/1444)
  (starting from [this comment](https://github.com/esphome/feature-requests/issues/1444#issuecomment-1556090095))

* the original component proposed by [@lhoracek](https://github.com/lhoracek) in his PR [esphome/esphome#4256](https://github.com/esphome/esphome/pull/4256)

* the proposed documentation [esphome/esphome-docs#2948](https://github.com/esphome/esphome-docs/pull/2948), here
  you can find the link to preview the latest version


## License

BSD 3-Clause License (SPDX ID: BSD-3-Clause).

Except where explicitly written in files themselves or when other license files state differently.


## Authors

* Simone Rossetto (simros85@gmail.com)
* Tomoyuki Sakurai (y@trombik.org)
* Daniel Hope (daniel.hope@smartalock.com)
* Kenta Ida (fuga@fugafuga.org)
* Matthew Dempsky
* D. J. Bernstein
