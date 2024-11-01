# File:    ms-release-sequence.ps1
# Author:  Garrett Carter
# Purpose: Execute release flashing sequence

$ErrorActionPreference = 'Stop'
$desired_fuses_file = "$PSScriptRoot\fuses.txt"

# Allow the other scripts (downloaded from web, unsigned) to run
# Running this script initially will likely trigger a security pop-up.
Set-ExecutionPolicy -ExecutionPolicy Bypass -Scope Process

Write-Host "**********************************************************"
Write-Host "Updating fuses..."
Write-Host "**********************************************************"
& $PSScriptRoot\ms-update-fuses.ps1 $desired_fuses_file
# Failure of updating fuses is expected if the target was in debugWIRE mode, or the tool is in a bad state.
# We will handle that case later in the sequence during the flashing.

Write-Host "**********************************************************"
Write-Host "Flashing eep_data_write..."
Write-Host "**********************************************************"
& $PSScriptRoot\ms-flash-hex.ps1 -desired_fuses_file $desired_fuses_file -hex_to_flash $PSScriptRoot\eep_data_write.hex
if ($LASTEXITCODE -ne 0) {
    Write-Host
    Write-Host "FAILURE! Did you remember to unzip?"
    Read-Host -Prompt "Press enter to exit" | Out-Null
    Exit $LASTEXITCODE
}

Write-Host "**********************************************************"
Write-Host "Waiting for eep_data_write to execute..."
Write-Host "**********************************************************"
Start-Sleep -Seconds 3

Write-Host "**********************************************************"
Write-Host "Flashing neo_driver_app..."
Write-Host "**********************************************************"
& $PSScriptRoot\ms-flash-hex.ps1 -desired_fuses_file $desired_fuses_file -hex_to_flash $PSScriptRoot\neo_driver_app.hex
if ($LASTEXITCODE -ne 0) {
    Write-Host
    Write-Host "FAILURE! Did you remember to unzip?"
    Read-Host -Prompt "Press enter to exit" | Out-Null
    Exit $LASTEXITCODE
}

Read-Host -Prompt "Flashing complete. Press enter to exit" | Out-Null

Exit 0