# File:    ms-recover-tool.ps1
# Author:  Garrett Carter
# Purpose: Sometimes the programmer can be stuck in a bad state, or the target can be in debugWIRE mode.
#          This script does a song and a dance to try to recover the tool and target.
param (
    # Need fuse settings to properly disable debugWIRE. These fuse settings should have DWEN set to 0.
    [Parameter(Mandatory)]
    [string]
    $desired_fuses_file
)

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

# atprogram.exe $atprog_verbose -t atmelice -i ISP -d attiny85 info

Write-Host "Attempting to recover atmelice tool and target..."

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

Exit 0