# package_nuttx.ps1
# Copies embfetch NuttX port files into apps/examples/embfetch and updates apps/examples/Kconfig.
# Usage: .\package_nuttx.ps1 <apps_dir>
#   apps_dir : path to your NuttX apps/ directory

param (
    [Parameter(Mandatory=$true, Position=0)]
    [string]$AppsDir
)

$ErrorActionPreference = "Stop"

$EmbfetchRoot = Resolve-Path "$PSScriptRoot\..\.."
$Src = "$EmbfetchRoot\src"

# --- Validate paths ---
if (-not (Test-Path $AppsDir -PathType Container)) {
    Write-Error "apps/ directory '$AppsDir' does not exist."
    exit 1
}

$ExamplesDir = Join-Path $AppsDir "examples"
$Dest = Join-Path $ExamplesDir "embfetch"
$Kconfig = Join-Path $ExamplesDir "Kconfig"

if (-not (Test-Path $ExamplesDir -PathType Container)) {
    Write-Error "examples/ directory '$ExamplesDir' does not exist."
    exit 1
}

if (-not (Test-Path $Kconfig -PathType Leaf)) {
    Write-Error "Kconfig not found at '$Kconfig'."
    exit 1
}

# --- Create embfetch app directory ---
if (Test-Path $Dest -PathType Container) {
    Write-Warning "'$Dest' already exists. Files may be overwritten."
} else {
    New-Item -ItemType Directory -Path $Dest | Out-Null
    Write-Host "Created '$Dest'."
}

# --- Copy files ---
Write-Host "Copying embfetch NuttX port files..."

Copy-Item "$Src\sysinfo_rtos_common.c"         "$Dest\sysinfo_rtos_common.c"
Copy-Item "$Src\include\embed_sysinfo.h"       "$Dest\embed_sysinfo.h"
Copy-Item "$Src\rtos\nuttx\embfetch_main.c"   "$Dest\embfetch_main.c"
Copy-Item "$Src\rtos\nuttx\sysinfo_nuttx.c"   "$Dest\sysinfo_nuttx.c"
Copy-Item "$Src\rtos\nuttx\Kconfig"           "$Dest\Kconfig"
Copy-Item "$Src\rtos\nuttx\Makefile"          "$Dest\Makefile"
Copy-Item "$Src\rtos\nuttx\Make.defs"         "$Dest\Make.defs"
Copy-Item "$Src\rtos\nuttx\include\logo.h"    "$Dest\logo.h"

Write-Host "Files copied to '$Dest':"
Write-Host "  sysinfo_rtos_common.c"
Write-Host "  embed_sysinfo.h"
Write-Host "  embfetch_main.c"
Write-Host "  sysinfo_nuttx.c"
Write-Host "  Kconfig"
Write-Host "  Makefile"
Write-Host "  Make.defs"
Write-Host "  logo.h"

# --- Edit apps/examples/Kconfig ---
Write-Host ""
Write-Host "Updating '$Kconfig'..."

$KconfigContent = Get-Content $Kconfig -Raw

if ($KconfigContent -match "examples/embfetch/Kconfig") {
    Write-Warning "Kconfig already contains embfetch entry. Skipping."
} else {
    $KconfigContent += "`r`nsource `"examples/embfetch/Kconfig`"`r`n"
    Set-Content $Kconfig $KconfigContent -NoNewline
    Write-Host "Kconfig updated."
}

# --- Next steps ---
Write-Host ""
Write-Host "Next steps:"
Write-Host "  1. Run 'make menuconfig' and enable embfetch under Application Configuration > Examples."
Write-Host "  2. Run 'make' to compile embfetch into the kernel."
Write-Host "  3. After compilation, 'embfetch' will be available as an NSH command."