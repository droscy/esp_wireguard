set(WIREGUARD_SRCS
  "src/crypto.c"
  "src/crypto/refc/blake2s.c"
  "src/crypto/refc/chacha20.c"
  "src/crypto/refc/chacha20poly1305.c"
  "src/crypto/refc/poly1305-donna.c"
  "src/esp_wireguard.c"
  "src/wireguard.c"
  "src/wireguardif.c"
  "src/wireguard-platform.c"
)

# mbedtls_ctr_drbg_random()'s return value is deliberately ignored in a few
# places; the sources are upstream's and are not warning-clean without this.
set(WIREGUARD_COMPILE_OPTIONS
  "-Wno-unused-result"
)

if(ESP_PLATFORM)
  # ESP-IDF component build.
  #
  # src is a public include dir, not a private one: the public
  # include/esp_wireguard.h includes esp_wireguard_err.h, which lives in src.
  idf_component_register(
    SRCS ${WIREGUARD_SRCS}
    INCLUDE_DIRS "include" "src"
    REQUIRES lwip
    PRIV_REQUIRES esp_netif mbedtls esphome__libsodium
  )

  target_compile_options(${COMPONENT_LIB} PRIVATE ${WIREGUARD_COMPILE_OPTIONS})
else()
  # Generic CMake build, for consumers that pull this repo in with
  # add_subdirectory().
  cmake_minimum_required(VERSION 3.13)

  add_library(wireguard STATIC ${WIREGUARD_SRCS})
  add_library(esphome::wireguard ALIAS wireguard)

  target_include_directories(wireguard PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}/include"
    # Public, not private: include/esp_wireguard.h includes
    # esp_wireguard_err.h, which lives in src.
    "${CMAKE_CURRENT_LIST_DIR}/src"
  )

  target_compile_options(wireguard PRIVATE ${WIREGUARD_COMPILE_OPTIONS})

  # crypto.h pulls in <sodium.h> for crypto_scalarmult_curve25519(), so link a
  # libsodium target if the consumer has already defined one. lwip and mbedtls
  # are likewise the consumer's to provide; outside ESP-IDF there is no single
  # canonical target name for either, so they are left to the consumer's
  # include path and link line.
  if(TARGET sodium)
    target_link_libraries(wireguard PUBLIC sodium)
  endif()
endif()
