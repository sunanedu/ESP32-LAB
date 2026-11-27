"""ตรวจสัญญาพินคงที่ Lab02–07 เทียบ constants ในโค้ดจริงทั้ง S3/WROOM."""
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
S3 = dict(LED_PIN=5, SECOND_LED_PIN=2, BUTTON_PIN=18, RED_PIN=10,
          YELLOW_PIN=11, GREEN_PIN=12, SDA_PIN=8, SCL_PIN=9,
          I2C_SDA_PIN=8, I2C_SCL_PIN=9, RELAY_1_PIN=16,
          RELAY_2_PIN=17, TRIG_PIN=6, ECHO_PIN=7, DHT_PIN=4)
WROOM = dict(S3, LED_PIN=16, RED_PIN=17, YELLOW_PIN=23, GREEN_PIN=32,
             SDA_PIN=21, SCL_PIN=22, I2C_SDA_PIN=21, I2C_SCL_PIN=22,
             RELAY_1_PIN=26, RELAY_2_PIN=27, TRIG_PIN=25, ECHO_PIN=33)


def target_source(source, target):
    """เลือกเฉพาะบล็อก target ที่ใช้ในหลักสูตร; ไม่ประเมินโค้ดหรือไฟล์ secrets."""
    def choose(match):
        s3, separator, rest = match[1].partition(
            "#elif defined(CONFIG_IDF_TARGET_ESP32)")
        if not separator:
            raise ValueError("บล็อกพินต้องระบุ S3 และ ESP32 แยกกัน")
        wroom = rest.partition("#else")[0]
        return s3 if target == "S3" else wroom
    return re.sub(r"#if defined\(CONFIG_IDF_TARGET_ESP32S3\)\s*\n(.*?)#endif",
                  choose, source, flags=re.S)


