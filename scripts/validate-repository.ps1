param(
    [string]$RepositoryRoot = (Split-Path -Parent $PSScriptRoot)
)

$ErrorActionPreference = 'Stop'
$failures = [System.Collections.Generic.List[string]]::new()

function Add-Failure([string]$Message) {
    $failures.Add($Message)
    Write-Host "FAIL  $Message" -ForegroundColor Red
}

$requiredHeadings = @(
    '## 🎯 วัตถุประสงค์',
    '## 🧰 อุปกรณ์ที่ใช้',
    '## ❓ คำถามก่อนเริ่มต้น',
    '## 📖 ทฤษฎี',
    '## 🔌 การต่อวงจร',
    '## 🧪 การทดลอง',
    '## 🛠️ Troubleshooting',
    '## 🔍 คำถามค้นคว้าหลังเรียน',
    '## 📝 แบบทดสอบหลังเรียน',
    '## 📚 อ้างอิง',
    '## ➡️ แล็บถัดไป'
)

$labReadmes = Get-ChildItem -LiteralPath (Join-Path $RepositoryRoot 'labs') -Directory -Filter 'lab*' |
    Sort-Object Name | ForEach-Object { Join-Path $_.FullName 'README.md' }

foreach ($readme in $labReadmes) {
    $labName = Split-Path (Split-Path $readme -Parent) -Leaf
    $content = Get-Content -LiteralPath $readme -Raw
    $previousIndex = -1
    foreach ($heading in $requiredHeadings) {
        $headingIndex = $content.IndexOf($heading, [StringComparison]::Ordinal)
        if ($headingIndex -lt 0) {
            Add-Failure "$labName ไม่มีหัวข้อ $heading"
        } elseif ($headingIndex -le $previousIndex) {
            Add-Failure "$labName เรียงหัวข้อไม่ตรง template ที่ $heading"
        }
        $previousIndex = $headingIndex
    }

    $troubleshooting = [regex]::Match(
        $content,
        '(?ms)^## 🛠️ Troubleshooting\s*(?<body>.*?)^## 🔍 คำถามค้นคว้าหลังเรียน\s*$'
    )
    if (-not $troubleshooting.Success) {
        Add-Failure "$labName แยกส่วน Troubleshooting ไม่ได้"
    } else {
        $dataRows = [regex]::Matches($troubleshooting.Groups['body'].Value, '(?m)^\|[^\r\n]+\|\s*$') |
            Where-Object { $_.Value -notmatch '^\|\s*(อาการ|[-: ]+)\s*\|' }
        if ($dataRows.Count -lt 5) {
            Add-Failure "$labName มี Troubleshooting เพียง $($dataRows.Count) รายการ"
        }
    }

    $imageLinks = [regex]::Matches($content, '!\[[^\]]*\]\((?<path>images/[^)#?]+)') |
        ForEach-Object { $_.Groups['path'].Value } | Sort-Object -Unique
    if ($imageLinks.Count -lt 3) {
        Add-Failure "$labName มีภาพใน README เพียง $($imageLinks.Count) ภาพ"
    }
    foreach ($relativeImage in $imageLinks) {
        $fullImage = Join-Path (Split-Path $readme -Parent) $relativeImage
        if (-not (Test-Path -LiteralPath $fullImage)) {
            Add-Failure "$labName ลิงก์ภาพที่ไม่มีไฟล์: $relativeImage"
        }
    }

    # ตรวจ local link และ image link ทุกชนิด; external URL ตรวจด้วย markdown-link-check แยกต่างหาก
    $links = [regex]::Matches($content, '!?(?:\[[^\]]*\])\((?<target>[^)]+)\)')
    foreach ($link in $links) {
        $target = $link.Groups['target'].Value.Trim().Trim('<', '>')
        if ($target -match '^(https?://|mailto:|#)') { continue }
        $pathOnly = ($target -split '#', 2)[0]
        if ([string]::IsNullOrWhiteSpace($pathOnly)) { continue }
        $decoded = [Uri]::UnescapeDataString($pathOnly)
        $fullTarget = Join-Path (Split-Path $readme -Parent) $decoded
        if (-not (Test-Path -LiteralPath $fullTarget)) {
            Add-Failure "$labName ลิงก์ local เสีย: $target"
        }
    }

    if ($failures.Count -eq 0) {
        Write-Host "PASS  $labName — template/images/local links"
    }
}

$sourceManifest = Get-Content -LiteralPath (Join-Path $RepositoryRoot 'assets/IMAGE_SOURCES.md') -Raw
$labImages = @(& git -C $RepositoryRoot ls-files --cached --others --exclude-standard -- labs) |
    Sort-Object -Unique | Where-Object { $_ -match '\.(svg|png|jpe?g|gif|webp)$' } |
    ForEach-Object { $p = Join-Path $RepositoryRoot $_; if (Test-Path -LiteralPath $p) { Get-Item -LiteralPath $p } }
foreach ($image in $labImages) {
    $relative = [IO.Path]::GetRelativePath($RepositoryRoot, $image.FullName).Replace('\', '/')
    if ($sourceManifest -notmatch [regex]::Escape($relative)) {
        Add-Failure "IMAGE_SOURCES.md ไม่มี $relative"
    }
}
if ($labImages.Count -gt 0 -and -not $failures.Count) {
    Write-Host "PASS  IMAGE_SOURCES.md — $($labImages.Count) image files"
}

$tracked = @(& git -C $RepositoryRoot ls-files)
$forbidden = @($tracked | Where-Object {
    $_ -match '(^|/)(_reference|answer-keys)/' -or $_ -match '(^|/)secrets\.h$'
})
if ($forbidden.Count -gt 0) {
    Add-Failure "พบไฟล์ต้องห้ามใน git: $($forbidden -join ', ')"
} else {
    Write-Host 'PASS  ไม่มี _reference/, answer-keys/ หรือ secrets.h ใน git'
}

if ($failures.Count -gt 0) {
    throw "Repository QA ไม่ผ่าน $($failures.Count) รายการ"
}

Write-Host "Validated $($labReadmes.Count) labs, $($labImages.Count) images and tracked-file policy."
