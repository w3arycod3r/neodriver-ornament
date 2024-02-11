@REM File:    ms_build_flash_config.bat
@REM Author:  Garrett Carter
@REM Purpose: Build a config (passed in param) in Microchip Studio, then flash it

@echo off

pushd "%~dp0"

set PATH=C:\Program Files (x86)\Atmel\Studio\7.0\;C:\Program Files (x86)\Atmel\Studio\7.0\shellutils\;C:\Program Files (x86)\Atmel\Studio\7.0\atbackend\;C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\;%PATH%
set config=%1
set elf_filename=%2
set clean_opt=%3
set build_folder=..\microchip-studio\neo_driver_app\%config%
set script_dir=%~dp0

pushd %build_folder%

:: Invoke correct rule in makefile
if "%clean_opt%" == "clean" (
    make.exe clean || goto :error
)
make.exe all --jobs 8 --output-sync || goto :error

@REM atprogram.exe -v -t atmelice -i ISP -d attiny85 info
@REM atprogram.exe -v -t atmelice -i ISP -d attiny85 info --voltage

:: Disable debugWIRE interface
atprogram.exe -v -t atmelice -i debugWIRE -d attiny85 dwdisable
:: Clear DWEN fuse via ISP interface
call %script_dir%\ms_update_fuses.bat || goto :error

:: Program elf file
atprogram.exe -v -t atmelice -i ISP -d attiny85 program --chiperase -f %elf_filename% --verify || goto :error

:error
popd
popd
if %errorlevel% neq 0 exit /b %errorlevel%