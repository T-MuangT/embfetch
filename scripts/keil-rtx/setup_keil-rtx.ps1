# package_keilrtx.ps1
# Copies embfetch Keil RTX port files into your project.
# Usage: .\package_keilrtx.ps1 <destination_src_dir>
#   destination_src_dir : path to your project's src directory

param (
    [Parameter(Mandatory=$true, Position=0)]
    [string]$DestDir
)

$ErrorActionPreference = "Stop"

$EmbfetchRoot = Resolve-Path "$PSScriptRoot\..\.."
$Src = "$EmbfetchRoot\src"

if (-not (Test-Path $DestDir -PathType Container)) {
    Write-Error "Destination directory '$DestDir' does not exist."
    exit 1
}

Write-Host "Copying embfetch Keil RTX port files..."

Copy-Item "$Src\sysinfo_rtos_common.c"            "$DestDir\sysinfo_rtos_common.c"
Copy-Item "$Src\include\embed_sysinfo.h"          "$DestDir\embed_sysinfo.h"
Copy-Item "$Src\rtos\keil-rtx\sysinfo_keilrtx.c" "$DestDir\sysinfo_keilrtx.c"
Copy-Item "$Src\rtos\keil-rtx\include\logo.h"    "$DestDir\logo.h"

Write-Host "Files copied to '$DestDir':"
Write-Host "  sysinfo_rtos_common.c"
Write-Host "  embed_sysinfo.h"
Write-Host "  sysinfo_keilrtx.c"
Write-Host "  logo.h"

Write-Host ""
Write-Host "Next steps:"
Write-Host "  1. Add the copied files to your project using one of the following:"
Write-Host "     - uVision: Project > Add Existing Files to Group"
Write-Host "     - MDK v6 / VS Code: Add files to your project's CMakeLists.txt manually"
Write-Host "  2. Add entry and register the embfetch command in your shell source file."
Write-Host "  3. Include 'embed_sysinfo.h' in your shell source file."