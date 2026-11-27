"""Sync collapsed code and rebuild Lab09 ZIP from allowlisted public sources."""
from pathlib import Path
from zipfile import ZipFile, ZipInfo, ZIP_DEFLATED

ROOT = Path(__file__).resolve().parents[1]
LAB = ROOT/'labs/lab09-secure-homeguard'
START, END = '<!-- FULL_STARTER_CODE -->', '<!-- END_STARTER_CODE -->'
readme = LAB/'README.md'
content = readme.read_text(encoding='utf-8')
blocks = []
for ino in sorted((LAB/'code').glob('*/*.ino')):
    relative = ino.relative_to(LAB).as_posix()
    blocks.append(f'<details><summary>{ino.stem} — โค้ดเต็ม</summary>\n\n[เปิดไฟล์ .ino]({relative})\n\n```cpp\n{ino.read_text(encoding="utf-8").rstrip()}\n```\n\n</details>')
example = LAB/'code/lab09_mqtt_tls_probe/secrets.example.h'
blocks.append('<details><summary>secrets.example.h — คัดลอกเป็นไฟล์ local เท่านั้น</summary>\n\n```cpp\n' + example.read_text(encoding='utf-8').rstrip() + '\n```\n\n</details>')
replacement = START + '\n\n' + '\n\n'.join(blocks) + '\n\n' + END
if END in content:
    before, after = content.split(START, 1)
    content = before + replacement + after.split(END, 1)[1]
else:
    content = content.replace(START, replacement)
readme.write_text(content, encoding='utf-8')
out = LAB/'downloads/lab09-starter-sketches.zip'
out.parent.mkdir(exist_ok=True)
files = sorted((LAB/'code').glob('*/*.ino')) + [example]
with ZipFile(out, 'w', ZIP_DEFLATED) as archive:
    for file in files:
        info = ZipInfo(file.relative_to(LAB/'code').as_posix(), (2026, 11, 27, 0, 0, 0))
        info.compress_type = ZIP_DEFLATED
        archive.writestr(info, file.read_bytes())
    archive.writestr('HOW_TO_USE.txt', 'Lab09 S3 HomeGuard starter only. Extract first. Open one sketch at a time.\nHardware preflight: SH1106G + DHT11 + button; relays stay OFF.\nCrypto self-test: PUBLIC TEST KEYS ONLY. Never provision test keys.\nTLS probe: copy secrets.example.h to local secrets.h, set Wi-Fi and provider CA.\nNo native provisioning/secure remote actuator implementation is included.\nSee labs/lab09-secure-homeguard/README.md in the course repository.\n')
with ZipFile(out) as archive:
    assert archive.testzip() is None
    assert len(archive.namelist()) == 5
    assert not any(Path(name).name == 'secrets.h' for name in archive.namelist())
    for file in files:
        assert archive.read(file.relative_to(LAB/'code').as_posix()) == file.read_bytes()
print('PASS: collapsed code synced; ZIP contains 3 sketches + example config + HOW_TO_USE, no secrets')