def main():
    # ชุด S3 ไม่มีสองหน้าที่ใช้ GPIO เดียวกัน (aliases ของ I2C นับเป็นบัสเดียว)
    roles = {name: pin for name, pin in S3.items() if not name.startswith("I2C_")}
    assert len(set(roles.values())) == len(roles), "S3 มีพินชนกัน"
    assert not (set(roles.values()) & ({0, 3, 19, 20, 45, 46} | set(range(26, 38))))

    ino_files = sorted(path for lab in ROOT.glob("labs/lab0[2-7]*")
                       for path in lab.glob("code/*/*.ino"))
    files = ino_files + [ROOT / "labs/lab07-smart-home-project/code/lab07_smart_home/config.h"]
    coverage = {target: set() for target in ("S3", "WROOM")}
    for target, expected in (("S3", S3), ("WROOM", WROOM)):
        for path in files:
            source = target_source(path.read_text(encoding="utf-8"), target)
            values = dict((name, int(value)) for name, value in re.findall(
                r"constexpr\s+(?:uint8_t|int)\s+(\w+_PIN)\s*=\s*(\d+)\s*;", source))
            for name, value in values.items():
                if name not in expected:
                    raise AssertionError(f"ไม่รู้จักพิน {name} ใน {path}")
                assert value == expected[name], f"{target} {path.name}: {name}={value}, ต้องเป็น{expected[name]}"
                coverage[target].add(name)
            for members in re.findall(r"LED_PINS\[\]\s*=\s*\{([^}]+)\}", source):
                pins = [int(token.strip()) if token.strip().isdigit() else values[token.strip()]
                        for token in members.split(",")]
                assert pins == [expected["SECOND_LED_PIN"], expected["LED_PIN"]]
        assert coverage[target] == set(expected), f"{target}: coverage ไม่ครบ {set(expected) - coverage[target]}"

    # ทุกสเก็ตช์ต้องยังมีโค้ดเต็มที่ตรงไฟล์ในบทเรียนหน้าเดียว
    for ino in ino_files:
        readme = ino.parents[2] / "README.md"
        blocks = [block.strip() for block in re.findall(
            r"```cpp\s*\n(.*?)```", readme.read_text(encoding="utf-8"), re.S)]
        assert ino.read_text(encoding="utf-8").strip() in blocks, f"README/code ไม่ตรง: {ino}"
    module_files = sorted(ROOT.glob("module-tests/*/*.ino"))
    assert len(module_files) == 6, "ชุดทดสอบแยกต้องมี 6 สเก็ตช์รวม OLED"
    for ino in module_files:
        source = ino.read_text(encoding="utf-8")
        assert ino.stem == ino.parent.name, f"ชื่อ sketch/โฟลเดอร์ไม่ตรง: {ino}"
        assert "#if !defined(CONFIG_IDF_TARGET_ESP32S3)" in source
        for name, value in re.findall(
                r"constexpr\s+uint8_t\s+(\w+_PIN)\s*=\s*(\d+)\s*;", source):
            assert name in S3 and int(value) == S3[name], f"พินชุดทดสอบไม่ตรง: {ino} {name}={value}"
    lab08_files = sorted(ROOT.glob("labs/lab08-hardware-check/code/*/*.ino"))
    assert len(lab08_files) == 6, "Lab08 ต้องมี 6 สเก็ตช์ทดสอบพื้นฐาน"
    lab08_readme = (ROOT / "labs/lab08-hardware-check/README.md").read_text(encoding="utf-8")
    lab08_blocks = [block.strip() for block in re.findall(r"```cpp\s*\n(.*?)```", lab08_readme, re.S)]
    for ino in lab08_files:
        source = ino.read_text(encoding="utf-8")
        assert ino.stem == ino.parent.name and "#if !defined(CONFIG_IDF_TARGET_ESP32S3)" in source
        assert source.strip() in lab08_blocks, f"README/code Lab08 ไม่ตรง: {ino}"
        for name, value in re.findall(r"constexpr\s+uint8_t\s+(\w+_PIN)\s*=\s*(\d+)\s*;", source):
            assert name in S3 and int(value) == S3[name], f"พิน Lab08 ไม่ตรง: {ino} {name}={value}"
    print(f"PASS: fixed S3/WROOM pin map, no S3 conflicts, README/code parity {len(ino_files)} sketches")
    print(f"PASS: {len(module_files)} standalone S3 module sketches use the same fixed pins")
    print(f"PASS: Lab08 {len(lab08_files)} sketches use fixed S3 pins and match README")
    lab09 = ROOT / "labs/lab09-secure-homeguard"
    lab09_files = sorted(p for p in lab09.glob("code/*/*.ino") if p.parent.name != "lab09_s3_homeguard")
    assert len(lab09_files) == 3, "Lab09 starter ต้องมี 3 sketches"
    lab09_blocks = [block.strip() for block in re.findall(
        r"```cpp\s*\n(.*?)```", (lab09 / "README.md").read_text(encoding="utf-8"), re.S)]
    expected09 = dict(S3, RELAY1_PIN=S3['RELAY_1_PIN'], RELAY2_PIN=S3['RELAY_2_PIN'])
    for ino in lab09_files:
        source = ino.read_text(encoding="utf-8")
        assert source.strip() in lab09_blocks, f"README/code Lab09 ไม่ตรง: {ino}"
        assert "#if !defined(CONFIG_IDF_TARGET_ESP32S3)" in source
        for name, value in re.findall(r"constexpr\s+uint8_t\s+(\w+_PIN)\s*=\s*(\d+)\s*;", source):
            assert int(value) == expected09[name], f"พิน Lab09 ไม่ตรง: {name}"
    print("PASS: Lab09 3 starter sketches use fixed S3 pins and match README")
    runtime_path = lab09 / "code/lab09_s3_homeguard/lab09_s3_homeguard.ino"
    if runtime_path.exists():
        runtime = (lab09 / "code/lab09_s3_homeguard/lab09_s3_homeguard.ino").read_text(encoding="utf-8")
        for name, value in {"RELAY_1_PIN":16, "RELAY_2_PIN":17, "DHT_PIN":4, "BUTTON_PIN":18, "OLED_SDA_PIN":8, "OLED_SCL_PIN":9}.items():
            assert re.search(rf"\b{name}\s*=\s*{value}\b", runtime), f"Lab09 runtime pin mismatch: {name}"
        assert "HG_BARE_BOARD?0:BUTTON_PIN" in runtime, "BOOT0 must be limited to bare-board mode"
        print("PASS: Lab09 secure development runtime uses the same S3 module pins")
    else:
        print("INFO: instructor secure runtime is private; public preflight pin checks completed")


if __name__ == "__main__":
    main()
