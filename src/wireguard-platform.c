/*
 * Copyright (c) 2022 Tomoyuki Sakurai <y@trombik.org>
 * Copyright (c) 2023-2024 Simone Rossetto <simros85@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "wireguard-platform.h"

#include <stdlib.h>
#include <sys/time.h>
#include <inttypes.h>

#include "lwip/sys.h"

#if defined(ESP8266) && !defined(IDF_VER)
#include <osapi.h>
#define esp_fill_random(out, size) os_get_random(out, size)
#elif defined(LIBRETINY)
#include <libretiny.h>
#define esp_fill_random(out, size) lt_rand_bytes(out, size)
#else // defined(LIBRETINY)
#include <esp_system.h>
#endif // defined(ESP8266) && !defined(IDF_VER)

#include "esp_wireguard_err.h"
#include "esp_wireguard_log.h"
#include "crypto.h"
#include "crypto/refc/chacha20.h"

#define TAG "wireguard-platform"

/*
 * ChaCha20 fast-key-erasure DRBG (D.J. Bernstein scheme).
 * State: a 256-bit ChaCha20 key seeded from hardware entropy on init.
 * Per-call: fresh hardware entropy is XOR-ed into the key, one 64-byte
 * ChaCha20 block is generated, the first 32 bytes replace the key (forward
 * secrecy / key erasure) and the remaining 32 bytes are the output pool.
 */
static uint8_t  rng_key[CHACHA20_KEY_SIZE];
static uint64_t rng_counter = 0;

esp_err_t wireguard_platform_init() {
	esp_fill_random(rng_key, sizeof(rng_key));
	return ESP_OK;
}

void wireguard_random_bytes(void *bytes, size_t size) {
	uint8_t *out = (uint8_t *)bytes;

	while (size > 0) {
		struct chacha20_ctx ctx;
		uint8_t block[CHACHA20_BLOCK_SIZE];
		static const uint8_t zeros[CHACHA20_BLOCK_SIZE] = {0};
		uint8_t hw[CHACHA20_KEY_SIZE];
		size_t chunk;

		/* Reseed: mix fresh hardware entropy into the key. */
		esp_fill_random(hw, sizeof(hw));
		for (int i = 0; i < CHACHA20_KEY_SIZE; i++) {
			rng_key[i] ^= hw[i];
		}

		/* Generate one 64-byte keystream block. */
		chacha20_init(&ctx, rng_key, rng_counter++);
		chacha20(&ctx, block, zeros, sizeof(block));

		/* Key erasure: first 32 bytes become the new key (forward secrecy). */
		memcpy(rng_key, block, CHACHA20_KEY_SIZE);

		/* Output: second 32 bytes. */
		chunk = size < CHACHA20_KEY_SIZE ? size : CHACHA20_KEY_SIZE;
		memcpy(out, block + CHACHA20_KEY_SIZE, chunk);
		out  += chunk;
		size -= chunk;

		crypto_zero(block, sizeof(block));
		crypto_zero(hw, sizeof(hw));
		crypto_zero(&ctx, sizeof(ctx));
	}
}

uint32_t wireguard_sys_now() {
	// Default to the LwIP system time
	return sys_now();
}

void wireguard_tai64n_now(uint8_t *output) {
	// See https://cr.yp.to/libtai/tai64.html
	// 64 bit seconds from 1970 = 8 bytes
	// 32 bit nano seconds from current second

	struct timeval tv;
	gettimeofday(&tv, NULL);

	uint64_t seconds = 0x400000000000000aULL + tv.tv_sec;
	uint32_t nanos = tv.tv_usec * 1000;
	U64TO8_BIG(output + 0, seconds);
	U32TO8_BIG(output + 8, nanos);
}

bool wireguard_is_under_load() {
	return false;
}
// vim: noexpandtab
