param(
    [string]$RepositoryRoot = (Split-Path -Parent $PSScriptRoot)
)

$ErrorActionPreference = 'Stop'

function Assert-Content {
    param(
        [bool]$Condition,
        [string]$Message
    )

    if (-not $Condition) {
        throw $Message
    }
}

$labReadmes = Get-ChildItem -LiteralPath (Join-Path $RepositoryRoot 'labs') `
    -Directory -Filter 'lab*' |
    ForEach-Object { Join-Path $_.FullName 'README.md' } |
    Where-Object { Test-Path -LiteralPath $_ }

$readyLabs = @()

foreach ($readme in $labReadmes) {
    $content = Get-Content -LiteralPath $readme -Raw

    # หน้า Roadmap ไม่ใช่แล็บพร้อมเรียน จึงยังไม่ถูกนับเป็น quiz ที่ต้องผ่าน
    if ($content -notmatch '(?m)^## ❓ คำถามก่อนเริ่มต้น\s*$') {
        Write-Output "SKIP  $readme (Roadmap)"
        continue
    }

    $readyLabs += $readme
    $labNumberMatch = [regex]::Match($readme, 'lab(?<number>\d{2})-')
    Assert-Content $labNumberMatch.Success "ระบุหมายเลข Lab จาก path ไม่ได้: $readme"
    $labNumber = $labNumberMatch.Groups['number'].Value

    # REQ-02: แยกเฉพาะส่วน Pre-Lab แล้วตรวจจำนวนและรูปแบบ
    $preLabMatch = [regex]::Match(
        $content,
        '(?ms)^## ❓ คำถามก่อนเริ่มต้น\s*(?<body>.*?)^## 📖 ทฤษฎี\s*$'
    )
    Assert-Content $preLabMatch.Success "Lab ${labNumber}: ไม่พบขอบเขต Pre-Lab ตาม template"
    $preLab = $preLabMatch.Groups['body'].Value
    Assert-Content ($preLab -match '<details>') "Lab ${labNumber}: Pre-Lab ไม่มี <details>"
    Assert-Content ($preLab -match '<summary><b>คลิกเพื่อดูคำถามก่อนเรียน</b></summary>') `
        "Lab ${labNumber}: summary ของ Pre-Lab ไม่ตรงมาตรฐาน"
    Assert-Content ($preLab -match '</details>') "Lab ${labNumber}: Pre-Lab ไม่มี </details>"
    Assert-Content ($preLab -notmatch 'เฉลย') "Lab ${labNumber}: Pre-Lab มีคำว่า เฉลย"

    $preLabQuestions = [regex]::Matches($preLab, '(?m)^\d+\.\s+.+\?\s*$')
    Assert-Content ($preLabQuestions.Count -ge 5 -and $preLabQuestions.Count -le 8) `
        "Lab ${labNumber}: Pre-Lab ต้องมี 5–8 ข้อ แต่พบ $($preLabQuestions.Count)"

    # REQ-04: ตรวจโครงสร้าง quiz, difficulty และตัวเลือก
    $quizMatch = [regex]::Match(
        $content,
        '(?ms)^## 📝 แบบทดสอบหลังเรียน\s*(?<body>.*?)^## 📚 อ้างอิง\s*$'
    )
    Assert-Content $quizMatch.Success "Lab ${labNumber}: ไม่พบขอบเขตแบบทดสอบตาม template"
    $quiz = $quizMatch.Groups['body'].Value
    Assert-Content ($quiz -notmatch 'เฉลย|คำตอบ\s*[:：]') `
        "Lab ${labNumber}: README เปิดเผยคำตอบในส่วนแบบทดสอบ"

    $questionMatches = [regex]::Matches(
        $quiz,
        '(?m)^\*\*ข้อ (?<number>\d+) \((?<difficulty>ง่าย|ปานกลาง|ยาก)\)\*\*'
    )
    Assert-Content ($questionMatches.Count -eq 10) `
        "Lab ${labNumber}: แบบทดสอบต้องมี 10 ข้อ แต่พบ $($questionMatches.Count)"

    for ($index = 0; $index -lt 10; $index++) {
        $expectedNumber = $index + 1
        $actualNumber = [int]$questionMatches[$index].Groups['number'].Value
        Assert-Content ($actualNumber -eq $expectedNumber) `
            "Lab ${labNumber}: ลำดับข้อไม่ต่อเนื่องที่ข้อ $expectedNumber"

        $expectedDifficulty = if ($expectedNumber -le 3) {
            'ง่าย'
        } elseif ($expectedNumber -le 7) {
            'ปานกลาง'
        } else {
            'ยาก'
        }
        $actualDifficulty = $questionMatches[$index].Groups['difficulty'].Value
        Assert-Content ($actualDifficulty -eq $expectedDifficulty) `
            "Lab $labNumber ข้อ ${expectedNumber}: ต้องเป็น $expectedDifficulty แต่พบ $actualDifficulty"

        $blockStart = $questionMatches[$index].Index
        $blockEnd = if ($index -lt 9) {
            $questionMatches[$index + 1].Index
        } else {
            $quiz.Length
        }
        $questionBlock = $quiz.Substring($blockStart, $blockEnd - $blockStart)

        foreach ($choice in @('ก', 'ข', 'ค', 'ง')) {
            $choiceCount = [regex]::Matches($questionBlock, "(?m)^- $choice\.\s+.+$").Count
            Assert-Content ($choiceCount -eq 1) `
                "Lab $labNumber ข้อ ${expectedNumber}: ตัวเลือก $choice ต้องมี 1 รายการ แต่พบ $choiceCount"
        }
    }

    # ไฟล์ answer key อยู่ local-only แต่ต้องมีหนึ่งคำตอบต่อข้อเพื่อใช้ QA
    $answerPath = Join-Path $RepositoryRoot "answer-keys/lab$labNumber-answers.md"
    Assert-Content (Test-Path -LiteralPath $answerPath) `
        "Lab ${labNumber}: ไม่พบ answer key แบบ local-only ที่ $answerPath"
    $answerContent = Get-Content -LiteralPath $answerPath -Raw
    $answerMatches = [regex]::Matches(
        $answerContent,
        '(?m)^\|\s*(?<number>\d+)\s*\|\s*(?<answer>[กขคง])\s*\|'
    )
    Assert-Content ($answerMatches.Count -eq 10) `
        "Lab ${labNumber}: answer key ต้องมี 10 แถว แต่พบ $($answerMatches.Count)"

    $answers = @($answerMatches | ForEach-Object { $_.Groups['answer'].Value })
    for ($index = 0; $index -lt 10; $index++) {
        $answerNumber = [int]$answerMatches[$index].Groups['number'].Value
        Assert-Content ($answerNumber -eq ($index + 1)) `
            "Lab ${labNumber}: answer key เรียงลำดับไม่ถูกต้อง"
    }

    $counts = @{}
    foreach ($choice in @('ก', 'ข', 'ค', 'ง')) {
        $counts[$choice] = @($answers | Where-Object { $_ -eq $choice }).Count
    }
    $maxCount = ($counts.Values | Measure-Object -Maximum).Maximum
    $minCount = ($counts.Values | Measure-Object -Minimum).Minimum
    Assert-Content (($maxCount - $minCount) -le 1) `
        "Lab ${labNumber}: ตำแหน่งคำตอบไม่สมดุล ก=$($counts['ก']) ข=$($counts['ข']) ค=$($counts['ค']) ง=$($counts['ง'])"

    $runLength = 1
    for ($index = 1; $index -lt $answers.Count; $index++) {
        if ($answers[$index] -eq $answers[$index - 1]) {
            $runLength++
        } else {
            $runLength = 1
        }
        Assert-Content ($runLength -le 3) `
            "Lab ${labNumber}: คำตอบตำแหน่งเดิมติดกันเกิน 3 ข้อ"
    }

    Write-Output "PASS  Lab $labNumber — REQ-02/REQ-04"
}

Assert-Content ($readyLabs.Count -gt 0) 'ไม่พบแล็บสถานะพร้อมเรียนสำหรับตรวจ quiz'
Write-Output "Validated $($readyLabs.Count) ready lab(s)."
