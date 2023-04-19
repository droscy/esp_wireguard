# WireGuard implementation for ESPHome

This is an implementation of [WireGuard&reg;](https://www.wireguard.com/) VPN
for [ESPHome](https://esphome.io/), based on
[Wireguard Implementation for ESP-IDF](https://github.com/trombik/esp_wireguard)
(by [@trombik](https://github.com/trombik)).

The branch `trombik/main` will be kept in sync with
[@trombik](https://github.com/trombik)'s `main` branch.

The branch `esphome/dev` is my development branch.

[![Build examples](https://github.com/droscy/esp_wireguard/actions/workflows/build.yml/badge.svg)](https://github.com/droscy/esp_wireguard/actions/workflows/build.yml)

## Status

The code is alpha.

A single tunnel to a WireGuard peer has been working.

## Supported boards and frameworks

The code works only on `esp32` boards with both frameworks:
`esp-idf` and `Arduino`.

The original [@trombik](https://github.com/trombik)'s code was designed
for `esp-idf` only but it seams to work on `Arduino` too.

## Usage

Add the following configuration to your `yaml` file:

```yaml
# define wireguard external source
external_components:
  - source:
      type: git
      url: https://github.com/droscy/esphome
      ref: feature/wireguard/dev
    components: [ wireguard ]

# setup a time source
# do not use 'homeassistant' platform if Home Assistant is on the remote
# peer because the time synchronization is a prerequisite to establish
# the vpn link
time:
  - platform: sntp
    id: src_time

# setup wireguard
wireguard:
  address: x.y.z.w
  private_key: !secret wg_privkey
  peer_endpoint: wg.server.example
  peer_public_key: !secret wg_peer_pubkey

  # optional netmask (this is the default if omitted)
  netmask: 255.255.255.255

  # optional custom port (this is the wireguard default)
  peer_port: 51820

  # optional pre-shared key
  peer_preshared_key: !secret wg_peer_shrdkey

  # optional keepalive in seconds (disabled by default)
  peer_persistent_keepalive: 0
```

If you give an `id` to the wireguard component you can manually create
a binary sensor to check if the peer is online:

```yaml
wireguard:
  id: vpn
  [...]

binary_sensor:
  - platform: template
    name: 'Wireguard status'
    device_class: connectivity
    lambda: |-
      return id(vpn).is_peer_up();
```

For additional information see:

* the original feature-request [esphome/esphome#1444](https://github.com/esphome/feature-requests/issues/1444)
* my [comment](https://github.com/esphome/feature-requests/issues/1444#issuecomment-1502960116) with detailed description
* the original component proposed by [@lhoracek](https://github.com/lhoracek) in his PR [esphome/esphome#4256](https://github.com/esphome/esphome/pull/4256)

## License

BSD 3-Clause "New" or "Revised" License (SPDX ID: BSD-3-Clause).
See [LICENSE](LICENSE) for details.

[smult.c](src/nacl/crypto_scalarmult/curve25519/ref/smult.c) file is in Public Domain.

## Authors

* Daniel Hope (daniel.hope@smartalock.com)
* Kenta Ida (fuga@fugafuga.org)
* Matthew Dempsky
* D. J. Bernstein
* Tomoyuki Sakurai (y@trombik.org)
* Simone Rossetto (simros85@gmail.com)
