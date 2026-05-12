# package_espidf.ps1
# Copies embfetch ESP-IDF port files into your project and updates main/CMakeLists.txt.
# Usage: .\package_espidf.ps1 <destination_main_dir> [cmake_path]
#   destination_main_dir : path to your project's main directory
#   cmake_path           : path to your main/CMakeLists.txt (optional, defaults to <destination_main_dir>\CMakeLists.txt)

param (
    [Parameter(Mandatory=$true, Position=0)]
    [string]$DestDir,

    [Parameter(Mandatory=$false, Position=1)]
    [string]$CMakePath
)

$ErrorActionPreference = "Stop"

$EmbfetchRoot = Resolve-Path "$PSScriptRoot\..\.."
$Src = "$EmbfetchRoot\src"

if (-not (Test-Path $DestDir -PathType Container)) {
    Write-Error "Destination directory '$DestDir' does not exist."
    exit 1
}

if (-not $CMakePath) {
    $CMakePath = Join-Path $DestDir "CMakeLists.txt"
}

if (-not (Test-Path $CMakePath -PathType Leaf)) {
    Write-Error "CMakeLists.txt not found at '$CMakePath'. Specify it as the second argument."
    exit 1
}

Write-Host "Copying embfetch ESP-IDF port files..."

Copy-Item "$Src\sysinfo_rtos_common.c"                      "$DestDir\sysinfo_rtos_common.c"
Copy-Item "$Src\include\embed_sysinfo.h"                    "$DestDir\embed_sysinfo.h"
Copy-Item "$Src\rtos\freertos\esp-idf\sysinfo_esp-idf.c"   "$DestDir\sysinfo_esp-idf.c"
Copy-Item "$Src\rtos\freertos\esp-idf\include\logo.h"       "$DestDir\logo.h"

Write-Host "Files copied to '$DestDir':"
Write-Host "  sysinfo_rtos_common.c"
Write-Host "  embed_sysinfo.h"
Write-Host "  sysinfo_esp-idf.c"
Write-Host "  logo.h"

Write-Host ""
Write-Host "Updating SRCS in '$CMakePath'..."

$CMakeContent = Get-Content $CMakePath -Raw

if ($CMakeContent -match "sysinfo_rtos_common\.c") {
    Write-Warning "CMakeLists.txt already contains embfetch sources. Skipping SRCS edit."
} elseif ($CMakeContent -match "SRCS") {
    $CMakeContent = $CMakeContent -replace "(SRCS[^)]*)", {
        $_.Value.TrimEnd() + "`r`n    sysinfo_rtos_common.c`r`n    sysinfo_esp-idf.c"
    }
    Write-Host "SRCS updated."
} else {
    $CMakeContent += "`r`n# embfetch`r`nset(SRCS `${SRCS} sysinfo_rtos_common.c sysinfo_esp-idf.c)`r`n"
    Write-Host "SRCS added."
}

Write-Host ""
Write-Host "Updating REQUIRES in '$CMakePath'..."

if ($CMakeContent -match "esp_timer") {
    Write-Warning "CMakeLists.txt already contains embfetch REQUIRES. Skipping REQUIRES edit."
} elseif ($CMakeContent -match "REQUIRES") {
    $CMakeContent = $CMakeContent -replace "(REQUIRES[^)]*)", {
        $_.Value.TrimEnd() + "`r`n    console`r`n    esp_timer`r`n    heap`r`n    esp_system`r`n    esp_hw_support"
    }
    Write-Host "REQUIRES updated."
} else {
    $CMakeContent += "`r`n# embfetch requirements`r`nset(REQUIRES `${REQUIRES} console esp_timer heap esp_system esp_hw_support)`r`n"
    Write-Host "REQUIRES added."
}

Set-Content $CMakePath $CMakeContent -NoNewline

Write-Host ""
Write-Host "Next steps:"
Write-Host "  1. Add entry and register the embfetch command in your main or shell source file."
Write-Host "  2. Include 'embed_sysinfo.h' in your shell source file."
Write-Host "  3. Rebuild the project."