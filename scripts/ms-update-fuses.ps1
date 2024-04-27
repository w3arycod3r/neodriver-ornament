# File:    ms_build_flash_config.bat
# Author:  Garrett Carter
# Purpose: Build a config (passed in param) in Microchip Studio, then flash it

# Change to script dir
Set-Location $PSScriptRoot

$PATH = "C:\Program Files (x86)\Atmel\Studio\7.0\;C:\Program Files (x86)\Atmel\Studio\7.0\shellutils\;C:\Program Files (x86)\Atmel\Studio\7.0\atbackend\;C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\;$Env:PATH"
$curr_fuses_file = ..\microchip-studio\curr_fuses.txt
$desired_fuses_file = ..\microchip-studio\fuses.txt

# Use verbose flag for atprogram commands? 1 for yes, 0 for no
$atprog_use_verbose = 0

if ($atprog_use_verbose == 1){
    $atprog_verbose = "-v"
} else {
    $atprog_verbose = ""
}

Write-Host Reading attiny85 fuses...
atprogram.exe $atprog_verbose --force -t atmelice -i ISP -d attiny85 --externalreset read --fuses --size 3 --format hex --file $curr_fuses_file || goto :error

set /p desired_fuses_settings=<%desired_fuses_file%
set /p curr_fuses_settings=<%curr_fuses_file%

fc /c %curr_fuses_file% %desired_fuses_file%
IF %ERRORLEVEL% EQU 0 (
    Echo Fuses set correctly to 0x%curr_fuses_settings%
) ELSE (
    Echo Fuses need to be updated to 0x%desired_fuses_settings%
    echo Writing attiny85 fuses...
    atprogram.exe %atprog_verbose% --force -t atmelice -i ISP -d attiny85 --externalreset write --fuses --offset 0 --values %desired_fuses_settings% --verify || goto :error
)

:error
if exist %curr_fuses_file% del /f /q %curr_fuses_file%
popd
if %errorlevel% neq 0 exit /b %errorlevel%