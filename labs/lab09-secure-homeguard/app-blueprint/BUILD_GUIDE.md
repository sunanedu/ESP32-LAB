# สร้างและติดตั้งแอป Lab09 บน Windows

อัปเดต 27 พฤศจิกายน 2569 · สำหรับผู้เรียนสร้างโปรเจกต์เอง เทียบ workflow แอปอ้างอิงรุ่น 2.1.3 · [กลับสารบัญ](README.md)

เป้าหมายแรกคือสร้าง **Release APK ที่ทดสอบ native self-test ได้** แล้วจึงจับคู่กับบอร์ด ปัญหา R8/Protobuf ที่เคยพบเกิดเฉพาะ release จึงต้องตรวจ release ตั้งแต่ต้น

## 1. สร้าง source ของตนจาก Markdown ก่อน

Repository สาธารณะไม่มี source แอปอ้างอิงให้ build ตรงจาก clone ให้สร้างโปรเจกต์ใหม่ตาม P0 และ requirements/design/implementation แล้วสร้าง local module/plugins/build script/tests ของตน คำสั่งด้านล่างเป็น workflow เป้าหมายเมื่อไฟล์เหล่านั้นพร้อม ไม่ใช่การรันแอปสำเร็จจาก repository นี้

ก่อนสร้างไฟล์ครบให้ล็อก control และแสดงเหตุผล ห้าม fake success หรือใช้ plaintext MQTT แทน Native

## 2. Environment ที่ใช้เป็น baseline

| รายการ | baseline ที่บันทึก/ตรวจจาก source |
|---|---|
| OS / shell | Windows + PowerShell 7 |
| Node | 24; รายงานเดิมใช้ 24.19.0 |
| Java | JDK 17; `JAVA_HOME` ชี้ราก JDK |
| Android SDK | ตั้ง `ANDROID_HOME` ตามที่ติดตั้ง Android Studio; ใช้ SDK/NDK ตาม generated Gradle ของ baseline |
| CMake | 3.31.6 ตาม `with-short-cmake-path.cjs` |
| Expo / React Native / React | 58.0.26 / 0.88.1 / 19.3.0 ตาม package และ lockfile ของรอบอ้างอิง |
| App Android | minSDK 30, targetSDK 36 ตามผล build อ้างอิง; ตรวจ generated Manifest เมื่อเปลี่ยน dependency |
| Native dependencies | HiveMQ 1.4.2, protobuf-javalite/protoc 3.25.5 ตาม local module Gradle |

ใช้ `package-lock.json`, local module Gradle และ config plugins เป็นฐาน ไม่อัปเกรดทุก dependency พร้อมแก้บั๊ก หากจะเปลี่ยนรุ่นให้แยกงานและทดสอบ release/BLE ใหม่

```powershell
Set-Location D:\AI\S3-HomeGuard\mobile
node --version
npm --version
java -version
Get-Item Env:JAVA_HOME,Env:ANDROID_HOME
adb version
```

ถ้าต้องตั้ง environment ให้ใช้พาธจริงของเครื่อง ตัวอย่างรูปแบบเท่านั้น:

```powershell
$env:JAVA_HOME = 'C:\Program Files\Eclipse Adoptium\jdk-17-example'
$env:ANDROID_HOME = Join-Path $env:LOCALAPPDATA 'Android\Sdk'
Test-Path (Join-Path $env:JAVA_HOME 'bin/java.exe')
Test-Path (Join-Path $env:ANDROID_HOME 'platform-tools/adb.exe')
```

ต้องได้ `True` ทั้งคู่ก่อนใช้ค่าตัวอย่างต่อ พาธ JDK ด้านบนต้องเปลี่ยนให้ตรงการติดตั้งจริง

## 3. ติดตั้ง dependencies และตรวจ TypeScript

```powershell
npm ci
npm run typecheck
npm run lint
npm test
```

**จุดตรวจ:** ทุกคำสั่ง exit 0; tests เดิมมี 3 ข้อด้าน UI/security boundaries ผลนี้ยังไม่ตรวจ Kotlin หรือ BLE

