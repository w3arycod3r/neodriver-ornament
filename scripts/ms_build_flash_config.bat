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
:: Use verbose flag for atprogram commands? 1 for yes, 0 for no
set atprog_use_verbose=0

if %atprog_use_verbose% == 1 (
    set atprog_verbose=-v
) else (
    set atprog_verbose=
)

pushd %build_folder%

:: Invoke correct rule in makefile
if "%clean_opt%" == "clean" (
    make.exe clean || goto :error
)
make.exe all --jobs 8 --output-sync || goto :error

@REM atprogram.exe %atprog_verbose% -t atmelice -i ISP -d attiny85 info

echo Issuing reset to atmelice...
atprogram %atprog_verbose% --force -t atmelice reboot || goto :error

echo Waiting for atmelice to reset...
timeout /t 2 /nobreak > NUL

echo Attempting to disable debugWIRE interface...
echo (Failure is expected if the target was not in debugWIRE mode)
atprogram.exe %atprog_verbose% --force -t atmelice -i debugWIRE -d attiny85 --externalreset dwdisable
:: Clear DWEN fuse via ISP interface
call %script_dir%\ms_update_fuses.bat || goto :error

echo Checking target voltage...
atprogram.exe %atprog_verbose% --force -t atmelice -i ISP -d attiny85 --externalreset info --voltage

echo Flashing %elf_filename%
atprogram.exe %atprog_verbose% --force -t atmelice -i ISP -d attiny85 --externalreset program --chiperase -f %elf_filename% --verify || goto :error

:error
popd
popd
if %errorlevel% neq 0 exit /b %errorlevel%