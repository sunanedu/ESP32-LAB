"""Test keys only: byte contract reference, not a network service or secure runtime."""
import base64
import json
import re
from pathlib import Path

from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.ciphers.aead import AESGCM
from cryptography.hazmat.primitives.kdf.hkdf import HKDF

TOPIC = "iot/v1/00112233-4455-4677-8899-aabbccddeeff/cmd"
SID = "00112233445566778899aabbccddeeff"
AAD = f"v1|1|{SID}|1|{TOPIC}".encode("ascii")


def derive(root, salt, info, length=32):
    return HKDF(algorithm=hashes.SHA256(), length=length, salt=salt, info=info).derive(root)


def unique_object(pairs):
    result = {}
    for key, value in pairs:
        if key in result:
            raise ValueError("duplicate field")
        result[key] = value
    return result


def decode64(text, size=None):
    if not isinstance(text, str):
        raise ValueError("base64 type")
    data = base64.b64decode(text, validate=True)
    if base64.b64encode(data).decode() != text or (size is not None and len(data) != size):
        raise ValueError("base64 length/canonical form")
    return data


def receive(raw, key, topic=TOPIC, sid=SID, last_seq=0):
    """Validate one session envelope. Caller commits sequence only after schema/policy checks."""
    if len(raw) > 2048:
        raise ValueError("wire limit")
    envelope = json.loads(raw, object_pairs_hook=unique_object)
    if not isinstance(envelope, dict) or set(envelope) != {"v", "kid", "sid", "seq", "nonce", "ct", "tag"}:
        raise ValueError("fields")
    if type(envelope['v']) is not int or envelope['v'] != 1 or type(envelope['kid']) is not int or envelope['kid'] != 1:
        raise ValueError("version/key id")
    if envelope['sid'] != sid or not re.fullmatch('[0-9a-f]{32}', sid):
        raise ValueError("session")
    seq_text = envelope['seq']
    if not isinstance(seq_text, str) or not re.fullmatch('[1-9][0-9]{0,19}', seq_text):
        raise ValueError("sequence encoding")
    seq = int(seq_text)
    if not last_seq < seq <= 2**64 - 1:
        raise ValueError("replay/overflow")
    nonce = decode64(envelope['nonce'], 12)
    if nonce != bytes(4) + seq.to_bytes(8, 'big'):
        raise ValueError("session nonce")
    ciphertext, tag = decode64(envelope['ct']), decode64(envelope['tag'], 16)
    if len(ciphertext) > 1024:
        raise ValueError("plaintext limit")
    aad = f"v1|1|{sid}|{seq_text}|{topic}".encode('ascii')
    # AESGCM does not return plaintext on a failed tag. No GPIO exists in this reference.
    return AESGCM(key).decrypt(nonce, ciphertext + tag, aad), seq


def vector():
    key, nonce, plaintext = bytes(range(32)), bytes(11) + b'\x01', bytes(16)
    result = AESGCM(key).encrypt(nonce, plaintext, AAD)
    encoded = lambda value: base64.b64encode(value).decode('ascii')
    return {
        "purpose": "PUBLIC TEST KEY ONLY; never provision this key",
        "key_hex": key.hex(), "nonce_hex": nonce.hex(), "aad_hex": AAD.hex(),
        "plaintext_hex": plaintext.hex(), "ciphertext_hex": result[:-16].hex(), "tag_hex": result[-16:].hex(),
        "topic": TOPIC,
        "envelope": {"v": 1, "kid": 1, "sid": SID, "seq": "1", "nonce": encoded(nonce),
                     "ct": encoded(result[:-16]), "tag": encoded(result[-16:])},
        "hkdf_rfc5869_a1_hex": derive(bytes([11])*22, bytes(range(13)), bytes(range(240, 250)), 42).hex(),
        "session_c2d_test_hex": derive(key, bytes.fromhex(SID), b"secure-iot/v1/session/c2d").hex(),
        "session_d2c_test_hex": derive(key, bytes.fromhex(SID), b"secure-iot/v1/session/d2c").hex(),
    }


if __name__ == '__main__':
    path = Path(__file__).with_name('vectors.json')
    path.write_text(json.dumps(vector(), indent=2) + '\n', encoding='utf-8')
    print('Wrote deterministic PUBLIC TEST vectors.json')
