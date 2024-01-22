@REM File:    ms_build_flash_config.bat
@REM Author:  Garrett Carter
@REM Purpose: Build a config (passed in param) in Microchip Studio, then flash it

@echo off

pushd "%~dp0"

set PATH=C:\Program Files (x86)\Atmel\Studio\7.0\;C:\Program Files (x86)\Atmel\Studio\7.0\shellutils\;C:\Program Files (x86)\Atmel\Studio\7.0\atbackend;%PATH%
set config=%1
set elf_filename=%2
set build_folder=..\microchip-studio\neo_driver_app\%config%

pushd %build_folder%
@REM make.exe clean || goto :error
make.exe all --jobs 8 --output-sync || goto :error
atprogram.exe -v -t atmelice -i ISP -d attiny85 program --chiperase -f %elf_filename% --verify || goto :error

:error
popd
popd
if %errorlevel% neq 0 exit /b %errorlevel%