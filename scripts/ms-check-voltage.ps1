# File:    ms-check-voltage.ps1
# Author:  Garrett Carter
# Purpose: Check and print target voltage

$ErrorActionPreference = 'Stop'
if (-Not (Get-Command atprogram.exe -ErrorAction SilentlyContinue)) {
    Write-Host "Adding Atmel Studio dirs to process PATH variable..."
    $Env:PATH = "C:\Program Files (x86)\Atmel\Studio\7.0\;C:\Program Files (x86)\Atmel\Studio\7.0\shellutils\;C:\Program Files (x86)\Atmel\Studio\7.0\atbackend\;C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\;$Env:PATH"
}
# Use verbose flag for atprogram commands? 1 for yes, 0 for no
$atprog_use_verbose = 0

if ($atprog_use_verbose -eq 1){
    $atprog_verbose = "-v"
} else {
    $atprog_verbose = $null
}

Write-Host "Checking target voltage..."
$atprogram_command = "atprogram.exe $atprog_verbose --force -t atmelice -i ISP -d attiny85 --externalreset info --voltage"
Write-Host $atprogram_command
Invoke-Expression $atprogram_command
if ($LASTEXITCODE -ne 0) {
    Exit $LASTEXITCODE
}

Exit 0