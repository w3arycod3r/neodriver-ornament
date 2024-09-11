# File:    ms-flash-hex.ps1
# Author:  Garrett Carter
# Purpose: Robustly flash a hex file passed in via params
param (
    [Parameter(Mandatory)]
    [string]
    $hex_to_flash,
    # Need fuse settings to properly disable debugWIRE. These fuse settings should have DWEN set to 0.
    [Parameter(Mandatory)]
    [string]
    $desired_fuses_file,
    # Number of times to attempt flashing before giving up
    [int]
    $max_flash_attempts = 2
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

$num_flash_attempts = 1
$flash_success = $false
$last_attempt = $false
while ($num_flash_attempts -le $max_flash_attempts) {

    if ($num_flash_attempts -eq $max_flash_attempts) {
        $last_attempt = $true
    }

    Write-Host "Flashing $hex_to_flash (Attempt $num_flash_attempts)"
    $atprogram_command = "atprogram.exe $atprog_verbose --force -t atmelice -i ISP -d attiny85 --externalreset program --chiperase -f $hex_to_flash --verify"
    Write-Host $atprogram_command
    Invoke-Expression $atprogram_command
    
    if ($LASTEXITCODE -ne 0) {
        # If we failed to flash, try to recover the tool and target before trying again.
        Write-Host "Failed to flash $hex_to_flash"
        if (-Not $last_attempt) {
            & $PSScriptRoot\ms-recover-tool.ps1 -desired_fuses_file $desired_fuses_file
        }
    } else {
        $flash_success = $true
        Write-Host "Successfully flashed $hex_to_flash"
        # If we successfully flashed, exit the loop
        break
    }
    $num_flash_attempts++
    
}

if (-Not $flash_success) {
    Write-Host "Failed to flash $hex_to_flash after $max_flash_attempts attempts"
    Exit 1
}

Exit 0