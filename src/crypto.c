#include "crypto.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <sodium.h>

/*
 * wireguard_aead_encrypt - Authenticated encryption with ChaCha20Poly1305-IETF.
 *
 * Wraps libsodium's crypto_aead_chacha20poly1305_ietf_encrypt.
 * The WireGuard protocol uses a 64-bit counter as nonce, encoded as:
 *   npub[12] = { 0x00, 0x00, 0x00, 0x00, counter[0..7] (little-endian) }
 * as specified in RFC 7539 section 2.8.
 *
 * The 16-byte authentication tag is appended to the ciphertext in dst,
 * so dst must be at least src_len + 16 bytes.
 */
void wireguard_aead_encrypt(uint8_t *dst, const uint8_t *src, size_t src_len,
                            const uint8_t *ad, size_t ad_len,
                            uint64_t nonce, const uint8_t *key) {
	uint8_t npub[crypto_aead_chacha20poly1305_ietf_NPUBBYTES] = {0};
	// Encode the 64-bit counter as little-endian bytes starting at offset 4
	npub[4] = (uint8_t)(nonce);
	npub[5] = (uint8_t)(nonce >> 8);
	npub[6] = (uint8_t)(nonce >> 16);
	npub[7] = (uint8_t)(nonce >> 24);
	npub[8] = (uint8_t)(nonce >> 32);
	npub[9] = (uint8_t)(nonce >> 40);
	npub[10] = (uint8_t)(nonce >> 48);
	npub[11] = (uint8_t)(nonce >> 56);
	crypto_aead_chacha20poly1305_ietf_encrypt(dst, NULL, src, (unsigned long long)src_len,
	                                          ad, (unsigned long long)ad_len,
	                                          NULL, npub, key);
}

/*
 * wireguard_aead_decrypt - Authenticated decryption with ChaCha20Poly1305-IETF.
 *
 * Wraps libsodium's crypto_aead_chacha20poly1305_ietf_decrypt.
 * Nonce construction is identical to wireguard_aead_encrypt.
 * src is expected to be ciphertext followed by the 16-byte authentication tag.
 *
 * Returns true on success (authentication tag verified), false on failure.
 */
bool wireguard_aead_decrypt(uint8_t *dst, const uint8_t *src, size_t src_len,
                            const uint8_t *ad, size_t ad_len,
                            uint64_t nonce, const uint8_t *key) {
	uint8_t npub[crypto_aead_chacha20poly1305_ietf_NPUBBYTES] = {0};
	npub[4] = (uint8_t)(nonce);
	npub[5] = (uint8_t)(nonce >> 8);
	npub[6] = (uint8_t)(nonce >> 16);
	npub[7] = (uint8_t)(nonce >> 24);
	npub[8] = (uint8_t)(nonce >> 32);
	npub[9] = (uint8_t)(nonce >> 40);
	npub[10] = (uint8_t)(nonce >> 48);
	npub[11] = (uint8_t)(nonce >> 56);
	return crypto_aead_chacha20poly1305_ietf_decrypt(dst, NULL, NULL,
	                                                 src, (unsigned long long)src_len,
	                                                 ad, (unsigned long long)ad_len,
	                                                 npub, key) == 0;
}

/*
 * wireguard_xaead_encrypt - Authenticated encryption with XChaCha20Poly1305-IETF.
 *
 * Wraps libsodium's crypto_aead_xchacha20poly1305_ietf_encrypt.
 * Used for WireGuard cookie replies, which require a 24-byte random nonce.
 * The nonce parameter is a pointer to a 24-byte buffer (COOKIE_NONCE_LEN).
 *
 * The 16-byte authentication tag is appended to the ciphertext in dst.
 */
void wireguard_xaead_encrypt(uint8_t *dst, const uint8_t *src, size_t src_len,
                             const uint8_t *ad, size_t ad_len,
                             const uint8_t *nonce, const uint8_t *key) {
	crypto_aead_xchacha20poly1305_ietf_encrypt(dst, NULL, src, (unsigned long long)src_len,
	                                           ad, (unsigned long long)ad_len,
	                                           NULL, nonce, key);
}

/*
 * wireguard_xaead_decrypt - Authenticated decryption with XChaCha20Poly1305-IETF.
 *
 * Wraps libsodium's crypto_aead_xchacha20poly1305_ietf_decrypt.
 * Nonce is a pointer to a 24-byte buffer (COOKIE_NONCE_LEN).
 *
 * Returns true on success (authentication tag verified), false on failure.
 */
bool wireguard_xaead_decrypt(uint8_t *dst, const uint8_t *src, size_t src_len,
                             const uint8_t *ad, size_t ad_len,
                             const uint8_t *nonce, const uint8_t *key) {
	return crypto_aead_xchacha20poly1305_ietf_decrypt(dst, NULL, NULL,
	                                                  src, (unsigned long long)src_len,
	                                                  ad, (unsigned long long)ad_len,
	                                                  nonce, key) == 0;
}

