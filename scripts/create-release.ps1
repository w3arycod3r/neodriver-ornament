# File:    create-release.ps1
# Author:  Garrett Carter
# Purpose: Package hex files, fuse settings, and scripts into a zip for release

$ErrorActionPreference = 'Stop'
$releases_dir = "$PSScriptRoot\..\releases"
if (-Not (Test-Path $releases_dir)) {
    New-Item -Path $releases_dir -ItemType Directory | Out-Null
}

# Take the tag name (if one exists on current commit) or take a short SHA
$release_git_rev = $(git describe --exact-match --tags 2>$null || git rev-parse --short HEAD)
$release_zip_name = "neodriver-ornament-$release_git_rev.zip"
$release_zip_file = "$releases_dir\$release_zip_name"
$release_zip_file = [System.IO.Path]::GetFullPath($release_zip_file)

Write-Host "Creating release at $release_zip_file ..."

$params = @{
    Path = "$PSScriptRoot\..\microchip-studio\neo_driver_app\eep_data_write\eep_data_write.hex",
           "$PSScriptRoot\..\microchip-studio\neo_driver_app\Release\neo_driver_app.hex",
           "$PSScriptRoot\..\microchip-studio\fuses.txt",
           "$PSScriptRoot\..\scripts\ms-update-fuses.ps1",
           "$PSScriptRoot\..\scripts\ms-flash-hex.ps1",
           "$PSScriptRoot\..\scripts\ms-release-sequence.ps1",
           "$PSScriptRoot\..\scripts\ms-recover-tool.ps1",
           "$PSScriptRoot\..\scripts\run-me.ps1"
    CompressionLevel = "Fastest"
    DestinationPath = "$release_zip_file"
    Force = $true
}

Compress-Archive @params

Invoke-Item $releases_dir