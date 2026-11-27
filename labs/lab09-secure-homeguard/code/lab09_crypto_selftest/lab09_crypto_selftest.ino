/*
 * Lab09.2 ESP32-S3: HKDF-SHA256 และ AES-256-GCM self-test ด้วย test keys เท่านั้น
 * คง Relay16/17 OFF ไม่ใช้เครือข่าย; mbedTLS มากับ Arduino-ESP32 core3.x
 * เปรียบเทียบกับ protocol/vectors.json; Serial115200; ห้ามใช้ test keys กับอุปกรณ์จริง
 */
#include <Arduino.h>
#include <mbedtls/gcm.h>
#include <mbedtls/hkdf.h>
#include <mbedtls/md.h>
#include <mbedtls/platform_util.h>
#if !defined(CONFIG_IDF_TARGET_ESP32S3)
#error "Select ESP32S3 Dev Module"
#endif
constexpr uint8_t RELAY_1_PIN = 16;
constexpr uint8_t RELAY_2_PIN = 17;
constexpr bool RELAY_ACTIVE_LOW = true;

bool decodeHex(const char *text, uint8_t *output, size_t length) {
  if (strlen(text) != length * 2) return false;
  for (size_t i = 0; i < length; ++i) {
    unsigned int value;
    if (sscanf(text + i * 2, "%2x", &value) != 1) return false;
    output[i] = static_cast<uint8_t>(value);
  }
  return true;
}

void setup() {
  const uint8_t offLevel = RELAY_ACTIVE_LOW ? HIGH : LOW;
  digitalWrite(RELAY_1_PIN, offLevel);
  digitalWrite(RELAY_2_PIN, offLevel);
  pinMode(RELAY_1_PIN, OUTPUT);
  pinMode(RELAY_2_PIN, OUTPUT);
  Serial.begin(115200);
  // RFC5869 Appendix A.1: ต้องตรงค่าอ้างอิง ไม่ตรวจแค่ encrypt/decrypt ตัวเอง
  uint8_t ikm[22], salt[13], info[10], expected[42], derived[42];
  memset(ikm, 0x0b, sizeof(ikm));
  for (size_t i = 0; i < sizeof(salt); ++i) salt[i] = i;
  for (size_t i = 0; i < sizeof(info); ++i) info[i] = 0xf0 + i;
  bool ok = decodeHex("3cb25f25faacd57a90434f64d0362f2a2d2d0a90cf1a5a4c5db02d56ecc4c5bf34007208d5b887185865", expected, sizeof(expected));
  ok = ok && mbedtls_hkdf(mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), salt, sizeof(salt),
                          ikm, sizeof(ikm), info, sizeof(info), derived, sizeof(derived)) == 0;
  ok = ok && memcmp(derived, expected, sizeof(expected)) == 0;
  Serial.printf("[CRYPTO] RFC5869-A1 %s\n", ok ? "PASS" : "FAIL");
  bool allPassed = ok;

  // Test vector สังเคราะห์ร่วม Python: key00..1f, nonce=00000000 || uint64BE(1)
  uint8_t key[32], nonce[12] = {}, ciphertext[16], tag[16], plain[16] = {};
  uint8_t expectedCipher[16], expectedTag[16], recovered[16];
  for (size_t i = 0; i < sizeof(key); ++i) key[i] = i;
  nonce[11] = 1;
  constexpr char AAD[] = "v1|1|00112233445566778899aabbccddeeff|1|iot/v1/00112233-4455-4677-8899-aabbccddeeff/cmd";
  // ค่าสองบรรทัดนี้เติมจาก generator แล้วบันทึกให้ตรวจข้ามภาษา
  ok = decodeHex("15d6bffc44f4301e0e2e5139eca63af7", expectedCipher, sizeof(expectedCipher));
  ok = decodeHex("d43e7347262aede7fd6f28ef716bda66", expectedTag, sizeof(expectedTag)) && ok;
  mbedtls_gcm_context gcm;
  mbedtls_gcm_init(&gcm);
  ok = mbedtls_gcm_setkey(&gcm, MBEDTLS_CIPHER_ID_AES, key, 256) == 0 && ok;
  const int encryptResult = mbedtls_gcm_crypt_and_tag(&gcm, MBEDTLS_GCM_ENCRYPT, sizeof(plain),
      nonce, sizeof(nonce), reinterpret_cast<const uint8_t *>(AAD), strlen(AAD),
      plain, ciphertext, sizeof(tag), tag);
  ok = ok && encryptResult == 0 && memcmp(ciphertext, expectedCipher, sizeof(ciphertext)) == 0 &&
       memcmp(tag, expectedTag, sizeof(tag)) == 0;
  Serial.printf("[CRYPTO] AES256-GCM vector %s\n", ok ? "PASS" : "FAIL");
  allPassed = allPassed && ok;
  const int validResult = mbedtls_gcm_auth_decrypt(&gcm, sizeof(ciphertext), nonce, sizeof(nonce),
      reinterpret_cast<const uint8_t *>(AAD), strlen(AAD), tag, sizeof(tag), ciphertext, recovered);
  ok = validResult == 0 && memcmp(recovered, plain, sizeof(plain)) == 0;
  Serial.printf("[CRYPTO] valid decrypt %s\n", ok ? "PASS" : "FAIL");
  allPassed = allPassed && ok;
  tag[0] ^= 1;
  const int invalidResult = mbedtls_gcm_auth_decrypt(&gcm, sizeof(ciphertext), nonce, sizeof(nonce),
      reinterpret_cast<const uint8_t *>(AAD), strlen(AAD), tag, sizeof(tag), ciphertext, recovered);
  ok = invalidResult == MBEDTLS_ERR_GCM_AUTH_FAILED;
  Serial.printf("[CRYPTO] reject changed tag %s\n", ok ? "PASS" : "FAIL");
  allPassed = allPassed && ok;
  mbedtls_gcm_free(&gcm);
  mbedtls_platform_zeroize(key, sizeof(key));
  mbedtls_platform_zeroize(derived, sizeof(derived));
  mbedtls_platform_zeroize(recovered, sizeof(recovered));
  Serial.printf("[CRYPTO] SUMMARY %s; no network or actuator\n", allPassed ? "PASS" : "FAIL");
}

void loop() { delay(1000); }
