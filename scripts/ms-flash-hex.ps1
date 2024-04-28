# File:    ms-flash-hex.ps1
# Author:  Garrett Carter
# Purpose: Robustly flash a hex file passed in via params
param (
    [Parameter(Mandatory)]
    [string]
    $hex_to_flash,
    # Need fuse settings to properly disable debugWIRE
    [Parameter(Mandatory)]
    [string]
    $desired_fuses_file
)

$ErrorActionPreference = 'Stop'
if (-Not (Get-Command atprogram.exe -ErrorAction SilentlyContinue)) {
    Write-Host "Adding Atmel Studio dirs to process PATH variable..."
    $Env:PATH = "C:\Program Files (x86)\Atmel\Studio\7.0\;C:\Program Files (x86)\Atmel\Studio\7.0\shellutils\;C:\Program Files (x86)\Atmel\Studio\7.0\atbackend\;C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\;$Env:PATH"
}
$hex_to_flash = [System.IO.Path]::GetFullPath($hex_to_flash)
$hex_dump = Get-Content $hex_to_flash

# Use verbose flag for atprogram commands? 1 for yes, 0 for no
$atprog_use_verbose = 0

if ($atprog_use_verbose -eq 1){
    $atprog_verbose = "-v"
} else {
    $atprog_verbose = $null
}

# atprogram.exe $atprog_verbose -t atmelice -i ISP -d attiny85 info

Write-Host "Issuing reset to atmelice..."
$atprogram_command = "atprogram $atprog_verbose --force -t atmelice reboot"
Write-Host $atprogram_command
Invoke-Expression $atprogram_command
if ($LASTEXITCODE -ne 0) {
    Exit $LASTEXITCODE
}

Write-Host "Waiting for atmelice to reset..."
Start-Sleep -Seconds 2

Write-Host "Attempting to disable debugWIRE interface..."
Write-Host "(Failure is expected if the target was not in debugWIRE mode)"
$atprogram_command = "atprogram.exe $atprog_verbose --force -t atmelice -i debugWIRE -d attiny85 --externalreset dwdisable"
Write-Host $atprogram_command
Invoke-Expression $atprogram_command

# Clear DWEN fuse via ISP interface
& $PSScriptRoot\ms-update-fuses.ps1 $desired_fuses_file
if ($LASTEXITCODE -ne 0) {
    Exit $LASTEXITCODE
}

Write-Host "Checking target voltage..."
$atprogram_command = "atprogram.exe $atprog_verbose --force -t atmelice -i ISP -d attiny85 --externalreset info --voltage"
Write-Host $atprogram_command
Invoke-Expression $atprogram_command
if ($LASTEXITCODE -ne 0) {
    Exit $LASTEXITCODE
}

Write-Host "Flashing $hex_to_flash"
$atprogram_command = "atprogram.exe $atprog_verbose --force -t atmelice -i ISP -d attiny85 --externalreset program --chiperase -f $hex_to_flash --verify"
Write-Host $atprogram_command
Invoke-Expression $atprogram_command
if ($LASTEXITCODE -ne 0) {
    Exit $LASTEXITCODE
}

Exit 0