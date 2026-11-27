"""Host tests only; not evidence of Android/C interoperability or hardware security."""
import copy
import json
import unittest
from pathlib import Path
from cryptography.exceptions import InvalidTag
from reference import receive, vector, TOPIC, SID


class ProtocolTest(unittest.TestCase):
    def setUp(self):
        self.sample = vector()
        self.envelope = self.sample['envelope']
        self.key = bytes.fromhex(self.sample['key_hex'])

    def test_reference_file_matches(self):
        self.assertEqual(self.sample, json.loads(Path(__file__).with_name('vectors.json').read_text()))

    def test_rfc5869(self):
        self.assertEqual(self.sample['hkdf_rfc5869_a1_hex'],
                         '3cb25f25faacd57a90434f64d0362f2a2d2d0a90cf1a5a4c5db02d56ecc4c5bf34007208d5b887185865')

    def test_directional_keys_differ(self):
        self.assertNotEqual(self.sample['session_c2d_test_hex'], self.sample['session_d2c_test_hex'])

    def test_valid(self):
        self.assertEqual(receive(json.dumps(self.envelope).encode(), self.key), (bytes(16), 1))

    def test_tamper_each_authenticated_field(self):
        changes = {'kid': 2, 'sid': 'f'*32, 'seq': '2', 'nonce': 'AAAAAAAAAAAAAAAA',
                   'ct': 'AAAAAAAAAAAAAAAAAAAAAA==', 'tag': 'AAAAAAAAAAAAAAAAAAAAAA=='}
        for field, value in changes.items():
            with self.subTest(field=field):
                modified = copy.deepcopy(self.envelope)
                modified[field] = value
                with self.assertRaises((ValueError, InvalidTag)):
                    receive(json.dumps(modified).encode(), self.key)

    def test_wrong_topic_and_key(self):
        for key, topic in [(bytes(32), TOPIC), (self.key, TOPIC.replace('/cmd', '/state'))]:
            with self.assertRaises(InvalidTag):
                receive(json.dumps(self.envelope).encode(), key, topic=topic)

    def test_replay(self):
        with self.assertRaises(ValueError):
            receive(json.dumps(self.envelope).encode(), self.key, last_seq=1)

    def test_duplicate_fields(self):
        with self.assertRaises(ValueError):
            receive(json.dumps(self.envelope).replace('"v": 1', '"v": 1, "v": 1').encode(), self.key)

    def test_sequence_encoding(self):
        for value in ['01', '0', '-1', str(2**64), 1, True]:
            with self.subTest(value=value), self.assertRaises(ValueError):
                receive(json.dumps({**self.envelope, 'seq': value}).encode(), self.key)

    def test_noncanonical_base64(self):
        for value in [self.envelope['ct'].rstrip('='), self.envelope['ct']+'\n', 42]:
            with self.subTest(value=value), self.assertRaises(ValueError):
                receive(json.dumps({**self.envelope, 'ct': value}).encode(), self.key)

    def test_wire_limit(self):
        with self.assertRaises(ValueError):
            receive(b' '*2049, self.key)


if __name__ == '__main__':
    unittest.main()
