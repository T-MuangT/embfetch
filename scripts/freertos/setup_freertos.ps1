# setup.ps1
# Copies embfetch FreeRTOS port files into your project and updates Makefile SRCS.
# Usage: .\setup.ps1 <destination_src_dir> [makefile_path]
#   destination_src_dir : path to your project's src directory
#   makefile_path       : path to your Makefile (optional, defaults to <destination_src_dir>\..\Makefile)

param (
    [Parameter(Mandatory=$true, Position=0)]
    [string]$DestDir,

    [Parameter(Mandatory=$false, Position=1)]
    [string]$MakefilePath
)

$ErrorActionPreference = "Stop"

$EmbfetchRoot = Resolve-Path "$PSScriptRoot\..\.."
$Src = "$EmbfetchRoot\src"

if (-not (Test-Path $DestDir -PathType Container)) {
    Write-Error "Destination directory '$DestDir' does not exist."
    exit 1
}

if (-not $MakefilePath) {
    $MakefilePath = Join-Path (Split-Path $DestDir -Parent) "Makefile"
}

if (-not (Test-Path $MakefilePath -PathType Leaf)) {
    Write-Error "Makefile not found at '$MakefilePath'. Specify it as the second argument."
    exit 1
}

Write-Host "Copying embfetch FreeRTOS port files..."

Copy-Item "$Src\sysinfo_rtos_common.c"                     "$DestDir\sysinfo_rtos_common.c"
Copy-Item "$Src\include\rtos_sysinfo.h"                    "$DestDir\rtos_sysinfo.h"
Copy-Item "$Src\rtos\freertos\freertos\sysinfo_freertos.c" "$DestDir\sysinfo_freertos.c"
Copy-Item "$Src\rtos\freertos\freertos\include\logo.h"     "$DestDir\logo.h"

Write-Host "Files copied to '$DestDir':"
Write-Host "  sysinfo_rtos_common.c"
Write-Host "  rtos_sysinfo.h"
Write-Host "  sysinfo_freertos.c"
Write-Host "  logo.h"
Write-Host ""
Write-Host "Updating Makefile SRCS in '$MakefilePath'..."

$MakefileContent = Get-Content $MakefilePath -Raw

if ($MakefileContent -match "sysinfo_rtos_common\.c") {
    Write-Warning "Makefile already contains embfetch sources. Skipping SRCS edit."
} else {
    if ($MakefileContent -match "^SRCS\s*=") {
        $MakefileContent = $MakefileContent -replace "^SRCS\s*=", "SRCS +="
    }
    $MakefileContent += "`r`n# embfetch`r`nSRCS += sysinfo_rtos_common.c sysinfo_freertos.c`r`n"
    Set-Content $MakefilePath $MakefileContent -NoNewline
    Write-Host "Makefile updated."
}
Write-Host ""
Write-Host "Next steps:"
Write-Host "  1. Add entry and register the embfetch command in your shell source file."
Write-Host "  2. Include 'rtos_sysinfo.h' in your shell source file."