# setup_zephyr_builtin.ps1
# Copies embfetch Zephyr port files into your project and updates CMakeLists.txt target_sources().
# Usage: .\setup_zephyr_builtin.ps1 <destination_src_dir> [cmake_path]
#   destination_src_dir : path to your project's src directory
#   cmake_path          : path to your CMakeLists.txt (optional, defaults to <destination_src_dir>\..\CMakeLists.txt)

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
    $CMakePath = Join-Path (Split-Path $DestDir -Parent) "CMakeLists.txt"
}

if (-not (Test-Path $CMakePath -PathType Leaf)) {
    Write-Error "CMakeLists.txt not found at '$CMakePath'. Specify it as the second argument."
    exit 1
}

Write-Host "Copying embfetch Zephyr port files..."

Copy-Item "$Src\sysinfo_rtos_common.c"                 "$DestDir\sysinfo_rtos_common.c"
Copy-Item "$Src\include\embed_sysinfo.h"               "$DestDir\embed_sysinfo.h"
Copy-Item "$Src\rtos\zephyr\sysinfo_zephyr_common.c"   "$DestDir\sysinfo_zephyr_common.c"
Copy-Item "$Src\rtos\zephyr\zephyr\sysinfo_zephyr.c"  "$DestDir\sysinfo_zephyr.c"
Copy-Item "$Src\rtos\zephyr\zephyr\include\logo.h"    "$DestDir\logo.h"

Write-Host "Files copied to '$DestDir':"
Write-Host "  sysinfo_rtos_common.c"
Write-Host "  embed_sysinfo.h"
Write-Host "  sysinfo_zephyr_common.c"
Write-Host "  sysinfo_zephyr.c"
Write-Host "  logo.h"

Write-Host ""
Write-Host "Updating target_sources() in '$CMakePath'..."

$CMakeContent = Get-Content $CMakePath -Raw

if ($CMakeContent -match "sysinfo_rtos_common\.c") {
    Write-Warning "CMakeLists.txt already contains embfetch sources. Skipping edit."
} elseif ($CMakeContent -match "target_sources") {
    $CMakeContent = $CMakeContent -replace "(target_sources\s*\([^)]*)", {
        $_.Value.TrimEnd() + "`r`n    src/sysinfo_rtos_common.c`r`n    src/sysinfo_zephyr_common.c`r`n    src/sysinfo_zephyr.c"
    }
    Set-Content $CMakePath $CMakeContent -NoNewline
    Write-Host "CMakeLists.txt updated."
} else {
    Write-Warning "No target_sources() found in '$CMakePath'."
    Write-Host "Manually add the following to your target_sources():"
    Write-Host "    src/sysinfo_rtos_common.c"
    Write-Host "    src/sysinfo_zephyr_common.c"
    Write-Host "    src/sysinfo_zephyr.c"
}

Write-Host ""
Write-Host "Next steps:"
Write-Host "  1. Make sure CONFIG_SHELL=y is set in your prj.conf."
Write-Host "  2. Add entry and register the embfetch command in your main or shell source file."
Write-Host "  3. Include 'embed_sysinfo.h' in your shell source file."