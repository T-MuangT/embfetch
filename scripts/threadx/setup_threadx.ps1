# setup.ps1
# Copies embfetch ThreadX port files into your project and updates CMakeLists.txt.
# Usage: .\setup.ps1 <destination_src_dir> [cmake_path]
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

Write-Host "Copying embfetch ThreadX port files..."

Copy-Item "$Src\sysinfo_rtos_common.c"           "$DestDir\sysinfo_rtos_common.c"
Copy-Item "$Src\include\rtos_sysinfo.h"          "$DestDir\rtos_sysinfo.h"
Copy-Item "$Src\rtos\threadx\sysinfo_threadx.c"  "$DestDir\sysinfo_threadx.c"
Copy-Item "$Src\rtos\threadx\include\logo.h"     "$DestDir\logo.h"

Write-Host "Files copied to '$DestDir':"
Write-Host "  sysinfo_rtos_common.c"
Write-Host "  rtos_sysinfo.h"
Write-Host "  sysinfo_threadx.c"
Write-Host "  logo.h"
Write-Host ""
Write-Host "Updating CMakeLists.txt at '$CMakePath'..."

$CMakeContent = Get-Content $CMakePath -Raw

if ($CMakeContent -match "sysinfo_rtos_common\.c") {
    Write-Warning "CMakeLists.txt already contains embfetch sources. Skipping edit."
} elseif ($CMakeContent -match "add_executable") {
    # Insert embfetch sources before the closing parenthesis of add_executable()
    $CMakeContent = $CMakeContent -replace "(add_executable\s*\([^)]*)", {
        $_.Value.TrimEnd() + "`r`n    src/sysinfo_rtos_common.c`r`n    src/sysinfo_threadx.c"
    }
    Set-Content $CMakePath $CMakeContent -NoNewline
    Write-Host "CMakeLists.txt updated."
} else {
    Write-Warning "No add_executable() found in '$CMakePath'."
    Write-Host "Manually add the following to your add_executable():"
    Write-Host "    src/sysinfo_rtos_common.c"
    Write-Host "    src/sysinfo_threadx.c"
}
Write-Host ""
Write-Host "Next steps:"
Write-Host "  1. Add entry and register the embfetch command in your shell source file."
Write-Host "  2. Include 'rtos_sysinfo.h' in your shell source file."