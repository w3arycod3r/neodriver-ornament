# File:    ms-update-fuses.ps1
# Author:  Garrett Carter
# Purpose: Update fuses if needed using atprogram.exe and the fuses.txt file

$Env:PATH = "C:\Program Files (x86)\Atmel\Studio\7.0\;C:\Program Files (x86)\Atmel\Studio\7.0\shellutils\;C:\Program Files (x86)\Atmel\Studio\7.0\atbackend\;C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\;$Env:PATH"
$curr_fuses_file = [System.IO.Path]::GetFullPath("$PSScriptRoot\..\microchip-studio\curr_fuses.txt")
$desired_fuses_file = [System.IO.Path]::GetFullPath("$PSScriptRoot\..\microchip-studio\fuses.txt")
$ErrorActionPreference = 'Stop'

# Use verbose flag for atprogram commands? 1 for yes, 0 for no
$atprog_use_verbose = 0

if ($atprog_use_verbose -eq 1){
    $atprog_verbose = "-v"
} else {
    $atprog_verbose = $null
}

Write-Host "Reading attiny85 fuses..."
$atprogram_command = "atprogram.exe $atprog_verbose --force -t atmelice -i ISP -d attiny85 --externalreset read --fuses --size 3 --format hex --file $curr_fuses_file"
Write-Host $atprogram_command
Invoke-Expression $atprogram_command
if ($LASTEXITCODE -ne 0) {
    Exit $LASTEXITCODE
}

$desired_fuses_settings = Get-Content $desired_fuses_file
$curr_fuses_settings = Get-Content $curr_fuses_file

if ($curr_fuses_settings -eq $desired_fuses_settings) {
    Write-Host "Fuses set correctly to 0x$curr_fuses_settings"
} else {
    Write-Host "Fuses currently set to 0x$curr_fuses_settings, need to be updated to 0x$desired_fuses_settings"
    Write-Host "Writing attiny85 fuses..."
    $atprogram_command = "atprogram.exe $atprog_verbose --force -t atmelice -i ISP -d attiny85 --externalreset write --fuses --offset 0 --values $desired_fuses_settings --verify"
    Write-Host $atprogram_command
    Invoke-Expression $atprogram_command
    if ($LASTEXITCODE -ne 0) {
        Exit $LASTEXITCODE
    }
}

if (Test-Path $curr_fuses_file) {
    Remove-Item -Force $curr_fuses_file
}

Exit 0