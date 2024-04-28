# File:    ms-release-sequence.ps1
# Author:  Garrett Carter
# Purpose: Execute release flashing sequence

Write-Host "**********************************************************"
Write-Host "Flashing eep_data_write..."
Write-Host "**********************************************************"
& $PSScriptRoot\ms-flash-hex.ps1 -desired_fuses_file $PSScriptRoot\fuses.txt -hex_to_flash $PSScriptRoot\eep_data_write.hex
if ($LASTEXITCODE -ne 0) {
    Exit $LASTEXITCODE
}

Write-Host "**********************************************************"
Write-Host "Waiting for eep_data_write to execute..."
Write-Host "**********************************************************"
Start-Sleep -Seconds 3

Write-Host "**********************************************************"
Write-Host "Flashing neo_driver_app..."
Write-Host "**********************************************************"
& $PSScriptRoot\ms-flash-hex.ps1 -desired_fuses_file $PSScriptRoot\fuses.txt -hex_to_flash $PSScriptRoot\neo_driver_app.hex
if ($LASTEXITCODE -ne 0) {
    Exit $LASTEXITCODE
}

Read-Host -Prompt "Flashing complete. Press enter to exit" | Out-Null

Exit 0