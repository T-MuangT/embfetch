# setup_zephyr_module.ps1
# Adds embfetch as a west module to your Zephyr project's west.yml and copies prj.conf.example.
# Usage: .\setup_zephyr_module.ps1 <project_root> [west_yml_path]
#   project_root   : path to your Zephyr project root
#   west_yml_path  : path to your west.yml (optional, defaults to <project_root>\west.yml)

param (
    [Parameter(Mandatory=$true, Position=0)]
    [string]$ProjectRoot,

    [Parameter(Mandatory=$false, Position=1)]
    [string]$WestYmlPath
)

$ErrorActionPreference = "Stop"

$EmbfetchRoot = Resolve-Path "$PSScriptRoot\..\.."

# --- Validate paths ---
if (-not (Test-Path $ProjectRoot -PathType Container)) {
    Write-Error "Project root '$ProjectRoot' does not exist."
    exit 1
}

if (-not $WestYmlPath) {
    $WestYmlPath = Join-Path $ProjectRoot "west.yml"
}

if (-not (Test-Path $WestYmlPath -PathType Leaf)) {
    Write-Error "west.yml not found at '$WestYmlPath'. Specify it as the second argument."
    exit 1
}

# --- Check for existing embfetch entry ---
$WestContent = Get-Content $WestYmlPath -Raw

if ($WestContent -match "embfetch") {
    Write-Warning "west.yml already contains an embfetch entry. Skipping."
} else {
    $WestContent += @"

    # embfetch
    - name: embfetch-rtos
      url: https://github.com/yourname/embfetch-rtos
      revision: main
      path: modules/embfetch-rtos
"@
    Set-Content $WestYmlPath $WestContent -NoNewline
    Write-Host "west.yml updated."
}

# --- Copy prj.conf.example ---
Write-Host ""
Write-Host "Copying prj.conf.example to '$ProjectRoot'..."

Copy-Item "$EmbfetchRoot\src\rtos\zephyr\vanilla\prj.conf.example" `
          "$ProjectRoot\prj_embfetch.conf"

Write-Host "Copied as 'prj_embfetch.conf'."

# --- Next steps ---
Write-Host ""
Write-Host "Next steps:"
Write-Host "  1. Update the 'url' in west.yml to point to your actual embfetch-rtos repository."
Write-Host "  2. Review 'prj_embfetch.conf' and merge the needed configs into your 'prj.conf':"
Write-Host "       CONFIG_SHELL=y"
Write-Host "       CONFIG_SHELL_STACK_SIZE=(your_value)"
Write-Host "       CONFIG_SYS_HEAP_RUNTIME_STATS=y"
Write-Host "  3. Run 'west update' to fetch the embfetch-rtos module."
Write-Host "  4. Add entry and register the embfetch command in your main or shell source file."
Write-Host "  5. Run 'west build -b <your_target_board>' to build."