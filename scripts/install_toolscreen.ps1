param(
    [string]$DllPath = "E:\i\Toolscreen\build\Toolscreen.dll",
    [string]$TargetJarPath = "E:\Minecraft\MultiMC-Data\instances\Ranked #1\Toolscreen.jar"
)

$ErrorActionPreference = "Stop"

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$repoRoot = (Resolve-Path (Join-Path $scriptDir "..")).Path

Write-Host "[Toolscreen] Repo: $repoRoot"
Write-Host "[Toolscreen] DLL: $DllPath"
Write-Host "[Toolscreen] Target jar: $TargetJarPath"

if (-not (Test-Path $DllPath)) {
    throw "Built DLL not found at '$DllPath'."
}

if (-not (Test-Path $TargetJarPath)) {
    throw "Target jar not found at '$TargetJarPath'."
}

$timestamp = Get-Date -Format "yyyyMMdd_HHmmss"
$backupPath = "$TargetJarPath.bak_integratednbb_$timestamp"
Copy-Item -LiteralPath $TargetJarPath -Destination $backupPath -Force
Write-Host "[Toolscreen] Backup created: $backupPath"

$tempZip = Join-Path $env:TEMP ("toolscreen_patch_" + [System.Guid]::NewGuid().ToString("N") + ".zip")
Copy-Item -LiteralPath $TargetJarPath -Destination $tempZip -Force

Add-Type -AssemblyName System.IO.Compression
Add-Type -AssemblyName System.IO.Compression.FileSystem

$zip = [System.IO.Compression.ZipFile]::Open($tempZip, [System.IO.Compression.ZipArchiveMode]::Update)
try {
    $entryPath = "dlls/Toolscreen.dll"
    $existing = $zip.GetEntry($entryPath)
    if ($null -ne $existing) {
        $existing.Delete()
    }

    [System.IO.Compression.ZipFileExtensions]::CreateEntryFromFile(
        $zip,
        $dllPath,
        $entryPath,
        [System.IO.Compression.CompressionLevel]::Optimal
    ) | Out-Null
}
finally {
    $zip.Dispose()
}

try {
    Move-Item -Path $tempZip -Destination $TargetJarPath -Force
}
catch {
    Copy-Item -Path $tempZip -Destination $TargetJarPath -Force
    Remove-Item -Path $tempZip -Force -ErrorAction SilentlyContinue
}

Write-Host "[Toolscreen] Installed new dlls/Toolscreen.dll into:"
Write-Host "  $TargetJarPath"
Write-Host "[Toolscreen] Done."
