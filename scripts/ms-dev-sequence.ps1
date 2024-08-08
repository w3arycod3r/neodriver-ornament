# File:    ms-dev-sequence.ps1
# Author:  Garrett Carter
# Purpose: Execute development test sequence

Write-Host "**********************************************************"
Write-Host "Building eep_data_write..."
Write-Host "**********************************************************"
$config_folder = "$PSScriptRoot\..\microchip-studio\neo_driver_app\eep_data_write"
$exec_name = "eep_data_write"
$eep_hex_file = "${config_folder}\${exec_name}.hex"

& $PSScriptRoot\ms-build-config.ps1 -config_folder $config_folder -exec_name $exec_name -clean_build "true"
if ($LASTEXITCODE -ne 0) {
    Exit $LASTEXITCODE
}

Write-Host "**********************************************************"
Write-Host "Building neo_driver_app..."
Write-Host "**********************************************************"
$config_folder = "$PSScriptRoot\..\microchip-studio\neo_driver_app\Release"
$exec_name = "neo_driver_app"
$main_hex_file = "${config_folder}\${exec_name}.hex"

& $PSScriptRoot\ms-build-config.ps1 -config_folder $config_folder -exec_name $exec_name -clean_build "true"
if ($LASTEXITCODE -ne 0) {
    Exit $LASTEXITCODE
}

Write-Host "**********************************************************"
Write-Host "Flashing eep_data_write..."
Write-Host "**********************************************************"
& $PSScriptRoot\ms-flash-hex.ps1 -desired_fuses_file $PSScriptRoot\..\microchip-studio\fuses.txt -hex_to_flash $eep_hex_file
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
& $PSScriptRoot\ms-flash-hex.ps1 -desired_fuses_file $PSScriptRoot\..\microchip-studio\fuses.txt -hex_to_flash $main_hex_file
if ($LASTEXITCODE -ne 0) {
    Exit $LASTEXITCODE
}

Exit 0