## 4. ตรวจไฟล์ที่ต้องอยู่ใน source

- `modules/homeguard-security/` ต้องมี Kotlin, vendor Java/proto, native tests และ `android/consumer-rules.pro`
- `plugins/` ต้องมีไฟล์ทุกตัวที่ `app.json` อ้างถึง รวม plugins สำหรับ security และ CMake; ถ้า source เปิด fullscreen ต้องมี plugin นั้นด้วย
- `scripts/build-homeguard.ps1`, `app.json`, package/lockfile ต้องเป็นชุดเดียวกัน
- `modules/homeguard-security/android/` เป็น **source ที่ติดตามใน Git** ต่างจาก `mobile/android/` ซึ่ง Expo สร้างใหม่ได้

```powershell
git status --short
git rev-parse HEAD
Get-ChildItem plugins
Test-Path modules/homeguard-security/android/consumer-rules.pro
```

บันทึก commit และ working-tree status ก่อน build เลขเวอร์ชันแอปอย่างเดียวไม่พิสูจน์ว่า APK มาจาก source ชุดใด ห้ามเก็บข้อแก้ไว้เฉพาะ `mobile/android/` หรือ build cache เพราะ Prebuild อาจสร้างไฟล์นั้นใหม่ตาม [Expo Continuous Native Generation](https://docs.expo.dev/workflow/continuous-native-generation)

## 5. Build ที่พาธสั้น

จากโฟลเดอร์ `mobile/`:

```powershell
# ตรวจ JVM tests ก่อน (ไม่สร้าง APK)
pwsh -File scripts/build-homeguard.ps1 -BuildRoot D:\AI\hg09\app -TestsOnly

# JVM tests และ Release APK สำหรับ Samsung/โทรศัพท์ arm64
pwsh -File scripts/build-homeguard.ps1 -BuildRoot D:\AI\hg09\app -Architectures arm64-v8a
```

เลือก BuildRoot แยกจาก source และยาวไม่เกิน 35 ตัวอักษรตาม script ถ้าโฟลเดอร์นั้นมีอยู่แต่ไม่มี `.homeguard-build-root` script จะหยุด ให้เลือกโฟลเดอร์ใหม่ที่ว่างและยังไม่มี ไม่ลบงานเดิมเพื่อฝืนใช้พาธนั้น

Script คัดลอก source ไป cache, รัน `expo prebuild`, JVM tests และ `assembleRelease` แล้วคัดลอก APK กลับ `mobile/dist/apk/` ไม่ต้องเปิด Metro เพื่อใช้ APK นี้

สำหรับ emulator x86_64 ให้เลือก cache อีกแห่งและ architecture ตรงเครื่อง เช่น:

```powershell
pwsh -File scripts/build-homeguard.ps1 -BuildRoot D:\AI\hg09emu\app -Architectures x86_64
```

หาก build หลาย architecture/หลายครั้ง ชื่อ APK ที่ปลายทางยังอิง version เดียวกัน ให้คัดลอกเก็บชื่อที่ระบุ architecture และ hash ก่อนถูก build ครั้งถัดไปเขียนทับ ห้ามสรุปว่า APK arm64 จะติดตั้งได้บน emulator x86_64 เสมอ

**จุดตรวจ:** Gradle `BUILD SUCCESSFUL`, JVM tests ผ่าน (baseline ปัจจุบัน 11 ข้อ) และมีไฟล์ APK อย่าอ่านเพียง exit ของ `robocopy` ระหว่างทางเป็นผล build ทั้งหมด

## 6. ติดตั้งโดยรักษาข้อมูลเดิม

```powershell
$appVersion = (Get-Content app.json -Raw | ConvertFrom-Json).expo.version
$apkPath = Join-Path (Get-Location) "dist/apk/s3-homeguard-$appVersion.apk"
Get-FileHash -LiteralPath $apkPath -Algorithm SHA256
adb devices -l

# เปลี่ยนเป็น serial ที่แสดงสถานะ device ในผลข้างบน
$deviceSerial = 'SERIAL_FROM_ADB'
adb -s $deviceSerial install -r $apkPath
adb -s $deviceSerial shell am start -n com.esp32lab.homeguard/.MainActivity
adb -s $deviceSerial shell dumpsys package com.esp32lab.homeguard | Select-String 'versionName|versionCode'
```

ใช้ `-s` เมื่อมีหลายเครื่อง `install -r` รักษาข้อมูลได้เมื่อ package/signature เข้ากัน หาก signature ไม่ตรงให้ตรวจ signing/source ก่อน ไม่ถอนแอปเพื่อลองแก้ทันที เพราะอาจเสียกุญแจเจ้าของและต้องจับคู่ใหม่

APK แล็บใช้ debug signing key แม้เป็น Release variant จึงยังไม่ใช่ production signing; เก็บ signing key จริงนอก repository เมื่อถึง phase เผยแพร่

## 7. ตรวจบน Release APK ก่อนใช้บอร์ด

1. เปิด **ตั้งค่า → ความปลอดภัย → ทดสอบ crypto ในโทรศัพท์**
2. ต้องผ่าน HKDF, AES-GCM, การปฏิเสธ tag ผิด และการสร้าง/อ่านแพ็กเกตแรก SRP
3. เข้า **ตั้งค่า → MQTT Broker** ทดสอบโปรไฟล์ TLS
4. ตรวจครบห้าแท็บ เลื่อนหน้าเพิ่มจนถึงปุ่ม **จับคู่** และเปิด/ปิดคีย์บอร์ด
5. บันทึก PASS แยก `BUILD`, `PHONE SELF-TEST`, `BROKER PROBE`; เริ่ม hardware pairing ตาม [USER_GUIDE](USER_GUIDE.md) ต่อไป

JVM tests ผ่านอย่างเดียวไม่จับปัญหาที่เกิดหลัง R8 shrink ได้ทั้งหมด **Release self-test บน Android จึงเป็น gate ที่ต้องทำเพิ่ม** ดู [KNOWN_ISSUES](KNOWN_ISSUES.md)

## 8. ตรวจ firmware และ protocol

จากราก repository:

```powershell
Set-Location D:\AI\ESP32-LAB
python scripts/validate-pinout.py
./build/lab09-python/Scripts/python.exe -m unittest discover -s labs/lab09-secure-homeguard/protocol -p test_reference.py -v
```

หากยังไม่มี Python environment ให้ทำตาม [SYSTEM_WORKFLOW หัวข้อ 10](../SYSTEM_WORKFLOW.md#practice) ส่วน firmware ใช้ไฟล์และ board settings จาก [CONFIGURATION](CONFIGURATION.md) อย่าใช้รหัส DEMO จาก preflight กับ native pairing

## 9. เมื่อ build ไม่ผ่าน

| อาการ | ตรวจ/แก้ |
|---|---|
| Java/SDK หาไม่พบ | ตรวจ `JAVA_HOME`, `ANDROID_HOME` และ PATH ของ terminal นี้ |
| CMake/path ยาว | ใช้ BuildRoot สั้นและคง CMake plugin ใน source |
| native module หาไม่พบ | ตรวจ local module/config และทำ Prebuild ผ่าน script; Expo Go ไม่มี module นี้ |
| SRP self-test ล้มเฉพาะ release | ตรวจ `consumer-rules.pro` และ Protobuf versions ตาม I-02 |
| duplicate Netty resources | ตรวจ packaging exclusions ใน security plugin อย่าเพิ่ม dependency ซ้ำเพื่อแก้ |
| install ไม่สำเร็จ | ตรวจ architecture, serial, package/signature และหน้าต่าง Android ที่รอผู้ใช้ |

หลังผ่านให้บันทึก source commit/status, tool versions, architecture, versionCode, APK SHA-256, รุ่นโทรศัพท์ และผลตาม [test-plan](test-plan.md) เมื่อเป็นการแก้เอกสารอย่างเดียว ไม่ต้อง flash หรือจับคู่ซ้ำเพื่อสร้างผลผ่านใหม่
