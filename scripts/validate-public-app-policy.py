"""Prevent complete reference app sources from entering the student repository."""
from pathlib import Path
import hashlib

ROOT = Path(__file__).resolve().parents[1]
MEDIA = {'.png', '.svg', '.jpg', '.jpeg', '.gif', '.webp'}
MOBILE = ['labs/lab07-smart-home-project/app-blueprint/mobile',
          'labs/lab09-secure-homeguard/app-blueprint/mobile']
violations = []
for folder in MOBILE + ['labs/lab09-secure-homeguard/code/lab09_s3_homeguard']:
    for path in (ROOT / folder).rglob('*'):
        if path.is_file() and path.suffix.lower() not in MEDIA | {'.md'} and path.name != 'LICENSE':
            violations.append(path.relative_to(ROOT).as_posix())
for forbidden in ['labs/lab09-secure-homeguard/app-blueprint/starter/App.tsx',
                  'labs/lab09-secure-homeguard/tools/new-homeguard-app.ps1']:
    if (ROOT / forbidden).exists(): violations.append(forbidden)
apk = ROOT / 'downloads/esp32-smart-home-preview-1.0.1.apk'
assert apk.is_file()
assert hashlib.sha256(apk.read_bytes()).hexdigest() == '433f0b805f2f1f5967518c41d46f4e8ae9416f7f58468501d57614c03d858cf7'
for path in ROOT.rglob('*.apk'):
    if path != apk: violations.append(path.relative_to(ROOT).as_posix())
assert not violations, 'Private implementation/artifact detected: ' + ', '.join(violations)
print('PASS: student repository contains no completed app implementation; only the approved basic APK')
