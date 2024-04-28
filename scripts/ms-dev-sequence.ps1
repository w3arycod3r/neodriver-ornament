# File:    ms-dev-sequence.ps1
# Author:  Garrett Carter
# Purpose: Execute development test sequence

Write-Host "**********************************************************"
Write-Host "Building eep_data_write..."
Write-Host "**********************************************************"
& $PSScriptRoot\ms-build-config.ps1 -config_folder $PSScriptRoot\..\microchip-studio\neo_driver_app\eep_data_write -clean_build "true"
if ($LASTEXITCODE -ne 0) {
    Exit $LASTEXITCODE
}
Write-Host "**********************************************************"
Write-Host "Flashing eep_data_write..."
Write-Host "**********************************************************"
& $PSScriptRoot\ms-flash-hex.ps1 -desired_fuses_file $PSScriptRoot\..\microchip-studio\fuses.txt -hex_to_flash $PSScriptRoot\..\microchip-studio\neo_driver_app\eep_data_write\eep_data_write.hex
if ($LASTEXITCODE -ne 0) {
    Exit $LASTEXITCODE
}

Write-Host "**********************************************************"
Write-Host "Waiting for eep_data_write to execute..."
Write-Host "**********************************************************"
Start-Sleep -Seconds 3

Write-Host "**********************************************************"
Write-Host "Building neo_driver_app..."
Write-Host "**********************************************************"
& $PSScriptRoot\ms-build-config.ps1 -config_folder $PSScriptRoot\..\microchip-studio\neo_driver_app\Release -clean_build "true"
if ($LASTEXITCODE -ne 0) {
    Exit $LASTEXITCODE
}
Write-Host "**********************************************************"
Write-Host "Flashing neo_driver_app..."
Write-Host "**********************************************************"
& $PSScriptRoot\ms-flash-hex.ps1 -desired_fuses_file $PSScriptRoot\..\microchip-studio\fuses.txt -hex_to_flash $PSScriptRoot\..\microchip-studio\neo_driver_app\Release\neo_driver_app.hex
if ($LASTEXITCODE -ne 0) {
    Exit $LASTEXITCODE
}

Exit 0